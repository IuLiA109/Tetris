#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace sf;

int forms[7][4] = { {1,3,5,7}, {7,4,6,8}, {3,5,7,8}, {5,7,6,8}, {3,5,6,8}, {5,4,6,8}, {5,7,4,6} };

int l[4], c[4];

int main()
{
    int move=0;
    bool rotate=0;
    RenderWindow window(sf::VideoMode(576,800), "SFML works!");
    
    Texture texture;
    if (!texture.loadFromFile("tetris_resurse/squares2.png")) {
        return EXIT_FAILURE;
    }

    Sprite sprite;
    sprite.setTexture(texture);
    IntRect textureRect(0, 0, 32,32); // Left, Top, Width, Height
    sprite.setTextureRect(textureRect);
    sprite.setPosition(100, 100);

    while (window.isOpen())
    {
        sf::Event event;
        int no= 0;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Right) move = 1;
                if (event.key.code == Keyboard::Left) move = -1;
                if (event.key.code == Keyboard::Up) rotate = 1;
            }
        }

        //move
        for (int i = 0; i < 4; i++) c[i] += move;

        //rotate - the pivot will be forms[x][1]
        if (rotate == 1) {
            for (int i = 0; i < 4; i++) {
                int x = l[i] - l[1];
                int y = c[i] - c[1];
                l[i] = l[1] + y;
                c[i] = c[1] - x;
            }
        }

        int n = 6;
        if(c[0]==0 || c[3] >= 18)
        for (int i = 0; i < 4; i++) {
            l[i] = (forms[n][i]-1) / 2;
            c[i] = (forms[n][i]-1) % 2;
        }
        if (c[0] < 0 && c[3]==0)
            for (int i = 0; i < 4; i++) {
                l[i] = (forms[n][i] - 1) / 2;
                c[i] = (forms[n][i] - 1) % 2+16;
            }
        if (c[0] < 0 && c[3] < 0)
            for (int i = 0; i < 4; i++) {
                l[i] = (forms[n][i] - 1) / 2;
                c[i] = (forms[n][i] - 1) % 2 + 17;
            }


        move = 0;
        rotate = 0;

        window.clear(Color::White);

        for (int i = 0; i < 4; i++) {
            sprite.setPosition(c[i]*32, l[i]*32);
            window.draw(sprite);
        }
        window.display();
    }

    return 0;
}