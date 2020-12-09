#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include <cmath>
#include <cstring>
#include <ostream>
#include <iostream>
#include <fstream>


int main()
{
    std::string address("127.0.0.1");
    int port = 25123;

    sf::TcpSocket socket;

    sf::Socket::Status status = socket.connect(address, port);
    if (status != sf::Socket::Done) {
        std::cout << "Connect failed!";
    }

    sf::ContextSettings settings;
    settings.antialiasingLevel = 9;

//////////////////////////////

    sf::RenderWindow window(sf::VideoMode(400, 400), "Cross&Null", 7, settings);

    std::vector<std::vector<int>> field_round;
    std::vector<sf::RectangleShape> field;

    for(int i = 0; i<=2; i++){
        for(int j = 0; j<=2; j++){

        }
    }


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if(event.type == sf::Event::MouseButtonPressed) {
                sf::Packet data;

                data << event.mouseButton.x << event.mouseButton.y;

                status = socket.send(data);
                if (status != sf::Socket::Done) {
                }
            }
        }

        const size_t SIZE = 4096;
        char result[SIZE];

        size_t bytesRead = 0;

        sf::SocketSelector selector;
        selector.add(socket);

        std::string message;

        if(selector.wait(sf::milliseconds(250)))
        {
            sf::Packet receivedData;
            status = socket.receive(receivedData);
            if (status != sf::Socket::Done && status != sf::Socket::Disconnected) {
                throw "AAAAA!";
            }


        }

        window.clear(sf::Color(180,0,50));


        window.display();
    }

    socket.disconnect();

    return 0;
}