#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include <cmath>
#include <cstring>
#include <ostream>
#include <iostream>
#include <fstream>

using namespace sf;

class Field{
public:
    std::vector<RectangleShape> field;
    std::vector<int> nums;
    Color color = Color::Magenta;
    Color color2 = Color::Black;
    float size = 100;
    float thickness = 4;
    std::vector<int> last_added;

    Field(){
        for(int i=0; i<=2; i++){
            for(int j=0; j<=2; j++){
                RectangleShape rect = RectangleShape(Vector2f(size, size));
                rect.move(size/2 + size*j, size/2 + size*i);
                rect.setFillColor(color);
                rect.setOutlineThickness(thickness);
                rect.setOutlineColor(color2);
                field.push_back(rect);
                nums.push_back(0);
            }
        }
        last_added.push_back(0);
        last_added.push_back(0);
    }
};

class Game{
public:
    std::vector<std::string> states = {"Main_Menu", "Settings", "Round", "View_Res"};
    Field field;
    std::string current_state;
    std::string playing_for = "O";
    std::string playing_against = "X";
    float size = 100;
    Color play_but_col = Color::Green;
    Color play_but_out_col = Color::Black;
    float thickness = 4;
    RectangleShape play_button;
    bool on=false, sets=false;
    bool my_turn = false;
    bool res = 0;


    Game(){
        current_state = "Main_Menu";
    }

    void main_menu(){
        play_button = RectangleShape(Vector2f(size*2, size));
        play_button.move(size, size/2);
        play_button.setFillColor(play_but_col);
        play_button.setOutlineColor(play_but_out_col);
        play_button.setOutlineThickness(thickness);
    }

    int main_menu_check(float x, float y){
        if (play_button.getGlobalBounds().contains(x, y)) return 1;
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

        Text text; Font font;
        if (!font.loadFromFile("C:/WINDOWS/FONTS/CENTAUR.TTF")){
            throw "panic.";
        }
        text.setFont(font);
        text.setString("Play"); text.setCharacterSize(36);
        text.setFillColor(play_but_out_col); text.move(size * 1.8, size * 0.8);
        win.draw(text);
    }

    void draw_round(RenderWindow& win){
        for(RectangleShape rect: field.field){
            win.draw(rect);
        }
        Font font;
        if (!font.loadFromFile("C:/WINDOWS/FONTS/CENTAUR.TTF")){
            throw "panic.";
        }

        for(int i=0; i<=2; i++){
            for(int j=0; j<=2; j++){
                if (field.nums[i*3 + j] == 2){
                    Text text; text.setFont(font); text.setString(playing_for); text.setCharacterSize(36);
                    text.setFillColor(play_but_out_col); text.move(size/2*1.7 + size*j, size/2*1.5 + size*i);
                    win.draw(text);
                } else if (field.nums[i*3 + j] == 1){
                    Text text; text.setFont(font); text.setString(playing_against); text.setCharacterSize(36);
                    text.setFillColor(play_but_out_col); text.move(size/2*1.7 + size*j, size/2*1.5 + size*i);
                    win.draw(text);
                }
            }
        }
    }

    void draw_result(RenderWindow& win){
        Text text;
        Font font;
        if (!font.loadFromFile("C:/WINDOWS/FONTS/CENTAUR.TTF")){
            throw "panic.";
        }
        text.setFont(font); text.setCharacterSize(60);
        text.setFillColor(play_but_out_col); text.move(size, size*1.1);
        if(res){
            text.setString("You won!");
        } else {

            text.setString("You lost.");
        }
        win.draw(text);
    }
};


int main()
{
    int port = 25123;

    sf::TcpListener listener ;

    sf::Socket::Status status = listener.listen(port);
    if (status != sf::Socket::Done) {
        std::cout << "Listen failed!";
        throw "AAAAA!";
    }

    sf::TcpSocket socket;

    std::cout << "Waiting for connection..."<<std::endl;
    status = listener.accept(socket);
    if (status != sf::Socket::Done) {
        std::cout << "Accept failed!";
        throw "AAAAA!";
    }

    sf::ContextSettings settings;
    settings.antialiasingLevel = 9;

////////////////////////////////

    sf::RenderWindow window(sf::VideoMode(400, 400), "Cross&Null_s", 7, settings);
    Game game;
    game.main_menu();

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if(event.type == sf::Event::MouseButtonPressed) {
                Packet data;
                data << game.current_state;
                if (game.current_state == "Main_Menu"){
                    int command = game.main_menu_check(event.mouseButton.x, event.mouseButton.y);
                    if (command == 1){
                        game.on = true;
                        game.current_state = "Round";
                    } else if (command == 2){
                        game.sets = true;
                    }
                    data << game.on << game.sets;
                }  else if (game.current_state == "Round"){
                    if (game.my_turn){
                        game.my_turn = false;
                        data<< true;
                        bool stabbed = game.round_check(event.mouseButton.x, event.mouseButton.y);
                        data<<stabbed;
                        if (stabbed){
                            game.res = game.win_check();
                            data<< game.field.last_added[0] << game.field.last_added[1] << game.res;
                            if(game.res){
                                game.current_state = "View_Res";
                            }
                        }
                    } else {
                        data<< false;
                    }
                } else if (game.current_state == "View_Res"){
                    data<<true;
                    game.current_state = "Main_Menu";
                }
                status = socket.send(data);
                if (status != sf::Socket::Done) {
                }
            }
        }

        sf::SocketSelector selector;
        selector.add(socket);

        if(selector.wait(sf::milliseconds(250)))
        {
            sf::Packet receivedData;
            status = socket.receive(receivedData);
            if (status != sf::Socket::Done && status != sf::Socket::Disconnected) {
                throw "AAAAA!";
            }
            receivedData >> game.current_state;
            if (game.current_state == "Main_Menu"){
                receivedData >> game.on >> game.sets;
                if(game.on){
                    game.current_state = "Round";
                }
            } else if (game.current_state == "Round"){
                bool could_go;
                receivedData >> could_go;
                if(could_go){
                    game.my_turn = true;
                    bool stabbed;
                    receivedData >> stabbed;
                    if (stabbed) {
                        int i, j;
                        receivedData >> i>>j;
                        game.add_num(game.playing_against, i, j);
                        bool lost;
                        receivedData >> lost;
                        if(lost){
                            game.current_state = "View_Res";
                        }
                    }
                }
            } else if (game.current_state == "View_Res"){
                game.current_state = "Main_Menu";
            }
        }

        window.clear(Color::Cyan);
        if (game.current_state=="Main_Menu"){
            game.draw_main_menu(window);
        } else if (game.current_state == "Round"){
            game.draw_round(window);
        } else if (game.current_state == "View_Res"){
            game.draw_result(window);
        }
        window.display();
    }

    socket.disconnect();

    listener.close();

    return 0;
}