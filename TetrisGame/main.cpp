#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace sf;

int forms[7][4] = { {0,2,4,6}, {4,1,3,5}, {0,2,4,5}, {0,2,1,3}, {0,2,3,5}, {2,1,3,5}, {2,4,1,3} };

int l[4], c[4],linit[4], cinit[4]; // l ~ y/height   c ~ x/width
const int n = 18; //nr of columns
const int m = 20; //nr of lines
int matrix[m][n];
int points;

bool check_collision() {
    for (int i = 0; i < 4; i++) {
        if (c[i] < 0 || c[i] >= n || l[i] < 0 || l[i] >= m) return true;
        else if (matrix[l[i]][c[i]] !=-1 ) return true;
    }
    return false;
}

bool game_over() {
    for (int i = 0; i < n; i++) 
        if (matrix[0][i] != -1) return true;
        
    return false;
}

void init_game() {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = -1;
        }
    }

    points = 0;
}

enum app_state {

    playing,
    over
};

int main()
{
    int move = 0, first = 1, color = 0, nform = 0;
    bool rotate = 0;

    srand(time(0));

    int window_w = (n + 2) * 32;
    int window_h = 800;
    RenderWindow window(sf::VideoMode(window_w, window_h), "TETRIS");

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

    RectangleShape boarder;
    boarder.setSize(Vector2f(n*32-6, m*32-6));
    boarder.setPosition(3+32, 3+32);
    boarder.setFillColor(Color::White);
    boarder.setOutlineThickness(3);
    boarder.setOutlineColor(Color::Black);

    Clock clock;
    float delay = 0.3, timer = 0;

    Font font;
    if (!font.loadFromFile("tetris_fonts/arial.ttf")) { 
        return 1;
    }
    Text score;
    points = 0;
    score.setFont(font);
    score.setString("SCORE: "+ to_string(points));
    score.setCharacterSize(20);
    score.setFillColor(Color::Black);
    score.setPosition((n/2)*32,(m+2)*32);

    init_game();

    Text gameOver;
    gameOver.setFont(font);
    gameOver.setString("Game Over!\nScore: " + to_string(points));
    gameOver.setCharacterSize(50);
    gameOver.setFillColor(Color::Black);

    FloatRect textBounds = gameOver.getLocalBounds();
    gameOver.setOrigin(textBounds.left + textBounds.width / 2.0f,
        textBounds.top + textBounds.height / 2.0f);
    gameOver.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 25);

    RectangleShape restart_button;
    restart_button.setSize(Vector2f(100, 100));
    restart_button.setPosition(100, 100);
    restart_button.setFillColor(Color::Green);

    Text buttonText("Restart", font, 24);
    buttonText.setFillColor(sf::Color::White);

    FloatRect buttonTextBounds = buttonText.getLocalBounds();
    buttonText.setOrigin(buttonTextBounds.left + buttonTextBounds.width / 2.0f,
        buttonTextBounds.top + buttonTextBounds.height / 2.0f);
    buttonText.setPosition(restart_button.getPosition().x + restart_button.getSize().x / 2.0f,
        restart_button.getPosition().y + restart_button.getSize().y / 2.0f);

    app_state current_app_state = app_state::playing;

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
                if (event.key.code == Keyboard::Down) delay = 0.05;
            }

            if (current_app_state == app_state::over and event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if (restart_button.getGlobalBounds().contains(mousePos)) {
                        init_game();
                        current_app_state = app_state::playing;

                    }
                }
            }
        }

        if (current_app_state == app_state::over) {

            score.setString("SCORE: 0");
            window.clear(Color::White);
            window.draw(gameOver);
            window.draw(restart_button);
            window.draw(buttonText);
            window.display();
        }
        if (current_app_state == app_state::playing) {

            // init
            for (int i = 0; i < 4; i++) {
                linit[i] = l[i];
                cinit[i] = c[i];
            }

            //move
            for (int i = 0; i < 4; i++)
                c[i] += move;

            //rotate 
            //the pivot will be forms[x][1] when the form is not at the left edge
            //the pivot will be forms[x][3] when the form is at the left edge
            if (rotate == 1) {
                if (c[0] != 0) {
                    for (int i = 0; i < 4; i++) {
                        int x = l[i] - l[1];
                        int y = c[i] - c[1];
                        l[i] = l[1] + y;
                        c[i] = c[1] - x;
                    }
                }
                else {
                    for (int i = 0; i < 4; i++) {
                        int x = l[i] - l[3];
                        int y = c[i] - c[3];
                        l[i] = l[3] + y;
                        c[i] = c[3] - x;
                    }
                }
            }

            //if collision then back to init
            if (check_collision()) {
                for (int i = 0; i < 4; i++) {
                    c[i] = cinit[i];
                    l[i] = linit[i];
                }
            }

            if (timer > delay) {
                for (int i = 0; i < 4; i++) {
                    linit[i] = l[i];
                    cinit[i] = c[i];
                    l[i]++; //move dowm
                }

                if (check_collision()) {
                    for (int i = 0; i < 4; i++)
                        matrix[linit[i]][cinit[i]] = color;

                    nform = rand() % 6;
                    color = (color + 1) % 5;

                    for (int i = 0; i < 4; i++) {
                        l[i] = forms[nform][i] / 2;
                        c[i] = forms[nform][i] % 2;
                    }
                }
                timer = 0;
            }

            if (first) {
                nform = rand() % 6;

                for (int i = 0; i < 4; i++) {
                    l[i] = forms[nform][i] / 2;
                    c[i] = forms[nform][i] % 2;
                }
            }

            //erase lines
            int i = m - 1;
            while (i > 0) {
                int cnt = 0;
                for (int j = 0; j < n; j++) {
                    if (matrix[i][j] != -1)
                        cnt++;
                }
                if (cnt == n) {
                    for (int a = i - 1; a > 0; a--) {
                        for (int b = 0; b < n; b++) {
                            matrix[a + 1][b] = matrix[a][b];
                        }
                    }
                    i = m - 1;
                    points++;
                    score.setString("SCORE: " + to_string(points));
                }
                else i--;
            }

            move = 0;
            rotate = 0;
            first = 0;
            delay = 0.3;

            window.clear(Color::White);
            window.draw(boarder);
            window.draw(score);

            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    if (matrix[i][j] != -1) {
                        sprite.setTextureRect(IntRect(30 * matrix[i][j], 0, 32, 32));
                        sprite.setPosition(j * 32 + 32, i * 32 + 32);
                        window.draw(sprite);
                    }
                }
            }

            for (int i = 0; i < 4; i++) {
                sprite.setTextureRect(IntRect(30 * color, 0, 32, 32));
                sprite.setPosition(c[i] * 32 + 32, l[i] * 32 + 32);
                window.draw(sprite);
            }
            window.display();

            if (game_over()) {
                gameOver.setString("Game Over!\nScore: " + to_string(points));
                current_app_state = app_state::over;
            }
        }
    }

    return 0;
}