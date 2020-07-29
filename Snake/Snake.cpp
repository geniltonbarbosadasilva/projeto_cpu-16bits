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

void makeFruit(int fruit[]){
    srand(time(NULL));
    fruit[0] = rand() % 31 + 1;
    fruit[1] = rand() % 31 + 1;
}

void init(int field[][FIELD_Y], int body[][2], int fruit[])
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
}

void print(int field[][FIELD_Y], int body[][2], int len)
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
                printf("0");
            }            
            else if ( i == 0 || j == 0 || i == (FIELD_X-1) || j == (FIELD_Y-1))
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
    printf("%d %d\t| %d |\n", body[0][0], body[0][1], len);
    printf("%d %d\n", body[1][0], body[1][1]);
    printf("%d %d\n", body[2][0], body[2][1]);
}

void step(int body[][2], int len)
{
    for (int i = len - 1; i > 0; i--)
    {
        body[i][0] = body[i - 1][0];
        body[i][1] = body[i - 1][1];
    }
}

int move(int key, int body[][2], int fruit[], int len)
{
    if (key == 119)
    {
        // up
        step(body, len);
        body[0][0]--;
    } 
    else if (key == 100)
    {
        // right
        step(body, len);
        body[0][1]++;
    }
    else if (key == 115)
    {
        // down
        step(body, len);
        body[0][0]++;
    }
    else if (key == 97)
    {
        // left
        step(body, len);
        body[0][1]--;
    }

    if(body[0][0] == fruit[0] && body[0][1] == fruit[1]){
        makeFruit(fruit);
        return 10;
    }
    return 0;
}

void draw(int field[][FIELD_Y], int body[][2], int fruit[], int len)
{
    clear(field);

    field[ fruit[0] ][ fruit[1] ] = 2;

    for (int i = 0; i < len; i++)
    {
        field[body[i][0]][body[i][1]] = 1;
    }    
}

int main()
{
    int field[FIELD_X][FIELD_Y];
    int body[SNAKE_MAX][2];
    int fruit[2];
    int len = 3;    
    int key = 115;

    init(field, body, fruit);
    // draw( field, body, len);
    // print(field);

    while (true)
    {
        system("clear");
        draw(field, body, fruit, len);
        print(field, body, len);

        if (kbhit())
        {            
            key = getch_Key();
            len += move( key, body, fruit, len);
        }
        else
        {
            len += move( key, body, fruit, len);
        }
        sleep_Second(150000);
    }

    return 0;
}
