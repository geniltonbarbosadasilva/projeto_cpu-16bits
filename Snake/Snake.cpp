#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#define FIELD_X 32
#define FIELD_Y 32
#define SNAKE_MAX 100

void sleep_Second(float t)
{
    clock_t timeInit, timeEnd;
    timeInit = clock();
    do
    {
        timeEnd = (clock() - timeInit);
    } while (timeEnd < t);
}

int getch_Key(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

int kbhit()
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

void clear(int field[][FIELD_Y])
{
    for (int i = 0; i < FIELD_X; i++)
    {
        for (int j = 0; j < FIELD_Y; j++)
        {
            field[i][j] = 0;
        }
    }
}

void makeFruit(int fruit[])
{
    srand(time(NULL));

    do
    {
        fruit[0] = rand() % 31 + 1;
    } while (fruit[0] == 0 || fruit[0] == 31);
    do
    {
        fruit[1] = rand() % 31 + 1;
    } while (fruit[1] == 0 || fruit[1] == 31);
}

void init(int field[][FIELD_Y], int body[][2], int *head, int fruit[])
{
    for (int i = 0; i < FIELD_X; i++)
    {
        for (int j = 0; j < FIELD_Y; j++)
        {
            field[i][j] = 0;
        }
    }

    for (int i = 0; i < 8; i++)
    {
        body[i][0] = 0;
        body[i][1] = 0;
    }

    makeFruit(fruit);
    body[0][0] = 3;
    body[0][1] = 3;
    body[1][0] = 2;
    body[1][1] = 3;
    body[2][0] = 1;
    body[2][1] = 3;
    *head = 0;
}

void print(int field[][FIELD_Y], int body[][2], int head, int len, int key, int oldKey)
{
    for (int i = 0; i < FIELD_X; i++)
    {
        for (int j = 0; j < FIELD_Y; j++)
        {
            if (field[i][j] == 1)
            {
                printf("*");
            }
            else if (field[i][j] == 2)
            {
                printf("+");
            }
            else if (field[i][j] == 3)
            {
                printf("@");
            }
            else if (field[i][j] == 4)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("%d %d\t| %d |\n", body[head][0], body[head][1], len);
    printf("Key: %c\nOldKey: %c\nHead: %d\n", key, oldKey, head);
}

void step(int body[][2], int len, int head)
{
    if (head == 0)
    {
        for (int i = len - 1; i > 0; i--)
        {
            body[i][0] = body[i - 1][0];
            body[i][1] = body[i - 1][1];
        }
    }else
    {
        for (int i = 0; i < head; i++)
        {
            body[i][0] = body[i + 1][0];
            body[i][1] = body[i + 1][1];
        }
    }
}

void twist(int *head, int len)
{
    if (*head == 0)
    {
        *head = len - 1;
    }
    else
    {
        *head = 0;
    }
}

int move(int key, int *oldKey, int body[][2], int *head, int fruit[], int len)
{
    if (key == 119)
    {
        // up
        if (*oldKey == 115)
        {
            twist(head, len);
            *oldKey = key;
        }
        step(body, len, *head);
        body[*head][0]--;
    }
    else if (key == 100)
    {
        // right
        if (*oldKey == 97)
        {
            twist(head, len);
            *oldKey = key;
        }
        step(body, len, *head);
        body[*head][1]++;
    }
    else if (key == 115)
    {
        // down
        if (*oldKey == 119)
        {
            twist(head, len);
            *oldKey = key;
        }
        step(body, len, *head);
        body[*head][0]++;
    }
    else if (key == 97)
    {
        // left
        if (*oldKey == 100)
        {
            twist(head, len);
            *oldKey = key;
        }
        step(body, len, *head);
        body[*head][1]--;
    }

    if (body[*head][0] == fruit[0] && body[*head][1] == fruit[1])
    {
        makeFruit(fruit);
        if( *head != 0 ){ 
            body[len][0] = body[*head][0];
            body[len][1] = body[*head][1];
            *head = len; 
        }
        return 1;
    }
    return 0;
}

void draw(int field[][FIELD_Y], int body[][2], int head, int fruit[], int len)
{
    clear(field);

    field[fruit[0]][fruit[1]] = 3;

    for (int i = 0; i < len; i++)
    {
        field[body[i][0]][body[i][1]] = 1;
    }

    field[body[head][0]][body[head][1]] = 2;

    for (int i = 0; i < FIELD_X; i++)
    {
        for (int j = 0; j < FIELD_Y; j++)
        {
            if (i == 0 || j == 0 || i == (FIELD_X - 1) || j == (FIELD_Y - 1))
            {
                field[i][j] = 4;
            }
        }
    }
}

int main()
{
    int field[FIELD_X][FIELD_Y];
    int body[SNAKE_MAX][2];
    int head;
    int fruit[2];
    int len = 3;
    int key = 115;
    int oldKey = 115;

    init(field, body, &head, fruit);
    // draw( field, body, len);
    // print(field);

    while (true)
    {
        system("clear");
        draw(field, body, head, fruit, len);
        print(field, body, head, len, key, oldKey);
        
        if (field[body[head][0]][body[head][1]] == 1 ||
            field[body[head][0]][body[head][1]] == 4)
        {
            //system("clear");
            printf("\n\t Loser \n\n");
            printf("\t| %d |\n\n", len);
            exit(0);
        }

        if (kbhit())
        {
            oldKey = key;
            key = getch_Key();
            len += move(key, &oldKey, body, &head, fruit, len);
        }
        else
        {
            len += move(key, &oldKey, body, &head, fruit, len);
            sleep_Second(250000);
        }

        if (field[body[head][0]][body[head][1]] == 1 ||
            field[body[head][0]][body[head][1]] == 4)
        {
            //system("clear");
            printf("\n\t Loser \n\n");
            printf("\t| %d |\n\n", len);
            exit(0);
        }
    }

    return 0;
}
