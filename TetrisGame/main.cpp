#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace sf;

int forms[7][4] = { {1,3,5,7}, {7,4,6,8}, {3,5,7,8}, {5,7,6,8}, {3,5,6,8}, {5,4,6,8}, {5,7,4,6} };


int l[4], c[4],linit[4], cinit[4];
const int n = 18; // nr of c
const int m = 20; //nr of l
int matrix[m][n];

bool check_collision() {
    for (int i = 0; i < 4; i++) {
        if (c[i] < 0 || c[i] >= n || l[i] < 0 || l[i] >= m) return true;
        else if (matrix[l[i]][c[i]] !=0 ) return true;
    }
    return false;
}

int main()
{
    int move = 0, first = 1, color = 0, nform = 0;
    bool rotate = 0;

    srand(time(0));

    RenderWindow window(sf::VideoMode(576, 800), "SFML works!");

    Texture texture;
    if (!texture.loadFromFile("tetris_resurse/squares2.png")) {
        return EXIT_FAILURE;
    }

    Sprite sprite;
    sprite.setTexture(texture);
    //IntRect textureRect(0, 0, 32,32); // Left, Top, Width, Height
    //IntRect textureRect(30*5, 0, 32, 32);
    //sprite.setTextureRect(textureRect);
    sprite.setPosition(100, 100);

    Clock clock;
    float delay = 0.3, timer = 0;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        sf::Event event;
        int no = 0;
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
        // init
        for (int i = 0; i < 4; i++) {
            linit[i] = l[i];
            cinit[i] = c[i];
        }

        //move
        for (int i = 0; i < 4; i++)
            c[i] += move;

        //rotate - the pivot will be forms[x][1]
        if (rotate == 1) {
            for (int i = 0; i < 4; i++) {
                int x = l[i] - l[1];
                int y = c[i] - c[1];
                l[i] = l[1] + y;
                c[i] = c[1] - x;
            }
        }

        //if collision then back to init
        if (check_collision()) {
            for (int i = 0; i < 4; i++) {
                c[i] = cinit[i];
                l[i] = linit[i];
            }
        }

        /*if (first) {
            nform = rand() % 6;
            color = rand() % 5 + 1;

            for (int i = 0; i < 4; i++) {
                l[i] = (forms[nform][i] - 1) / 2;
                c[i] = (forms[nform][i] - 1) % 2;
            }
        }*/

        if (timer > delay) {
            for (int i = 0; i < 4; i++) {
                linit[i] = l[i];
                cinit[i] = c[i];
                l[i]++;
            }

            if (check_collision()) {
                for (int i = 0; i < 4; i++)
                    matrix[linit[i]][cinit[i]] = color;

                nform = rand() % 6;
                color = rand() % 5 + 1;

                for (int i = 0; i < 4; i++) {
                    l[i] = (forms[nform][i] - 1) / 2;
                    c[i] = (forms[nform][i] - 1) % 2;
                }
            }
            timer = 0;
        }

        if (first) {
            nform = rand() % 6;
            color = rand() % 5 + 1;

            for (int i = 0; i < 4; i++) {
                l[i] = (forms[nform][i] - 1) / 2;
                c[i] = (forms[nform][i] - 1) % 2;
            }
        }

        move = 0;
        rotate = 0;
        first = 0;

        window.clear(Color::White);

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] != 0) {
                    sprite.setTextureRect(IntRect(30 * matrix[i][j], 0, 32, 32));
                    sprite.setPosition(j * 32, i * 32);
                    window.draw(sprite);
                }
            }
        }

        for (int i = 0; i < 4; i++) {
            sprite.setTextureRect(IntRect(30 * color, 0, 32, 32));
            sprite.setPosition(c[i] * 32, l[i] * 32);
            window.draw(sprite);
        }
        window.display();
    }

    return 0;
}