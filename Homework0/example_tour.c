// C program for Knight Tour problem
#include <stdio.h>
 
/* A utility function to check if i,j are valid indexes
   for N*N chessboard */
int valid_move(int x, int y, int grid[5][5])
{
    return (x >= 0 && x < 5 && y >= 0 && y < 5 && grid[x][y] == -1);
}
 
/* A utility function to print solution matrix sol[N][N] */
void printSolution(int sol[5][5])
{
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++)
            printf(" %2d ", sol[x][y]);
        printf("\n");
    }
}
 
/* This function solves the Knight Tour problem using
   Backtracking.  This function mainly uses solveKTUtil()
   to solve the problem. It returns false if no complete
   tour is possible, otherwise return true and prints the
   tour.
   Please note that there may be more than one solutions,
   this function prints one of the feasible solutions.  */


/* A recursive utility function to solve Knight Tour
   problem */

void reset_board(int grid[5][5]){
    for (int x = 0; x < 5; x++)
        for (int y = 0; y < 5; y++)
            grid[x][y] = -1;
}

int tour(int x, int y, int num_jumps, int grid[5][5], int x_moveset[5], int y_moveset[5])
{

    int curr = (x+1)+(y*5);
    int k, next_x, next_y;
    if (num_jumps == 25)
        printf("%d,", curr);
        return 1;
 
    /* Try all next moves from the current coordinate x, y
     */
    for (k = 0; k < 8; k++) {
        next_x = x + x_moveset[k];
        next_y = y + y_moveset[k];
        if (valid_move(next_x, next_y, grid)) {
            grid[x][y] = 0;
            if (tour(next_x, next_y, num_jumps + 1, grid, x_moveset, y_moveset) == 1){
                printf("%d ,", curr);
                return 1;
            }
            else
                grid[next_x][next_y] = -1; // backtracking
        }
    }
 
    return 0;
}
 
int main()
{
    int grid[5][5];
 
    
    int x_moveset[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
    int y_moveset[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };

    int i,j;

    for(i = 0; i < 5; i++){
        for(j = 0; j< 5; j++){
            reset_board(grid);     
            
            grid[i][j] = 0;   
            if (tour(i, j, 1, grid, x_moveset, y_moveset) != -1) {
                printf("\n");
            }

        }
    }

    return 0;

 
    /* Start from 0,0 and explore all tours using
       solveKTUtil() */

}