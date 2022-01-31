#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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
    if (x >= 80 || x < 0 || y < 0 || y >= 21 || map[y][x] == '#' || map[y][x] == '%' || map[y][x] == 'C' || map[y][x] == 'M'){
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
            if ((y == 0 || y == 20 || x == 0 || x == 79) && map[y][x] != '#'){
                map[y][x] = '%';
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
                            map[y][rand_center+1] = 'C';
                            map[y][rand_center+2] = 'C';
                            map[y+1][rand_center+1] = 'C';
                            map[y+1][rand_center+2] = 'C';
                            center_placed=true;
                        }
                        else if(valid_space(y-1,rand_center+1) && valid_space(y-1,rand_center+2)){
                            map[y][rand_center+1] = 'C';
                            map[y][rand_center+2] = 'C';
                            map[y-1][rand_center+1] = 'C';
                            map[y-1][rand_center+2] = 'C';
                            center_placed=true;
                        }
                    }
                    else if(valid_space(y+1,rand_center) && valid_space(y+2,rand_center)){
                        if(valid_space(y+1,rand_center+1) && valid_space(y+2,rand_center+1)){
                            map[y+1][rand_center] = 'C';
                            map[y+2][rand_center] = 'C';
                            map[y+1][rand_center+1] = 'C';
                            map[y+2][rand_center+1] = 'C';
                            center_placed=true;
                        }
                        else if(valid_space(y+1,rand_center-1) && valid_space(y+2,rand_center-1)){
                            map[y+1][rand_center] = 'C';
                            map[y+2][rand_center] = 'C';
                            map[y+1][rand_center-1] = 'C';
                            map[y+2][rand_center-1] = 'C';
                            center_placed=true;
                        }
                    }
                    else if(valid_space(y-1,rand_center) && valid_space(y-2,rand_center)){
                        if(valid_space(y-1,rand_center+1) && valid_space(y-2,rand_center+1)){
                            map[y-1][rand_center] = 'C';
                            map[y-2][rand_center] = 'C';
                            map[y-1][rand_center+1] = 'C';
                            map[y-2][rand_center+1] = 'C';
                            center_placed=true;
                        }
                        else if(valid_space(y-1,rand_center-1) && valid_space(y-2,rand_center-1)){
                            map[y-1][rand_center] = 'C';
                            map[y-2][rand_center] = 'C';
                            map[y-1][rand_center-1] = 'C';
                            map[y-2][rand_center-1] = 'C';
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
                            map[y][rand_mart+1] = 'M';
                            map[y][rand_mart+2] = 'M';
                            map[y+1][rand_mart+1] = 'M';
                            map[y+1][rand_mart+2] = 'M';
                            mart_placed=true;
                        }
                        else if(valid_space(y-1,rand_mart+1) && valid_space(y-1,rand_mart+2)){
                            map[y][rand_mart+1] = 'M';
                            map[y][rand_mart+2] = 'M';
                            map[y+1][rand_mart+1] = 'M';
                            map[y+1][rand_mart+2] = 'M';
                            mart_placed=true;
                        }
                    }
                    else if(valid_space(y+1,rand_mart) && valid_space(y+2,rand_mart)){
                        if(valid_space(y+1,rand_mart+1) && valid_space(y+2,rand_mart+1)){
                            map[y+1][rand_mart] = 'M';
                            map[y+2][rand_mart] = 'M';
                            map[y+1][rand_mart+1] = 'M';
                            map[y+2][rand_mart+1] = 'M';
                            mart_placed=true;
                        }
                        else if(valid_space(y+1,rand_mart-1) && valid_space(y+2,rand_mart-1)){
                            map[y+1][rand_mart] = 'M';
                            map[y+2][rand_mart] = 'M';
                            map[y+1][rand_mart-1] = 'M';
                            map[y+2][rand_mart-1] = 'M';
                            mart_placed=true;
                        }
                    }
                    else if(valid_space(y-1,rand_mart) && valid_space(y-2,rand_mart)){
                        if(valid_space(y-1,rand_mart+1) && valid_space(y-2,rand_mart+1)){
                            map[y-1][rand_mart] = 'M';
                            map[y-2][rand_mart] = 'M';
                            map[y-1][rand_mart+1] = 'M';
                            map[y-2][rand_mart+1] = 'M';
                            mart_placed=true;
                        }
                        else if(valid_space(y+1,rand_mart-1) && valid_space(y+2,rand_mart-1)){
                            map[y-1][rand_mart] = 'M';
                            map[y-2][rand_mart] = 'M';
                            map[y-1][rand_mart-1] = 'M';
                            map[y-2][rand_mart-1] = 'M';
                            mart_placed=true;
                        }
                    }
                }
            }
        }

    }
}
 
int main()
{
    srand(time(NULL));

    int i, j;
    for(i = 0; i<21; i++){
        for(j = 0; j<80; j++){
            map[i][j] = CLEARING;
        }
    }
    gen_paths();
    gen_boarder();
    gen_buildings();
    for(i = 0; i<21; i++){
        for(j = 0; j<80; j++){
            printf("%c", map[i][j]);
        }
        printf("\n");
    }

    return 0;
}