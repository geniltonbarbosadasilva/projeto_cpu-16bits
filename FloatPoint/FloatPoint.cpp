#include <stdlib.h>
#include <stdio.h> 
#include <termios.h>
#include <unistd.h>

int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

void init(int mat[][16]){
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            mat[i][j] = 0;
        }        
    }
    mat[0][0] = 1;
}

void print(int mat[][16]){
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            if (mat[i][j] == 1)
            {
                printf("*");
            }
            printf(" ");
        }        
        printf("\n");
    }
}

void up(int mat[][16], int x){
    // mat[x-1] = mat[x]
    // mat[x] = 0
    for (int i = 0; i < 16; i++)
    {
        mat[x-1][i] = mat[x][i];
        mat[x][i] = 0;
    }    
}

void down(int mat[][16], int x){
    // mat[x+1] = mat[x]
    // mat[x] = 0
    for (int i = 0; i < 16; i++)
    {
        mat[x+1][i] = mat[x][i];
        mat[x][i] = 0;
    }
}

void left(int mat[][16], int x){
    // Shift_left mat[x]
    for (int i = 0; i < 15; i++)
    {
        mat[x][i] = mat[x][i+1];
    }
    mat[x][15] = 0;
}

void right(int mat[][16], int x){
    // Shift_right mat[x]
    for (int i = 15; i > 0; i--)
    {
        mat[x][i] = mat[x][i-1];
    }
    mat[x][0] = 0;    
}

int main() {
    int mat[8][16];
    int x = 0;
    char key;

    init(mat);
    // print(mat);
    
    while(true){
        key = getch();
        if (key == 113){
            break;
        }
        if (key == 119){
            // mat[x-1] = mat[x]
            // mat[x] = 0            
            up(mat, x);
            x--;
        }
        if (key == 100){
            // Shift_right mat[x]
            right(mat, x);            
        }
        if (key == 115){
            // mat[x+1] = mat[x]
            // mat[x] = 0
            down(mat, x);
            x++;
        }
        if (key == 97){
            // Shift_left mat[x]
            left(mat, x);
        }

        system("clear");
        print(mat);
    }    

    return 0;
}
