#include<iostream>
#include<conio.h>
#include<Windows.h>
#include<thread>
#include<chrono>

using namespace std;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;
bool gameOver;
const int height = 23; 
const int width = 23;  
int headX, headY, fruitX, fruitY, score;
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
        while (!gameOver) {
            draw();
            input();
            logic();

            // Sleep for 100 milliseconds
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            // Clear the console
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
    score = 0;
}


void draw()
{
    cout << "\x1B[2J\x1B[H";

    // Upper Border
    cout << "\t\t";
    for (int i = 0; i < width; i++)
    {
        cout << "# ";
    }
    cout << endl;

    // Snake, fruit, space, and side borders
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // left border
            if (j == 0)
            {
                cout << "\t\t#";
            }

            // extra border in the middle
            if (j == (2 * width) / 3 && (5 < i && i < 15))
            {
                cout << "   #";
            }

            // snake head
            if (i == headY && j == headX)
            {
                cout << "O";
            }
            // fruit
            else if (i == fruitY && j == fruitX)
            {
                cout << "*";
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
                        cout << "o";
                        print = true;
                    }
                }
                // space
                if (!print)
                {
                    cout << " ";
                }
            }

            // right border
            if (j == width - 1)
            {
                cout << "\t\t    #";
            }
        }

        cout << endl;
    }

    // Lower Border
    cout << "\t\t";
    for (int i = 0; i < width; i++)
    {
        cout << "# ";
    }
    cout << endl;
    cout << "\t\t\tScore: " << score << endl;
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
    if (headX == (2 * width) / 3 && (5 < headY && headY < 15))
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
        fruitX = rand() % width;
        fruitY = rand() % height;
        tail_len++;
    }
}
