#include <iostream>
#include <fstream>
#include <Windows.h>
#include <MMSystem.h>
#include <mmsystem.h>
#include <conio.h>
#include <thread>
#include <chrono>

using namespace std;

// Color constants
const int COLOR_BLACK = 0;
const int COLOR_GREEN = 2;
const int COLOR_RED = 12;
const int COLOR_YELLOW = 14;
const int COLOR_WHITE = 15;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum Difficulty { EASY, MEDIUM, HARD };

Direction dir;
bool gameOver;
int height, width, delay;
int headX, headY, fruitX, fruitY, specialFruitX, specialFruitY, score;
int tailx[100], taily[100];
int tail_len;
int fruitsEaten;
int highestScoreEasy = 0;
int highestScoreMedium = 0;
int highestScoreHard = 0;
Difficulty currentDifficulty = MEDIUM; // Default to medium difficulty

void setup();
void draw();
void input();
void logic();
void playBackgroundMusic(const char* musicFileName);
void playSound(const char* soundFileName);
void setColor(int color);
void saveHighestScore();
void loadHighestScore();
void setDifficulty();
void adjustDifficultyParameters();

void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void playBackgroundMusic(const char* musicFileName)
{
    PlaySound(musicFileName, NULL, SND_ASYNC | SND_LOOP);
}

void playSound(const char* soundFileName)
{
    PlaySound(soundFileName, NULL, SND_ASYNC);
}

void saveHighestScore()
{
    ofstream file;
    switch (currentDifficulty)
    {
    case EASY:
        file.open("highest_score_easy.txt");
        file << highestScoreEasy;
        file.close();
        break;
    case MEDIUM:
        file.open("highest_score_medium.txt");
        file << highestScoreMedium;
        file.close();
        break;
    case HARD:
        file.open("highest_score_hard.txt");
        file << highestScoreHard;
        file.close();
        break;
    }
}

void loadHighestScore()
{
    ifstream file;
    string fileName;

    switch (currentDifficulty)
    {
    case EASY:
        fileName = "highest_score_easy.txt";
        break;
    case MEDIUM:
        fileName = "highest_score_medium.txt";
        break;
    case HARD:
        fileName = "highest_score_hard.txt";
        break;
    }

    file.open(fileName);

    if (file.is_open())
    {
        // Read the highest score from the file
        switch (currentDifficulty)
        {
        case EASY:
            file >> highestScoreEasy;
            break;
        case MEDIUM:
            file >> highestScoreMedium;
            break;
        case HARD:
            file >> highestScoreHard;
            break;
        }

        file.close();
    }
    else
    {
        // If the file doesn't exist, set the highest score to a minimum value
        switch (currentDifficulty)
        {
        case EASY:
            highestScoreEasy = 0;
            break;
        case MEDIUM:
            highestScoreMedium = 0;
            break;
        case HARD:
            highestScoreHard = 0;
            break;
        }
    }
}

void setDifficulty()
{
    cout << "\tChoose Difficulty:" << endl;
    cout << "\t1. Easy" << endl;
    cout << "\t2. Medium" << endl;
    cout << "\t3. Hard" << endl;

    int choice;
    cout << "\tEnter your choice (1-3): ";
    cin >> choice;

    switch (choice)
    {
    case 1:
        currentDifficulty = EASY;
        break;
    case 2:
        currentDifficulty = MEDIUM;
        break;
    case 3:
        currentDifficulty = HARD;
        break;
    default:
        currentDifficulty = MEDIUM;
        break;
    }

    // Adjust difficulty parameters based on the chosen difficulty
    adjustDifficultyParameters();
}

void adjustDifficultyParameters()
{
    switch (currentDifficulty)
    {
    case EASY:
        // Adjust parameters for easy difficulty
        height = 25;
        width = 40;
        delay = 300 - score * 10; // Adjust delay based on score (slower as score increases)
        break;
    case MEDIUM:
        // Adjust parameters for medium difficulty
        height = 20;
        width = 30;
        delay = 200 - score * 10; // Adjust delay based on score (slower as score increases)
        break;
    case HARD:
        // Adjust parameters for hard difficulty
        height = 10;
        width = 20;
        delay = 100 - score * 10; // Adjust delay based on score (slower as score increases)
        break;
    default:
        // Default to medium difficulty parameters
        height = 20;
        width = 30;
        delay = 50 - score * 10; // Adjust delay based on score (slower as score increases)
        break;
    }

    // Ensure that delay doesn't go below a minimum value to prevent the game from becoming too slow
    if (delay < 10)
        delay = 10;
}


