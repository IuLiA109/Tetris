#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace sf;

int forms[7][4] = { {0,2,4,6}, {4,1,3,5}, {0,2,4,5}, {0,2,1,3}, {0,2,3,5}, {2,1,3,5}, {2,4,1,3} };

enum app_state {
    playing,
    over
};

int l[4], c[4],linit[4], cinit[4]; 
// l ~ y/height   c ~ x/width
const int n = 10; //nr of columns
const int m = 18; //nr of lines
int matrix[m][n];
int points;

//-------------------HELPFUL FUNCTIONS--------------------------
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
//-------------------HELPFUL FUNCTIONS--------------------------

int main()
{
    int move = 0, first = 1, color = 0, nform = 0, nextform = 0;
    bool rotate = 0;

    srand(time(0));

    int window_w = (n * 2) * 32;
    int window_h = (m * 5 / 4) * 32;

    RenderWindow window(sf::VideoMode(window_w, window_h), "TETRIS");

    //-------------------TEXTURES AND FONTS--------------------------
    Texture texture;
    if (!texture.loadFromFile("tetris_resurse/squares2.png")) {
        return EXIT_FAILURE;
    }

    Texture white_block;
    if (!white_block.loadFromFile("tetris_resurse/nextshape.png")) {
        return EXIT_FAILURE;
    }

    Font font;
    if (!font.loadFromFile("tetris_fonts/arial.ttf")) {
        return 1;
    }
    //-------------------TEXTURES AND FONTS--------------------------


    //----------------------SPRITES-------------------------------
    Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(100, 100);

    Sprite white_sprite;
    white_sprite.setTexture(white_block);
    white_sprite.setTextureRect(IntRect(1, 1, 38, 38));
    //----------------------SPRITES-------------------------------


    //----------------------BORDER-------------------------------
    RectangleShape border;
    border.setSize(Vector2f(n*32-6, m*32-6));
    border.setPosition(3+32, 3+32);
    border.setFillColor(Color::White);
    border.setOutlineThickness(3);
    border.setOutlineColor(Color::Black);
    //----------------------BORDER-------------------------------

    Clock clock;
    float delay = 0.3, timer = 0;

    //----------------------SCORE-------------------------------
    Text score;
    points = 0;
    score.setFont(font);
    score.setString("SCORE: "+ to_string(points));
    score.setCharacterSize(20);
    score.setFillColor(Color::Black);
    score.setPosition((n * 3 / 2) * 32 + 16 , (m / 5) * 32 + 32);
    FloatRect scoreBounds = score.getLocalBounds();
    score.setOrigin(scoreBounds.left + scoreBounds.width / 2, scoreBounds.top + scoreBounds.height / 2);
    //----------------------SCORE-------------------------------


    //----------------------NEXT SHAPE-------------------------------
    Text next_form;
    next_form.setFont(font);
    next_form.setString("NEXT SHAPE:");
    next_form.setCharacterSize(20);
    next_form.setFillColor(Color::Black);
    next_form.setPosition((n * 3 / 2) * 32 + 16, (m / 3) * 32);
    FloatRect next_formBounds = next_form.getLocalBounds();
    next_form.setOrigin(next_formBounds.left + next_formBounds.width / 2, next_formBounds.top + next_formBounds.height / 2);
    //----------------------NEXT SHAPE-------------------------------

    init_game();

    //----------------------GAME OVER-------------------------------
    Text gameOver;
    gameOver.setFont(font);
    gameOver.setString("Game Over!\nScore: " + to_string(points));
    gameOver.setCharacterSize(50);
    gameOver.setFillColor(Color::Black);

    FloatRect textBounds = gameOver.getLocalBounds();
    gameOver.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
    gameOver.setPosition(window.getSize().x / 2, window.getSize().y / 2 - 50);
    //----------------------GAME OVER-------------------------------


    //----------------------RESTART-------------------------------
    RectangleShape restart_button;
    restart_button.setSize(Vector2f(120, 65));
    restart_button.setPosition(window.getSize().x / 2, window.getSize().y / 2 + window_h * 7 / 100);
    restart_button.setFillColor(Color::White);
    restart_button.setOutlineThickness(2);
    restart_button.setOutlineColor(Color::Black);
    Vector2f restart_button_size = restart_button.getSize();
    restart_button.setOrigin(restart_button_size.x / 2, restart_button_size.y / 2);

    Text buttonText("Restart", font, 27);
    buttonText.setFillColor(Color::Black);

    FloatRect buttonTextBounds = buttonText.getLocalBounds();
    buttonText.setOrigin(buttonTextBounds.left + buttonTextBounds.width / 2, buttonTextBounds.top + buttonTextBounds.height / 2);
    buttonText.setPosition(restart_button.getPosition().x, restart_button.getPosition().y);
    //----------------------RESTART-------------------------------

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

            //ARROWS
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Right) move = 1;
                if (event.key.code == Keyboard::Left) move = -1;
                if (event.key.code == Keyboard::Up) rotate = 1;
                if (event.key.code == Keyboard::Down) delay = 0.05;
            }

            //RESTART BUTTON
            if (current_app_state == app_state::over and event.type == Event::MouseButtonReleased) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                    if (restart_button.getGlobalBounds().contains(mousePos)) {
                        init_game();
                        current_app_state = app_state::playing;

                    }
                }
            }
        }

        //GAME OVER
        if (current_app_state == app_state::over) {

            score.setString("SCORE: 0");
            window.clear(Color::White);
            window.draw(gameOver);
            window.draw(restart_button);
            window.draw(buttonText);
            window.display();
        }

        //PLAYING 
        if (current_app_state == app_state::playing) {

            //save init values
            for (int i = 0; i < 4; i++) {
                linit[i] = l[i];
                cinit[i] = c[i];
            }

            //move left right
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
            //no movement if collision
            if (check_collision()) {
                for (int i = 0; i < 4; i++) {
                    c[i] = cinit[i];
                    l[i] = linit[i];
                }
            }

            if (timer > delay) {
                //move dowm and save init values
                for (int i = 0; i < 4; i++) {
                    linit[i] = l[i];
                    cinit[i] = c[i];
                    l[i]++; 
                }

                if (check_collision()) {
                    for (int i = 0; i < 4; i++)
                        matrix[linit[i]][cinit[i]] = color;

                    nform = nextform;
                    nextform = rand() % 6;
                    color = (color + 1) % 5;

                    for (int i = 0; i < 4; i++) {
                        l[i] = forms[nform][i] / 2;
                        c[i] = forms[nform][i] % 2;
                    }
                }
                timer = 0;
            }

            //first form
            if (first) {
                nform = rand() % 6;
                nextform = rand() % 6;

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

            //test game over
            if (game_over()) {
                gameOver.setString("Game Over!\nScore: " + to_string(points));
                current_app_state = app_state::over;
            }

            //----------------------DRAW-------------------------------
            window.clear(Color::White);
            window.draw(border);
            window.draw(score);
            window.draw(next_form);

            //fixed forms
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    if (matrix[i][j] != -1) {
                        sprite.setTextureRect(IntRect(30 * matrix[i][j], 0, 32, 32));
                        sprite.setPosition(j * 32 + 32, i * 32 + 32);
                        window.draw(sprite);
                    }
                }
            }

            //moving form
            for (int i = 0; i < 4; i++) {
                sprite.setTextureRect(IntRect(30 * color, 0, 32, 32));
                sprite.setPosition(c[i] * 32 + 32, l[i] * 32 + 32);
                window.draw(sprite);
            }

            //next form
            for (int i = 0; i < 4; i++) {
                white_sprite.setPosition(forms[nextform][i] % 2 * 36 + (n * 3 / 2 * 32), forms[nextform][i] / 2 * 36 + (m / 3 * 32) + 32);
                window.draw(white_sprite);
            }

            window.display();
            //----------------------DRAW-------------------------------
        }
    }

    return 0;
}