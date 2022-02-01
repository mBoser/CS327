#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

char PATH = '#';
char CLEARING = '.';
char TREE = '^';
char TALL_GRASS = ':';
char ROCK = '%';
char MART = 'M';
char CENTER = 'C';
char map[21][80];

int gen_rand(int bound){
    return (rand() % bound);
}

bool valid_space(int y, int x){
    if (x >= 80 || x < 0 || y < 0 || y >= 21 || map[y][x] == PATH || map[y][x] == ROCK || map[y][x] == CENTER || map[y][x] == MART){
        return false;
    }
    else{
        return true;
    }
}

void gen_paths(){
    int north_exit = gen_rand(78) + 1;
    int south_exit = gen_rand(78) + 1;
    int east_exit = gen_rand(19) + 1;
    int west_exit = gen_rand(19) + 1;

    int x, y;

    int y_offset = gen_rand(14)+3; //generating how "low" the path is going to go, decided on 17 so the path woulnt get too close to the bottom, and to leave space for mart/med center
    for(y = 0; y < y_offset; y++){
        map[y][north_exit] = PATH;
    }

    if (north_exit < south_exit){
        for(x = north_exit; x < south_exit + 1; x++){
            map[y_offset][x] = PATH;
        }
    }
    if (north_exit > south_exit){
        for(x = north_exit; x > south_exit - 1; x--){
            map[y_offset][x] = PATH;
        }
    }

    for(y = y_offset; y < 21; y++){
        map[y][south_exit] = PATH;
    }

    int x_offset = gen_rand(74)+3; //generating how "far right" the path is going to go, decided on 17 so the path woulnt get too close to the bottom, and to leave space for mart/med center
    for(x = 0; x < x_offset; x++){
        map[west_exit][x] = PATH;
    }

    if (west_exit < east_exit){
        for(y = west_exit; y < east_exit + 1; y++){
            map[y][x_offset] = PATH;
        }
    }
    if (west_exit > east_exit){
        for(y = west_exit; y > east_exit - 1; y--){
            map[y][x_offset] = PATH;
        }
    }

    for(x = x_offset; x < 80; x++){
        map[east_exit][x] = PATH;
    }

}

void gen_boarder(){
    int x, y;
    for (y = 0; y < 21; y++){
        for (x = 0; x < 80; x++){
            if ((y == 0 || y == 20 || x == 0 || x == 79) && map[y][x] != PATH){
                map[y][x] = ROCK;
            }
        }
    }
}

void gen_buildings(){
    bool center_placed = false;
    bool mart_placed = false;
    while(!center_placed && !mart_placed){
        int rand_center = gen_rand(60)+5;
        int rand_mart = gen_rand(60)+5;
        int y;
        if(!center_placed){
            for(y = 0; y < 21 && !center_placed; y++){
                if (map[y][rand_center] == '#'){
                    if(valid_space(y,rand_center+1) && valid_space(y,rand_center+2)){ //CHECK IF THE RIGHT IS CLEAR
                        if(valid_space(y+1,rand_center+1) && valid_space(y+1,rand_center+2)){ //RIGHT DOWN
                            map[y][rand_center+1] = CENTER;
                            map[y][rand_center+2] = CENTER;
                            map[y+1][rand_center+1] = CENTER;
                            map[y+1][rand_center+2] = CENTER;
                            center_placed=true;
                        }
                        else if(valid_space(y-1,rand_center+1) && valid_space(y-1,rand_center+2)){
                            map[y][rand_center+1] = CENTER;
                            map[y][rand_center+2] = CENTER;
                            map[y-1][rand_center+1] = CENTER;
                            map[y-1][rand_center+2] = CENTER;
                            center_placed=true;
                        }
                    }
                    else if(valid_space(y+1,rand_center) && valid_space(y+2,rand_center)){
                        if(valid_space(y+1,rand_center+1) && valid_space(y+2,rand_center+1)){
                            map[y+1][rand_center] = CENTER;
                            map[y+2][rand_center] = CENTER;
                            map[y+1][rand_center+1] = CENTER;
                            map[y+2][rand_center+1] = CENTER;
                            center_placed=true;
                        }
                        else if(valid_space(y+1,rand_center-1) && valid_space(y+2,rand_center-1)){
                            map[y+1][rand_center] = CENTER;
                            map[y+2][rand_center] = CENTER;
                            map[y+1][rand_center-1] = CENTER;
                            map[y+2][rand_center-1] = CENTER;
                            center_placed=true;
                        }
                    }
                    else if(valid_space(y-1,rand_center) && valid_space(y-2,rand_center)){
                        if(valid_space(y-1,rand_center+1) && valid_space(y-2,rand_center+1)){
                            map[y-1][rand_center] = CENTER;
                            map[y-2][rand_center] = CENTER;
                            map[y-1][rand_center+1] = CENTER;
                            map[y-2][rand_center+1] = CENTER;
                            center_placed=true;
                        }
                        else if(valid_space(y-1,rand_center-1) && valid_space(y-2,rand_center-1)){
                            map[y-1][rand_center] = CENTER;
                            map[y-2][rand_center] = CENTER;
                            map[y-1][rand_center-1] = CENTER;
                            map[y-2][rand_center-1] = CENTER;
                            center_placed=true;
                        }
                    }
                }
            }
        }
        if(center_placed){
            for(y = 0; y < 21 && !mart_placed; y++){
                if (map[y][rand_mart] == '#'){
                    if(valid_space(y,rand_mart+1) && valid_space(y,rand_mart+2)){
                        if(valid_space(y+1,rand_mart+1) && valid_space(y+1,rand_mart+2)){
                            map[y][rand_mart+1] = MART;
                            map[y][rand_mart+2] = MART;
                            map[y+1][rand_mart+1] = MART;
                            map[y+1][rand_mart+2] = MART;
                            mart_placed=true;
                        }
                        else if(valid_space(y-1,rand_mart+1) && valid_space(y-1,rand_mart+2)){
                            map[y][rand_mart+1] = MART;
                            map[y][rand_mart+2] = MART;
                            map[y+1][rand_mart+1] = MART;
                            map[y+1][rand_mart+2] = MART;
                            mart_placed=true;
                        }
                    }
                    else if(valid_space(y+1,rand_mart) && valid_space(y+2,rand_mart)){
                        if(valid_space(y+1,rand_mart+1) && valid_space(y+2,rand_mart+1)){
                            map[y+1][rand_mart] = MART;
                            map[y+2][rand_mart] = MART;
                            map[y+1][rand_mart+1] = MART;
                            map[y+2][rand_mart+1] = MART;
                            mart_placed=true;
                        }
                        else if(valid_space(y+1,rand_mart-1) && valid_space(y+2,rand_mart-1)){
                            map[y+1][rand_mart] = MART;
                            map[y+2][rand_mart] = MART;
                            map[y+1][rand_mart-1] = MART;
                            map[y+2][rand_mart-1] = MART;
                            mart_placed=true;
                        }
                    }
                    else if(valid_space(y-1,rand_mart) && valid_space(y-2,rand_mart)){
                        if(valid_space(y-1,rand_mart+1) && valid_space(y-2,rand_mart+1)){
                            map[y-1][rand_mart] = MART;
                            map[y-2][rand_mart] = MART;
                            map[y-1][rand_mart+1] = MART;
                            map[y-2][rand_mart+1] = MART;
                            mart_placed=true;
                        }
                        else if(valid_space(y+1,rand_mart-1) && valid_space(y+2,rand_mart-1)){
                            map[y-1][rand_mart] = MART;
                            map[y-2][rand_mart] = MART;
                            map[y-1][rand_mart-1] = MART;
                            map[y-2][rand_mart-1] = MART;
                            mart_placed=true;
                        }
                    }
                }
            }
        }

    }
}