int main()
{
    loadHighestScore();  // Load the highest score from the file

    char start;
    cout << "\t-------------------------------" << endl;
    cout << "\t\t Simple Snake Game" << endl;
    cout << "\t-------------------------------" << endl;

    // Choose difficulty level
    setDifficulty();

    cout << "\tPress 's' to start: ";
    cin >> start;

    if (start == 's') {
        setup();
        playBackgroundMusic("snake_music.wav");

        // Clear the screen once before entering the game loop
        std::cout << "\x1B[2J\x1B[H";

        while (!gameOver) {
            draw();
            input();
            logic();

            // Sleep for the calculated duration
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));

            // Clear the screen before redrawing in the next iteration
            std::cout << "\x1B[2J\x1B[H";
        }

        // Stop background music
        PlaySound(NULL, NULL, SND_ASYNC);

        // Play the "end_game.wav" sound only once when the game is over
        PlaySound("END.wav", NULL, SND_FILENAME);

        // Save the highest score if the current score is higher
        switch (currentDifficulty) {
            case EASY:
                if (score > highestScoreEasy) {
                    highestScoreEasy = score;
                    saveHighestScore();  // Save the highest score
                }
                break;
            case MEDIUM:
                if (score > highestScoreMedium) {
                    highestScoreMedium = score;
                    saveHighestScore();  // Save the highest score
                }
                break;
            case HARD:
                if (score > highestScoreHard) {
                    highestScoreHard = score;
                    saveHighestScore();  // Save the highest score
                }
                break;
        }

        // Display the highest score and other game over details
        cout << "\t--------------------------------------------" << endl;
        cout << "\t\t Game Over!" << endl << endl;
        cout << "\t\t Your Score: " << score << endl;
        cout << "\t\t Highest Score: ";
        switch (currentDifficulty) {
            case EASY:
                cout << highestScoreEasy;
                break;
            case MEDIUM:
                cout << highestScoreMedium;
                break;
            case HARD:
                cout << highestScoreHard;
                break;
        }
        cout << endl;
        cout << "\t--------------------------------------------" << endl;

        // Add a delay before closing the application (optional)
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    getch();
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
    setColor(COLOR_WHITE);
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
        setColor(COLOR_WHITE);
        gameBoard += "\t\t\xBA";

        // extra border in the middle
        if ((2 * width) / 3 == 0 && (5 <= i && i <= 15))
        {
            setColor(COLOR_YELLOW);
            gameBoard += "\xB3";
            setColor(COLOR_WHITE);
        }
        else
        {
            // snake head, fruit, space, snake tail
            for (int j = 0; j < width; j++)
            {
                // snake head
                if (i == headY && j == headX)
                {
                    setColor(COLOR_RED);
                    gameBoard += "O";
                    setColor(COLOR_WHITE);
                }
                // special fruit
                else if (i == specialFruitY && j == specialFruitX)
                {
                    setColor(COLOR_RED);
                    gameBoard += "S";
                    setColor(COLOR_WHITE);
                }
                // fruit
                else if (i == fruitY && j == fruitX)
                {
                    setColor(COLOR_GREEN);
                    gameBoard += "*";
                    setColor(COLOR_WHITE);
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
                            setColor(COLOR_RED);
                            gameBoard += "o";
                            setColor(COLOR_WHITE);
                            print = true;
                        }
                    }
                    // space
                    if (!print)
                    {
                        // right border
                        if (j == width - 1 && (2 * width) / 3 == 0 && (5 <= i && i <= 15))
                        {
                            setColor(COLOR_YELLOW);
                            gameBoard += "\xB3";
                            setColor(COLOR_WHITE);
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

    // Debug output
    cout << "DEBUG: headX=" << headX << ", headY=" << headY << endl;

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

