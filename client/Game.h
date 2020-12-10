#ifndef CLIENT_GAME_H
#define CLIENT_GAME_H

#include "Field.h"
#include "Settings.h"

class Game{
public:
    //states = {"Main_Menu", "Settings", "Round", "View_Res"};
    Field field;
    std::string current_state;
    std::string playing_for = "X";
    std::string playing_against = "O";
    float size;
    RectangleShape play_button, sets_button;
    bool on=false, sets=false;
    bool my_turn = true;
    bool res = 0;
    Settings settings;

    Game(float s){
        current_state = "Main_Menu";
        size = s;
        settings.size = size;
    }

    void main_menu(){
        play_button = RectangleShape(Vector2f(size*2, size));
        //sets_button = RectangleShape(Vector2f(size*2, size));
        settings.set_play_button(play_button);
        //settings.set_sets_button(sets_button);
        Field field2;
        field = field2;
        my_turn = true;
    }

    int main_menu_check(float x, float y){
        if (play_button.getGlobalBounds().contains(x, y)){return 1;}
        //else if (sets_button.getGlobalBounds().contains(x, y)){return 2;}
        return 0;
    }

    void add_num(std::string s, int i, int j){
        field.last_added[0] = i;
        field.last_added[1] = j;
        if (s == playing_for){
            field.nums[i*3 + j] = 2;
        } else if (s == playing_against){
            field.nums[i*3 + j] = 1;
        } else {
            throw "panic";
        }
    }

    bool round_check(float x, float y){
        for(int i=0; i<=2; i++){
            for(int j=0; j<=2; j++){
                if (field.field[i*3 + j].getGlobalBounds().contains(x, y)){
                    add_num(playing_for, i, j);
                    return true;
                }
            }
        }
        return false;
    }

    bool win_check(){
        for(int i = 0; i <= 2; i++){
            bool win = true;
            for(int j = 0; j <=2; j++){
                if (field.nums[i*3 + j] != 2){
                    win = false;
                    break;
                }
            }
            if (win){return true;}
        }
        for(int i = 0; i <= 2; i++){
            bool win = true;
            for(int j = 0; j <=2; j++){
                if (field.nums[i + j*3] != 2){
                    win = false;
                    break;
                }
            }
            if (win){return true;}
        }
        if(field.nums[0] == 2 && field.nums[4] == 2 && field.nums[8] == 2){return true;}
        if(field.nums[2] == 2 && field.nums[4] == 2 && field.nums[6] == 2){return true;}
        return false;
    }

    void draw_main_menu(RenderWindow& win){
        win.draw(play_button);
        Text text, text2;
        settings.set_play_but_text(text);
        //settings.set_sets_but_text(text2);
        win.draw(text); win.draw(text2);
    }

    void draw_round(RenderWindow& win){
        for(RectangleShape rect: field.field){
            win.draw(rect);
        }

        for(int i=0; i<=2; i++){
            for(int j=0; j<=2; j++){
                Text text;
                if (field.nums[i*3 + j] == 2){
                    settings.set_game_symb(text, playing_for, j, i);
                } else if (field.nums[i*3 + j] == 1){
                    settings.set_game_symb(text, playing_against, j, i);
                }
                win.draw(text);
            }
        }
    }

    void draw_result(RenderWindow& win){
        Text text;
        if(res){
            settings.set_result_mess(text, "You won!");
        } else {
            settings.set_result_mess(text, "You lost.");
        }
        win.draw(text);
    }
};

#endif //CLIENT_GAME_H