char get_mixed_terrain(){
    int rand = gen_rand(100);
    if(rand < 2){
        return ROCK;
    }
    else if(rand >= 2 && rand < 4){
        return TREE;
    }
    else{
        return CLEARING;
    }
}

void place_terrain(int x, int y, char terrain){
    int i, j;
    for(i = y-5; i < y+5; i++){
        for(j = x-10; j< x+10; j++){
            if (valid_space(i,j)){
                map[i][j] = terrain;
            }
        }
    }
}

void gen_terrain(){
    int grass_1_x = gen_rand(77)+1;
    int grass_1_y = gen_rand(18)+1;
    place_terrain(grass_1_x,grass_1_y, TALL_GRASS);

    int grass_2_x = gen_rand(77)+1;
    int grass_2_y = gen_rand(18)+1;
    place_terrain(grass_2_x,grass_2_y, TALL_GRASS);

    int clearing_1_x = gen_rand(77)+1;
    int clearing_1_y = gen_rand(18)+1;
    place_terrain(clearing_1_x,clearing_1_y, CLEARING);

    int clearing_2_x = gen_rand(77)+1;
    int clearing_2_y = gen_rand(18)+1;
    place_terrain(clearing_2_x,clearing_2_y, CLEARING);

}
 
int main()
{
    srand(time(NULL));

    int i, j;
    for(i = 0; i<21; i++){
        for(j = 0; j<80; j++){
            map[i][j] = get_mixed_terrain();
        }
    }
    gen_paths();
    gen_boarder();
    gen_buildings();
    gen_terrain();
    for(i = 0; i<21; i++){
        for(j = 0; j<80; j++){
            if ((map[i][j] == CLEARING) || map[i][j] == TALL_GRASS){
                printf(ANSI_COLOR_GREEN   "%c", map[i][j]);
                printf(ANSI_COLOR_RESET);
            }else if (map[i][j] == ROCK){
                printf("%c", map[i][j]);
            }else if (map[i][j] == TREE){
                printf(ANSI_COLOR_YELLOW   "%c", map[i][j]);
                printf(ANSI_COLOR_RESET);
            }else if(map[i][j] == PATH){
                printf(ANSI_COLOR_RED   "%c", map[i][j]);
                printf(ANSI_COLOR_RESET);
            }else if((map[i][j] == MART) || map[i][j] == CENTER){
                printf(ANSI_COLOR_MAGENTA   "%c", map[i][j]);
                printf(ANSI_COLOR_RESET);
            }
            else{
                printf("%c", map[i][j]);
            }
            
        }
        printf("\n");
    }

    return 0;
}