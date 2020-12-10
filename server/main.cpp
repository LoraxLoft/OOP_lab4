#include "Game.h"
#include <ostream>
#include <iostream>

float size = 100;

int main()
{
    int port = 25123;

    TcpListener listener ;

    Socket::Status status = listener.listen(port);
    if (status != Socket::Done) {
        std::cout << "Listen failed!";
        throw "AAAAA!";
    }

    TcpSocket socket;

    std::cout << "Waiting for connection..."<<std::endl;
    status = listener.accept(socket);
    if (status != Socket::Done) {
        std::cout << "Accept failed!";
        throw "AAAAA!";
    }

    ContextSettings settings;
    settings.antialiasingLevel = 9;

////////////////////////////////

    RenderWindow window(VideoMode(int(size)*4, int(size)*4), "Cross&Null_s", 7, settings);
    Game game(size);
    game.main_menu();

    while (window.isOpen()){
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            else if(event.type == Event::MouseButtonPressed) {
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
                    game.main_menu();
                }
                status = socket.send(data);
                if (status != Socket::Done) {
                }
            }
        }

        SocketSelector selector;
        selector.add(socket);

        if(selector.wait(milliseconds(250)))
        {
            sf::Packet receivedData;
            status = socket.receive(receivedData);
            if (status != Socket::Done && status != Socket::Disconnected) {
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
                game.main_menu();
            }
        }

        window.clear(Color(130,207, 255));
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