#include <iostream>
#include <Windows.h>
#include <MMSystem.h>
#include <mmsystem.h>
#include <conio.h>
#include <thread>
#include <chrono>

using namespace std;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;
bool gameOver;
const int height = 20;
const int width = 30;
int headX, headY, fruitX, fruitY, specialFruitX, specialFruitY, score;
int tailx[100], taily[100];
int tail_len;
int fruitsEaten;

void setup();
void draw();
void input();
void logic();
void playBackgroundMusic(const char* musicFileName);
void playSound(const char* soundFileName);

void playBackgroundMusic(const char* musicFileName)
{
    PlaySound(musicFileName, NULL, SND_ASYNC | SND_LOOP);
}

void playSound(const char* soundFileName)
{
    PlaySound(soundFileName, NULL, SND_ASYNC);
}

int main()
{
    char start;
    cout << "\t-------------------------------" << endl;
    cout << "\t\t Simple Snake Game" << endl;
    cout << "\t-------------------------------" << endl;
    cout << "\tPress 's' to start: ";
    cin >> start;
    if (start == 's') {
        setup();
       playBackgroundMusic("snake_music.wav");

        // Clear the screen once before entering the game loop
        system("cls");

        while (!gameOver) {
               // playBackgroundMusic("hissing.wav");

            draw();
            input();
            logic();

            // Calculate delay based on the score
            int delay = max(20, 400 - (score * 4)); // Ensure a minimum delay

            // Sleep for the calculated duration
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));

            // Clear the screen before redrawing in the next iteration
            system("cls");
        }

        // Stop background music
        PlaySound(NULL, NULL, SND_ASYNC);

        // Play the "end_game.wav" sound only once when the game is over
        PlaySound("end_game.wav", NULL, SND_FILENAME);

        // Display the final score
        cout << "\t-------------------------------" << endl;
        cout << "\t\t Game Over!" << endl;
        cout << "\t\t Congratulations! Your Score: " << score << endl;
        cout << "\t-------------------------------" << endl;
    }

    return 0;
}

void setup()
{
    gameOver = false;
    dir = STOP;
    headX = width / 2;
    headY = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    specialFruitX = -1;
    specialFruitY = -1;
    score = 0;
    fruitsEaten = 0;
}

void draw()
{
    // Get the console handle
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set the cursor position to the top-left corner
    SetConsoleCursorPosition(console, {0, 0});

    // Construct the entire game board as a string
    string gameBoard = "\t\t";

    // Upper Border
    gameBoard += "\xC9";
    for (int i = 0; i < width; i++)
    {
        gameBoard += "\xCD";
    }
    gameBoard += "\xBB\n";

    // Snake, fruit, space, and side borders
    for (int i = 0; i < height; i++)
    {
        // left border
        gameBoard += "\t\t\xBA";

        // extra border in the middle
        if ((2 * width) / 3 == 0 && (5 <= i && i <= 15))
        {
            gameBoard += "\xB3";
        }
        else
        {
            // snake head, fruit, space, snake tail
            for (int j = 0; j < width; j++)
            {
                // snake head
                if (i == headY && j == headX)
                {
                    gameBoard += "O";
                }
                // special fruit
                else if (i == specialFruitY && j == specialFruitX)
                {
                    gameBoard += "S";
                }
                // fruit
                else if (i == fruitY && j == fruitX)
                {
                    gameBoard += "*";
                }
                // space, snake tail
                else
                {
                    bool print = false;
                    // tail
                    for (int k = 0; k < tail_len; k++)
                    {
                        if (tailx[k] == j && taily[k] == i)
                        {
                            gameBoard += "o";
                            print = true;
                        }
                    }
                    // space
                    if (!print)
                    {
                        // right border
                        if (j == width - 1 && (2 * width) / 3 == 0 && (5 <= i && i <= 15))
                        {
                            gameBoard += "\xB3";
                        }
                        else
                        {
                            gameBoard += " ";
                        }
                    }
                }
            }
        }

        // right border for the lower part of the extra border
        gameBoard += "\xBA";
        gameBoard += "\n";
    }

    // Lower Border
    gameBoard += "\t\t\xC8";
    for (int i = 0; i < width; i++)
    {
        gameBoard += "\xCD";
    }
    gameBoard += "\xBC\n";
    gameBoard += "\t\t\tScore: " + to_string(score) + "\n";
    gameBoard += "\t\t\tControls: a=left, d=right, w=up, s=down\n";

    // Display the entire game board at once
    cout << gameBoard;
}

void input()
{
    if (_kbhit())
        switch (_getch())
        {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        default:
            break;
        }
}

void logic()
{
    int prevx = tailx[0];
    int prevy = taily[0];
    int prev2x, prev2y;
    tailx[0] = headX;
    taily[0] = headY;
    for (int i = 1; i < tail_len; i++)
    {
        prev2x = tailx[i];
        prev2y = taily[i];
        tailx[i] = prevx;
        taily[i] = prevy;
        prevx = prev2x;
        prevy = prev2y;
    }

    switch (dir)
    {
    case LEFT:
        headX--;
        break;
    case RIGHT:
        headX++;
        break;
    case UP:
        headY--;
        break;
    case DOWN:
        headY++;
        break;
    default:
        break;
    }

    if (headX == (2 * width) / 3 && (5 <= headY && headY <= 15))
    {
        gameOver = true;
        return;
    }

    if (headX >= width || headX < 0 || headY >= height || headY < 0)
    {
        gameOver = true;
    }

    for (int i = 0; i < tail_len; i++)
    {
        if (tailx[i] == headX && taily[i] == headY)
        {
            gameOver = true;
        }
    }

    if (headX == fruitX && headY == fruitY)
    {
        score += 10;
        tail_len++;
        fruitsEaten++;

        playSound("eat_fruit.wav");

        fruitX = rand() % width;
        fruitY = rand() % height;

        if (fruitsEaten % 3 == 0)
        {
            specialFruitX = rand() % width;
            specialFruitY = rand() % height;
        }
    }

    if (headX == specialFruitX && headY == specialFruitY)
    {
        score += 30;
        playSound("eat_special.wav");
        specialFruitX = -1;
        specialFruitY = -1;
    }
}
