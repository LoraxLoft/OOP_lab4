#ifndef SERVER_FIELD_H
#define SERVER_FIELD_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

using namespace sf;

class Field{
public:
    std::vector<RectangleShape> field;
    std::vector<int> nums;
    Color color = Color(204, 142, 255);
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

#endif //SERVER_FIELD_H
