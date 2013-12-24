#include <iostream>
#include <vector>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <string>

#include "ConsoleGaming.h"

using namespace std;

HANDLE consoleHandle;

typedef vector<GameObject>::iterator randomAccess_iterator;
typedef vector<GameObject>::const_iterator const_iterator;

// Window constants
const int WindowWidth = 70;
const int WindowHeight = 30;
// Snake
const int SnakeSpeed = 1;
const int SnakeStartingLength = 5;
const char SnakeSymbol = '*',
        //Fruit
                FruitSymbol = '+';
const char PoisonSymbol = '-';

// Game variables
unsigned long sleepDuration = 200;
const int ESC = 27;
vector<GameObject> snake;
vector<GameObject> fruit;
vector<GameObject> poison;

COORD direction = { 1, 0 };

void CenterString(string s);
void GameInstructions();
void DisplayHighestScore();
int QuitGame();
void Menu();

void Update()
{
        // Save the tail, we might need it later.
        GameObject tail = *(snake.end() - 1);

        for (randomAccess_iterator i = snake.end() - 1; i != snake.begin(); --i)
        {
                GameObject next = *(i - 1);
                i->UpdateCoordinates(next.Coordinates);
        }

        if (kbhit())
        {
                char key = getch();
                switch (key)
                {
                case 'a':
                        direction.X = -SnakeSpeed;
                        direction.Y = 0;
                        break;
                case 'w':
                        direction.X = 0;
                        direction.Y = -SnakeSpeed;
                        break;
                case 'd':
                        direction.X = SnakeSpeed;
                        direction.Y = 0;
                        break;
                case 's':
                        direction.X = 0;
                        direction.Y = SnakeSpeed;
                        break;
				case 27:
					ClearScreen(consoleHandle);
					Menu();
					break;
                };
        }

        snake.begin()->Coordinates.X += direction.X;
        snake.begin()->Coordinates.Y += direction.Y;

        GameObject head = *snake.begin();
        for (randomAccess_iterator i = fruit.begin(); i != fruit.end(); ++i)
        {
                if (head.Coordinates.X == i->Coordinates.X && head.Coordinates.Y == i->Coordinates.Y)
                {
                        // Remove the old fruit, increase the snake's size
                        fruit.erase(i);
                        snake.push_back(tail);

                        // Add a new fruit
                        int x = rand() % WindowWidth;
                        int y = rand() % WindowHeight;
                        fruit.push_back(GameObject(x, y, FruitSymbol));

                        sleepDuration *= (sleepDuration > 50) * 0.1 + 0.9;
                        // Break, since you can't eat more than one fruit at the same time.
                        break;
                }

        }

		for (randomAccess_iterator i = poison.begin(); i != poison.end(); ++i)
        {
                if (head.Coordinates.X == i->Coordinates.X && head.Coordinates.Y == i->Coordinates.Y)
                {
                        // Remove the old fruit, decrease the snake's size
					if (snake.begin() < snake.end())
					{
                        poison.erase(i);
                        snake.erase(snake.end() - 1);

                        // Add a new fruit
                        int x = rand() % WindowWidth;
                        int y = rand() % WindowHeight;
                        poison.push_back(GameObject(x, y, PoisonSymbol));

                        sleepDuration *= (sleepDuration > 50) * 0.1 + 0.9;
                        // Break, since you can't eat more than one fruit at the same time.
                        break;
					}
					else
					{
						QuitGame();
					}
                }

        }
}

void Draw()
{
        ClearScreen(consoleHandle);

        for (const_iterator snakeBody = snake.begin(); snakeBody != snake.end(); ++snakeBody)
        {
                snakeBody->Draw(consoleHandle);
        }

        for (const_iterator singleFruit = fruit.begin(); singleFruit != fruit.end(); ++singleFruit)
        {
                singleFruit->Draw(consoleHandle);
        }

		for (const_iterator singlePoison = poison.begin(); singlePoison != poison.end(); ++singlePoison)
        {
                singlePoison->Draw(consoleHandle);
        }

}


void Menu()
{
        CenterString("Welcome to Snake Game 1.0!\n\n");
        CenterString("Choose action:\n");
        CenterString("1. Play New Game\n");
        CenterString("2.See Game Instructions\n");
        CenterString("3. See Highest Score\n");
        CenterString("4.Exit Game\n\n");
        
        char k = getch();
        switch(k)
        {
        case '1':
                ClearScreen(consoleHandle);
				 while (true)
                        {
                                Update();
                                Draw();
                                Sleep(sleepDuration);
                        }
                        break;
        case '2':
                ClearScreen(consoleHandle);
                        GameInstructions();
                        break;
        case '3':
                ClearScreen(consoleHandle);
                        DisplayHighestScore();
                        break;
        case '4':
                        QuitGame();
        }
}


int main()
{
	consoleHandle = GetStdHandle( STD_OUTPUT_HANDLE );
	// Randomize stuff
        srand(time(NULL));

        for (int i = SnakeStartingLength; i > -1; --i)
        {
                snake.push_back(GameObject(i, 0, SnakeSymbol));
        }

        int x1 = rand() % WindowWidth;
        int y1 = rand() % WindowHeight;
        fruit.push_back(GameObject(x1, y1, FruitSymbol));

		int x2 = rand() % WindowWidth;
		int y2 = rand() % WindowHeight;
		poison.push_back(GameObject(x2, y2, PoisonSymbol));
		
        Menu();

        return 0;
}

void CenterString(string str)
{
        int l = str.length();
        int pos = (int)((WindowWidth - l)/2);
        for (int i = 0; i < pos; ++i)
                cout << " ";
        cout << str;
}

void GameInstructions()
{
        CenterString("Here are the rules:\n\n");
        CenterString("You control the snake.\n");
        CenterString("When it eats one of the fruits, the snake grows bigger.\n");
        CenterString("Your goal is to grow as big as possible.\n");
        CenterString("If you ...... you die.\n\n");
        CenterString("To go FORWARD press 'w'\n");
        CenterString("To go BACK press 's'\n");
        CenterString("To turn RIGHT press 'd'\n");
        CenterString("To turn LEFT press 'a'\n\n");
        CenterString("Press 'ESC' to get back to the main menu\n");
        char k = getch();
        if (k == ESC)
        {
                ClearScreen(consoleHandle);
                Menu();
        }
}

void DisplayHighestScore()
{
        //must add later
        cout << "Highest score: .... \n";
        CenterString("Press 'ESC' to get back to the main menu\n");

        char k = getch();
        if (k == ESC)
        {
                ClearScreen(consoleHandle);
                Menu();
        }
}

int QuitGame()
{
        return 0;
}
