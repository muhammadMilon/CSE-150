#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <thread>
#include <chrono>

using namespace std;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;
bool gameOver;
const int height = 23;
const int width = 40;
int headX, headY, fruitX, fruitY, specialFruitX, specialFruitY, score;
int tailx[100], taily[100];
int tail_len;

void setup();
void draw();
void input();
void logic();

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

        // Clear the screen once before entering the game loop
        system("cls");

        while (!gameOver) {
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
    // tail logic
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

    // direction logic
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

    // check if snake touches the middle border
    if (headX == (2 * width) / 3 && (5 <= headY && headY <= 15))
    {
        gameOver = true;
        return;
    }

    // touch walls
    if (headX >= width || headX < 0 || headY >= height || headY < 0)
    {
        gameOver = true;
    }

    // snake bite itself
    for (int i = 0; i < tail_len; i++)
    {
        if (tailx[i] == headX && taily[i] == headY)
        {
            gameOver = true;
        }
    }

    // snake eat fruit
    if (headX == fruitX && headY == fruitY)
    {
        score += 10;
        tail_len++;
        if (score % 30 == 0 && score > 0)  // Check if the score is a multiple of 30 and greater than 0
        {
            // Generate a special fruit
            specialFruitX = rand() % width;
            specialFruitY = rand() % height;
        }
        else
        {
            // Generate a normal fruit
            fruitX = rand() % width;
            fruitY = rand() % height;
        }
    }

    // snake eat special fruit
    if (headX == specialFruitX && headY == specialFruitY)
    {
        score += 50;
        specialFruitX = -1; // Reset special fruit position
        specialFruitY = -1;
    }
}
