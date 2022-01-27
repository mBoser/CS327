#include <stdio.h>
#include <string.h>

char* jump(int x, int y, int visited[],int numJumps, int curr)
{
    if (numJumps == 25){
        printf("%d, ", visited[numJumps]);
        jump(visited, numJumps-1, curr);
    }

    reutrn NULL;
}

int is_valid(int x, int y)
{
    if (x > 0 && x <= 5 && y > 0 && y <= 5) {
        return 0;
    }
    else {
        return 1;
    }
}

int main(int argc, char *argv[])
{
    int grid[5][5];
    int i, j;
    int count = 1;
    int visited_arr[25];
    for (j = 0; j < 5; j++){
        for (i = 0; i < 5; i++){
            grid[j][i]=count;
            jump(visited_arr, 0, count);
            //printf("%3d", grid[j][i]);

            count++;
        }
        printf("\n");
    }
    
    return 0; 
}