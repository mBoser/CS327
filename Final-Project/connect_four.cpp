#include <iostream>
#include <cstdlib>
#include <sys/time.h>
#include <string>
#include <unistd.h>

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

    mvprintw(3,0, "1) Very Easy (Timmy)");
    mvprintw(4,0, "2) Slightly more difficult (Steve)");
    mvprintw(5,0, "3) Hardest (Giga Chad)");
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

    if(difficulty == 1){
        mvprintw(0, 0, "Welcome to Timmy's playground!");
        mvprintw(1, 0, "Take it easy on him");
    } else if(difficulty == 2){
        mvprintw(0, 0, "Welcome to Steve's office!");
        mvprintw(1, 0, "Should be a good match!");
    } else{
        mvprintw(0, 0, "How dare you challenge Giga Chad!");
        mvprintw(1, 0, "Prepare to get MOPPED bozo!");
    }
    mvprintw(5,0, "Use Side arrows to move left and right");
    mvprintw(6,0, "Use Space to place your token!");
    mvprintw(21,0, "Use 'Q' to quit any time...");
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


bool check_win(int player, int x, int y){
    std:: string url = "http://kevinalbs.com/connect4/back-end/index.php/hasWon?board_data=";
    url += get_board_info();
    url += "&player=";
    url += std::to_string(player);
    url += "&i=";
    url += std::to_string(y);
    url += "&j=";
    url += std::to_string(x);

    try
    {
        http::Request request{url};
        const auto response = request.send("GET");
        std::string str = {response.body.begin(), response.body.end()};
        if(str.compare("true") == 0){
            display_board();
            mvprintw(2, BOARD_X_OFFSET+1, "Game is over!");
            if(player == 1){
                if(difficulty == 1){
                    mvprintw(3, BOARD_X_OFFSET-10, "You should not feel good about this one...");
                } else if(difficulty == 2){
                    mvprintw(3, BOARD_X_OFFSET-13, "Good win, Steve is still working on his skills!");
                } else{
                    mvprintw(3, BOARD_X_OFFSET-3, "IMPRESSIVE! nobody has beaten him yet!");
                }
            } else{
                if(difficulty == 1){
                    mvprintw(3, BOARD_X_OFFSET-6, "How did you lose to Timmy...");
                } else if(difficulty == 2){
                    mvprintw(3, BOARD_X_OFFSET-3, "Steve has bested you!");
                } else{
                    mvprintw(3, BOARD_X_OFFSET-10, "I tried to warn you of Chad's power...");
                }
            }
            
            refresh();
            getch();
            end_game = true;
            return true;
        }else{
            return false;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Request failed, error: " << e.what() << '\n';
    }

    return false;
}

bool place_chip(int player, int column){
    int free_space = num_free(column);
    if(free_space > 0){
        board[free_space-1][column] = player;
        if(check_win(player, column, free_space-1)){
            end_game = true;
        }
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


void do_opponent_move(int player){
    std::string url = "http://kevinalbs.com/connect4/back-end/index.php/getMoves?board_data=";
    url += get_board_info();
    url += "&player=";
    url += std::to_string(player);
    int i;
    int moves[7];
    for(i = 0; i < 7; i++){
        moves[i] = -9999;
    }
   
    int num_moves = 0;

    try
    {
        http::Request request{url};
        const auto response = request.send("GET");
        std::string str = {response.body.begin(), response.body.end()};
        auto j = json::parse(str);
        i = 0;
        for (json::iterator it = j.begin(); it != j.end(); ++it) {
            moves[stoi(it.key())] = it.value();
            num_moves++;
            //std::cout << it.value() << "\n";
        }
        
        //getch();
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
            if(place_chip(player, move)){
                moved = true;
            }
        }

    } else if(difficulty == 2){
        //This is Steve, Steve is your average joe, he knows the concept, but will probably not pick the correct move every time...

        int best_move = -9999;
        int best_move_index = -1;
        int second_best_move_index = -1;
        int second_best_move = - -1000;
        int third_best_move_index = -1;
        int third_best_move = -1000;


        for(i = 0; i < 7; i++){
            if(moves[i] >= best_move){
                third_best_move_index = second_best_move_index;
                second_best_move_index = best_move_index;
                best_move = moves[i];
                best_move_index = i;
            } else if(moves[i] >= second_best_move){
                third_best_move = second_best_move;
                third_best_move_index = second_best_move_index;
                second_best_move = moves[i];
                second_best_move_index = i;
            } else if(moves[i] > third_best_move){
                third_best_move = moves[i];
                third_best_move_index = i;
            }
        }
        bool moved = false;
        while(!moved){
            if(num_moves == 1){
                if(place_chip(player, best_move_index)){
                    moved = true;
                }
            } else if(num_moves == 2){
                if(rand() % 100 < 40){
                    if(place_chip(player, best_move_index)){
                        moved = true;
                    }
                } else{
                    if(second_best_move_index >= 0){
                        if (place_chip(player, second_best_move_index)){
                            moved = true;
                        }
                    } else{
                        if(place_chip(player, best_move_index)){
                            moved = true;
                        }
                    }
                }
            } else{
                if(rand() % 100 < 25){
                    if (place_chip(player, best_move_index)){
                        moved = true;
                    }
                } else if(rand() % 75 < 40){
                    if(second_best_move_index >= 0){
                        if (place_chip(player, second_best_move_index)){
                            moved = true;
                        }
                    } else{
                        if (place_chip(player, best_move_index)){
                            moved = true;
                        }
                    }
                } else{
                    if(third_best_move_index >= 0){
                        if (place_chip(player, third_best_move_index)){
                            moved = true;
                        }
                    } else if(second_best_move_index >= 0){
                        if (place_chip(player, second_best_move_index)){
                            moved = true;
                        }
                    } else{
                        if (place_chip(player, best_move_index)){
                            moved = true;
                        }
                    }
                }

            }
        }

    } else if(difficulty == 3){
        //This is Giga Chad, Giga Chad will correctly pick the mathematically optimal move 99% of the time...
        int best_move = -9999;
        int best_move_index = 0;
        for(i = 0; i<7; i++){
            if(moves[i] > best_move){
                best_move = moves[i];
                best_move_index = i;
            }
        }
        // mvprintw(2, 70, "1: %d", best_move_index);
        // refresh();
        // getch();
        place_chip(player, best_move_index);
    }  else{
        //This is not good, just get rid of the game
        end_game = true;
    }

}

void game_loop(bool user_priority, bool ai_battle){
    while(!end_game){
        display_board();
        if(ai_battle){
            if(user_priority){
                sleep(.5);
                do_opponent_move(1);
                if(end_game){
                    break;
                }
                display_board();
                sleep(.5);
                do_opponent_move(2);
            } else{
                sleep(1);
                do_opponent_move(2);
                if(end_game){
                    break;
                }
                display_board();
                sleep(1);
                do_opponent_move(1);
            }
        } else if(user_priority){
            do_user_move();
            if(end_game){
                break;
            }
            display_board();
            do_opponent_move(2);
        } else{
            do_opponent_move(2);
            if(end_game){
                break;
            }
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
        game_loop(user_priority, false);
    }
    io_reset_terminal();
        
    return 0;
}