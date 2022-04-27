#include <iostream>
#include <cstdlib>
#include <sys/time.h>
#include <string>

#include <ncurses.h>
#include "HTTPRequest.hpp"
#include "json.hpp"

// for convenience
using json = nlohmann::json;


#define X_MAX 15
#define Y_MAX 7
#define BOARD_X_OFFSET 40

/*
012345678901234
| | | | | | | |
| | | | | | | |
| | | | | | | |
| | | | | | | |
| | | | | | | |
| | | | | | | |
| | | | | | | |

*/

int board[7][7];
int cursor;
int difficulty;
bool end_game = false;

void io_reset_terminal(){
  endwin();
}


void init_board(){
    cursor = 3; //Place cursor in the middle of the board;
    int x,y;
    for(y = 0; y<7; y++){
        for(x = 0; x<7; x++){
            board[y][x] = 0;
        }
    }

}

void init_screen(){
    initscr();
    raw();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
}

int get_difficulty(){
    clear();
    
    mvprintw(0,0, "Welcome to Connect 4!");
    mvprintw(1,0, "Please choose your difficulty");

    mvprintw(3,0, "1) Very Easy");
    mvprintw(4,0, "2) Slightly more difficult");
    mvprintw(5,0, "3) Hardest");
    refresh();
    char c;
    bool deciding = true;

    while(deciding){
        c = getch();
            switch (c){
            case '1':
                return 1;
                break;
            case '2':
                return 2;
                break;
            case '3':
                return 3;
                break;
            case 'Q':
                end_game = true;
                deciding = false;
                break;
        }
    }
    return 1;

}

bool get_priority(){
    clear();
    
    mvprintw(0,0, "Now we will decide who goes first!");
    mvprintw(1,0, "Please pick heads or tails");

    mvprintw(3,0, "1) Heads");
    mvprintw(4,0, "2) Tails");
    refresh();
    char c;
    bool deciding = true;

    while(deciding){
        c = getch();
            switch (c){
            case '1':
                if ((rand() % 2) == 0){
                    clear();
                    mvprintw(0,0, "Congrats you won! You will go first");
                    mvprintw(20,0, "Click any key to continue...");
                    refresh();
                    getch();
                    return true;
                } else{
                    clear();
                    mvprintw(0,0, "Im sorry it was tails, the opponent will go first");
                    mvprintw(20,0, "Click any key to continue...");
                    refresh();
                    getch();
                    return false;
                }
                break;
            case '2':
                if ((rand() % 2) == 1){
                    clear();
                    mvprintw(0,0, "Congrats you won! You will go first");
                    mvprintw(20,0, "Click any key to continue...");
                    refresh();
                    getch();
                    return true;
                } else{
                    clear();
                    mvprintw(0,0, "Im sorry it was heads, the opponent will go first");
                    mvprintw(20,0, "Click any key to continue...");
                    refresh();
                    getch();
                    return false;
                }
                break;
            case 'Q':
                end_game = true;
                deciding = false;
                break;
        }
    }
    return false;

}

std::string get_board_info(){
    std::string to_ret = "";
    int x, y;
    for(y = 0; y < 7; y++){
        for(x = 0; x < 7; x++){
            to_ret+= std::to_string(board[y][x]);
        }
    }
    return to_ret;
}

void display_board(){
    int x,y;
    int board_x = 0;

    clear();
    for(y = 0; y<Y_MAX; y++){
        for(x = 0; x<X_MAX; x++){
            if(x%2 == 1){
                switch (board[y][board_x]){
                    case 0:
                        mvaddch(y + 5, x + BOARD_X_OFFSET, ' ');
                        break;
                    case 1:
                        attron(COLOR_PAIR(COLOR_BLUE));
                        mvaddch(y + 5, x + BOARD_X_OFFSET, '0');
                        attroff(COLOR_PAIR(COLOR_BLUE));
                        break;
                    case 2:
                        attron(COLOR_PAIR(COLOR_RED));
                        mvaddch(y + 5, x + BOARD_X_OFFSET, '0');
                        attroff(COLOR_PAIR(COLOR_RED));
                        break;
                    default:
                        mvaddch(y + 5, x + BOARD_X_OFFSET, '?');
                        break;
                }
                board_x++;
            }else{
                attron(COLOR_PAIR(COLOR_WHITE));
                mvaddch(y + 5, x + BOARD_X_OFFSET, '|');
                attroff(COLOR_PAIR(COLOR_WHITE));
            }


        }
        board_x = 0;
    }

    refresh();
}

