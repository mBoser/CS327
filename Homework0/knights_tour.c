#include <stdio.h>
 
int valid_move(int x, int y, int grid[5][5])
{
    return (x >= 0 && x < 5 && y >= 0 && y < 5 && grid[x][y] == -1);
}
 
void reset_board(int grid[5][5]){
    for (int x = 0; x < 5; x++)
        for (int y = 0; y < 5; y++)
            grid[x][y] = -1;
}

void tour(int x, int y, int num_jumps, int grid[5][5], int x_moveset[5], int y_moveset[5], int path[25])
{

    grid[x][y] = num_jumps;
    int curr = (x+1)+(y*5);
    path[num_jumps-1] = curr;
    int k, next_x, next_y;
    if (num_jumps == 25){
        int i;
        for(i=0;i<24;i++){
            printf("%d, ", path[i]);
        }
        printf("%d", path[24]); //print last one without the comma
        printf("\n");
        grid[x][y] = -1;
        return;
    }

    for (k = 0; k < 8; k++) { //do all the moves in the set of possible moves
        next_x = x + x_moveset[k];
        next_y = y + y_moveset[k];
        if (valid_move(next_x, next_y, grid)) {
            tour(next_x, next_y, num_jumps + 1, grid, x_moveset, y_moveset, path);
        }
    }
    grid[x][y] = -1;

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
            int path[25];     
            
            grid[i][j] = 0;   
            tour(i, j, 1, grid, x_moveset, y_moveset, path);
        }
    }

    return 0;
}