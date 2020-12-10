#ifndef CLIENT_SETTINGS_H
#define CLIENT_SETTINGS_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

using namespace sf;

class Settings{
public:
    Color play_but_col = Color(8, 205, 202);
    Color out_col = Color::Black;
    //Color set_but_col = Color::Yellow;
    float thickness = 4;
    float size;
    Font font;

    Settings(float s=0){
        size = s;
        if (!font.loadFromFile("C:/WINDOWS/FONTS/COOPBL.TTF")){
            throw "panic.";
        }
    }

    void set_play_button(RectangleShape& but){
        but.move(size, size/2);
        but.setFillColor(play_but_col);
        but.setOutlineColor(out_col);
        but.setOutlineThickness(thickness);
    }

    void set_play_but_text(Text& text){
        text.setFont(font);
        text.setString("Play"); text.setCharacterSize(36);
        text.setFillColor(out_col); text.move(size * 1.6, size * 0.8);
    }
/*
    void set_sets_button(RectangleShape& but){
        but.move(size, size);
        but.setFillColor(set_but_col);
        but.setOutlineColor(out_col);
        but.setOutlineThickness(thickness);
    }

    void set_sets_but_text(Text& text){
        text.setFont(font);
        text.setString("Settings"); text.setCharacterSize(36);
        text.setFillColor(out_col); text.move(size * 1.5, size * 0.8* 3);
    }
*/

    void set_game_symb(Text& text, std::string s, int j, int i){
        text.setFont(font); text.setString(s); text.setCharacterSize(36);
        text.setFillColor(out_col); text.move(size / 2 * 1.7 + size * j, size / 2 * 1.5 + size * i);
    }

    void set_result_mess(Text& text, std::string s){
        text.setFont(font); text.setCharacterSize(60);
        text.setFillColor(out_col); text.move(size*0.7, size * 1.4);
        text.setString(s);
    }

};

#endif //CLIENT_SETTINGS_H