void move_right(){
    if(cursor < 6){
        mvaddch(4,BOARD_X_OFFSET + (cursor * 2) + 1, ' ');
        cursor++;
    }
    return;
}

void move_left(){
    if(cursor > 0){
        mvaddch(4,BOARD_X_OFFSET + (cursor * 2) + 1, ' ');
        cursor--;
    }
    return;
}

void display_cursor(){
    mvaddch(4,BOARD_X_OFFSET + (cursor * 2) + 1, 'V');
    refresh();
}

int num_free(int x){
    int y;
    int count = 0;
    for(y = 0; y<7; y++){
        if(board[y][x] == 0){
            count++;
        }
    }
    return count;
}

bool place_chip(int player, int column){
    int free_space = num_free(column);
    if(free_space > 0){
        board[free_space-1][column] = player;
        return true;
    } else{
        return false;
    }
    return false;
}

void do_user_move(){
    bool deciding = true;
    int c;
    while(deciding){
        display_cursor();
        c = getch();
        if(c == KEY_LEFT){
            move_left();
        } else if(c == KEY_RIGHT){
            move_right();
        }else if(c == ' '){
            if(place_chip(1, cursor)){
                deciding = false;
            }
        } else if(c == 'Q'){
            deciding = false;
            end_game = true;
        }
    }
}

void do_opponent_move(){
    std::string url = "http://kevinalbs.com/connect4/back-end/index.php/getMoves?board_data=";
    url += get_board_info();
    url += "&player=2";

    int moves[7] = {0};
    int i;

    try
    {
        http::Request request{url};
        const auto response = request.send("GET");
        std::string str = {response.body.begin(), response.body.end()};
        auto j = json::parse(str);
        i = 0;
        for (json::iterator it = j.begin(); it != j.end(); ++it) {
            moves[i] = *it;
            i++;
            std::cout << *it << '\n';
        }
        getch();
        //std::cout << j.dump(3) << std::endl; // print the result
    }
    catch (const std::exception& e)
    {
        std::cerr << "Request failed, error: " << e.what() << '\n';
    }

    if(difficulty == 1){
        //This is Timmy, timmy is really bad at connect 4, Timmy arguably does not know the obejctive of the game...
        int move=0;
        bool moved = false;
        while(!moved){
            move = (rand() % 7);
            if(place_chip(2, move)){
                moved = true;
            }
        }

    } else if(difficulty == 2){
        int best_moves[3] = {0};
        for(i = 0; i<7; i++){
            best_moves[1] = moves[i]; //bullshit logic to ignore warnings
        }
        
        bool moved = false;
        while(!moved){
            int move = rand() % 3;
            if(place_chip(2, best_moves[move])){
                moved = true;
            }
        }
        //This is Steve, steve is your average joe, he knows the concept, but will probably not pick the correct move every time...

    } else if(difficulty == 3){
        //This is Alpha Chad, Alpha chad will correctly pick the mathematically optimal move 99% of the time...
        int best_move = 0;
        int best_move_index = 0;
        for(i = 0; i<7; i++){
            if(moves[i] > best_move){
                best_move = moves[i];
                best_move_index = i;
            }
        }

        place_chip(2, best_move_index);
    }  else{
        //This is not good, just get rid of the game
        end_game = true;
    }

}

void game_loop(bool user_priority){
    //int c;
    while(!end_game){
        display_board();
        if(user_priority){
            do_user_move();
            display_board();
            do_opponent_move();
        } else{
            do_opponent_move();
            display_board();
            do_user_move();
        }
        refresh();
    }
}



int main(){
    struct timeval tv;
    uint32_t seed;
    gettimeofday(&tv, NULL);
    seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
    srand(seed);

    init_board();

    bool user_priority;
    init_screen();
    difficulty = get_difficulty();
    if(!end_game){
        user_priority = get_priority();
        game_loop(user_priority);
    }
    io_reset_terminal();
    


    
    return 0;
}