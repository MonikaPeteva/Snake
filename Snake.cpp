#include <iostream>
#include <vector>
#include <conio.h>
#include <time.h>
#include <string>
#include <fstream>
#include <cstdlib>

#include "ConsoleGaming.h"

using namespace std;

HANDLE consoleHandle;

typedef vector<GameObject>::iterator randomAccess_iterator;
typedef vector<GameObject>::const_iterator const_iterator;

// Window constants
const int WindowWidth = 70;
const int WindowHeight = 25;
const int BorderX = 3, BorderY = 2;


// Snake
const int SnakeSpeed = 1;
const int SnakeStartingLength = 5;
const char SnakeSymbol = '*',
           FruitSymbol = '+',
           PoisonSymbol = '-',
           WallSymbol = 'X';

// Game variables
unsigned long sleepDuration = 200;
const int ESC = 27;
vector<GameObject> snake;
vector<GameObject> fruit;
vector<GameObject> poison;
vector<GameObject> wall;

unsigned int Score;
unsigned int MaxScore = 0;
COORD direction ;

void Initialization();
void CenterString(string s);
void PrintScore();
void GameInstructions();
void DisplayHighestScore();
COORD GanaratingCoordinations();
void QuitGameOver();
int  QuitGame();
void Menu();
void SaveScore(int score);
int GetScore();


void Update()
{
        // Save the tail, we might need it later.
        GameObject tail = *(snake.end() - 1);
                tail.Symbol = ' ';

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
						Beep(420,200);
                        direction.X = -SnakeSpeed;
                        direction.Y = 0;
                        break;
					case 'w':
						Beep(620,200);
                        direction.X = 0;
                        direction.Y = -SnakeSpeed;
                        break;
                    case 'd':
						Beep(420,200);
                        direction.X = SnakeSpeed;
                        direction.Y = 0;
                        break;
                    case 's':
						Beep(620,200);
                        direction.X = 0;
                        direction.Y = SnakeSpeed;
                        break;
					case 27:
						//checking if MaxScore has to be updated
						if (MaxScore < Score)
							{
								MaxScore=Score;
							}

						//checking if MaxScore is the highest score achieved in the game
						int score = GetScore();
						if ( MaxScore > score )
							{
								SaveScore(MaxScore);
							}

                        ClearScreen(consoleHandle);
                        Menu();
                        break;
                };
        }

        snake.begin()->Coordinates.X += direction.X;
        snake.begin()->Coordinates.Y += direction.Y;

        GameObject head = *snake.begin();

                head.Draw(consoleHandle);
                tail.Draw(consoleHandle);


                for (randomAccess_iterator i = fruit.begin(); i != fruit.end(); ++i)
        {
                if (head.Coordinates.X == i->Coordinates.X && head.Coordinates.Y == i->Coordinates.Y)
                {
                        // Remove the old fruit, increase the snake's size
                        Score += 5;
                        PrintScore();

                        fruit.erase(i);

                        snake.push_back(tail);

                        tail.Symbol = '*';
                        tail.Draw(consoleHandle);
						Beep(700,850);

                        // Add a new fruit
                                                                                                
                       COORD CurrentCoordinates = GanaratingCoordinations ();
                       tail = GameObject(CurrentCoordinates.X, CurrentCoordinates.Y, FruitSymbol);
                       fruit.push_back(tail);
						tail.Draw(consoleHandle);


                        sleepDuration *= (sleepDuration > 50) * 0.1 + 0.9;

                        // Break, since you can't eat more than one fruit at the same time.
                        break;
                }

        }

                for (randomAccess_iterator i = poison.begin(); i != poison.end(); ++i)
        {
                if (head.Coordinates.X == i->Coordinates.X && head.Coordinates.Y == i->Coordinates.Y)
                {
                    // Remove poison and decrease the snake's size
						if (snake.begin() < snake.end()-1)
                            {
								if (Score == 0)
								{
									QuitGameOver();
								}
								else
									Score -= 5;
                                PrintScore();

								poison.erase(i);

                                tail = *(snake.end() - 1);
                                tail.Symbol = ' ';
                                tail.Draw(consoleHandle);

								snake.erase(snake.end() - 1);
								Beep(220,800);

                        // Add a new poisonous fruit
                                                
                                COORD CurrentCoordinates = GanaratingCoordinations ();;
                                tail = GameObject(CurrentCoordinates.X, CurrentCoordinates.Y, PoisonSymbol);
                                poison.push_back(tail);
                                tail.Draw(consoleHandle);


                       // Break, since you can't eat more than one fruit at the same time.
								break;
							}
                      else
                            {
								QuitGameOver();
                            }
                }

        }

                //if the snake touches one of the borders
                for (randomAccess_iterator i = wall.begin(); i != wall.end(); ++i)
				{
                       if (head.Coordinates.X == i->Coordinates.X && head.Coordinates.Y == i->Coordinates.Y)
							{
                                        tail.Symbol = ' ';
                                        tail.Draw(consoleHandle);
                                        i->Symbol = '*';
                                        i->Color = 0x4;
                                        i->Draw(consoleHandle);
										QuitGameOver();
							}

				}


				//if the snake crosses itself
                for (randomAccess_iterator i = snake.begin() + 1; i != snake.end(); ++i)
				{
                       if (head.Coordinates.X == i->Coordinates.X && head.Coordinates.Y == i->Coordinates.Y)
							{
                                        tail.Symbol = ' ';
                                        tail.Draw(consoleHandle);
                                        i->Symbol = '*';
                                        i->Color = 0x4;
                                        i->Draw(consoleHandle);
										QuitGameOver();
							}

				}
}

void Draw()
{
        PrintScore();
		CenterString("\nPress ESC to quit game and return to the main menu\n");

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

                for (const_iterator brick = wall.begin(); brick != wall.end(); ++brick)
        {
                brick->Draw(consoleHandle);
        }
}


void Menu()
{
		string border (WindowWidth, ':');
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //change color of border
		cout << border << endl;
		cout << border << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

		cout << "\n\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); //change color of text
        CenterString("Welcome to SNAKE GAME 1.0!\n\n");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        CenterString("Choose action:\n");
		cout << endl;
        CenterString("1. Play New Game\n");
        CenterString("2. See Game Instructions\n");
        CenterString("3. See Highest Score\n");
        CenterString("4. Exit Game\n");

		Beep(520,300);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);  //change color of border
        
cout << "      _    _" << endl;
cout << "   ,-(|)--(|)-." << endl;
cout << "   \_   ..   _/" << endl;
cout << "     \______/"<<endl;
cout << "       V  V                                  ____" << endl;
cout << "       `.^^`.                               /^,--`" << endl;
cout << "         \^^^\                             (^^\\" << endl;
cout << "         |^^^|                  _,-._       \^^\\"<< endl;
cout << "        (^^^^\      __      _,-'^^^^^`.    _,'^^)"<< endl;
cout << "         \^^^^`._,-'^^`-._.'^^^^__^^^^ `--'^^^_/"<< endl;
cout << "          \^^^^^ ^^^_^^^^^^^_,-'  `.^^^^^^^^_/"<< endl;
cout << "           `.____,-' `-.__.'        `-.___.'"<<endl;

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

        char k = getch();
        switch(k)
        {
        case '1':
				Beep(520,200);
                ClearScreen(consoleHandle);
                Initialization();
                Draw();
                                while (true)
                        {
                                Update();
								Sleep(sleepDuration);
                        }
                        break;
        case '2':
				Beep(520,200);
                ClearScreen(consoleHandle);
                        GameInstructions();
                        break;
        case '3':
				Beep(520,200);
                ClearScreen(consoleHandle);
                        DisplayHighestScore();
                        break;
        case '4':
				Beep(520,200);
                        QuitGame();
        }
}


int main()
{
        consoleHandle = GetStdHandle( STD_OUTPUT_HANDLE );
        // Randomize stuff

		srand(time(NULL));
        COORD CurrentCoordinates = GanaratingCoordinations ();
        fruit.push_back(GameObject(CurrentCoordinates.X, CurrentCoordinates.Y, FruitSymbol));

        CurrentCoordinates = GanaratingCoordinations ();
        poison.push_back(GameObject(CurrentCoordinates.X, CurrentCoordinates.Y, PoisonSymbol));
                
        Menu();

        return 0;
}

void GeneratingWall (int WindowWidth, int WindowHeight)
{
        for (int x = BorderX; x <= WindowWidth-BorderX; ++x)
        {
                wall.push_back(GameObject(x, BorderY, WallSymbol)); //top border
                wall.push_back(GameObject(x, WindowHeight-BorderY, WallSymbol)); //bottom border
        }
        for (int y = BorderY; y <= WindowHeight-BorderY; ++y)
        {
                wall.push_back(GameObject(BorderX, y, WallSymbol)); //left border
                wall.push_back(GameObject(WindowWidth-BorderX, y, WallSymbol)); //right border
        }
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
	string border (WindowWidth, ':');
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //change color of border
		cout << border << endl;
		cout << border << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		cout << "\n\n";


        CenterString("Here are the rules:\n\n");
        CenterString("You control the snake.\n");
		CenterString("When it eats one of the fruits, the snake grows longer.\n");
		CenterString("When it eats one of the poisonous fruits, the snake gets shorter.\n");
		CenterString("Your goal is to make the snake as long as possible.\n");
        CenterString("If the snake crosses itself or bumps into a border, it dies.\n\n");
        CenterString("To go FORWARD press 'w'\n");
        CenterString("To go BACK press 's'\n");
        CenterString("To turn RIGHT press 'd'\n");
        CenterString("To turn LEFT press 'a'\n\n");
        CenterString("Press 'ESC' to get back to the main menu\n\n\n");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //change color of border
		cout << border << endl;
		cout << border << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

        char k = getch();
        if (k == ESC)  //return to main menu if ESC is pressed
        {
                ClearScreen(consoleHandle);
                Menu();
        }
}


void QuitGameOver()
{
        if (MaxScore < Score) 
			{
				MaxScore = Score;
			}

		int score = GetScore();
		if ( MaxScore > score )
		{
			SaveScore(MaxScore);
		}

		Beep(520,2000);
        GameObject current (0,9,' ');
        current.Color = 0x4;
        current.Draw(consoleHandle);

        cout<<"\n";
        cout<<" #     #                  #                            \n";
        cout<<"  #   #   ####  #    #    #        ####   ####  ###### \n";
        cout<<"   # #   #    # #    #    #       #    # #      #      \n";
        cout<<"    #    #    # #    #    #       #    #  ####  #####  \n";
        cout<<"    #    #    # #    #    #       #    #      # #      \n";
        cout<<"    #    #    # #    #    #       #    # #    # #      \n";
        cout<<"    #     ####   ####     #######  ####   ####  ###### \n";
        
        GameObject tail = *(snake.end() - 1);
        tail.Draw(consoleHandle);

        char k = getch();
        ClearScreen(consoleHandle);
        Menu();
}

int QuitGame()
{
		int score = GetScore();
		if ( MaxScore > score )
		{
			SaveScore(MaxScore);
		}

        ClearScreen(consoleHandle);
        
    exit(0);
}

COORD GanaratingCoordinations ()
{
        COORD momental;
        momental.X = rand() % (WindowWidth-2*(BorderX+1))+BorderX+1;
        momental.Y = rand() % (WindowHeight-2*(BorderY+1))+BorderY+1;
        return momental;
}

void Initialization ()
{
        Score = 0;
        direction.X = 1;
        direction.Y = 0;

        while(snake.begin() != snake.end())
			{
				snake.erase(snake.end() - 1);
			}

        for (int i = SnakeStartingLength-1; i > -1; --i)
        {
                        snake.push_back(GameObject(i+BorderX+1, BorderY+1, SnakeSymbol));
        }

                GeneratingWall (WindowWidth, WindowHeight);
}

void PrintScore()
{
        string text;
        GameObject current (0,0,' ');
        current.Color = 0x2;
        current.Draw(consoleHandle);
        text = "Highest Score This Session: " +  to_string(MaxScore) + "        Score: " + to_string(Score);
        CenterString(text);
        current.Color = 15;
}

void SaveScore(int score) //saves score to scores.txt
{
	ofstream o_ScoreFile("scores.txt");
	if (o_ScoreFile.is_open())
	{
		o_ScoreFile << score << "\n";
		o_ScoreFile.close();
	}
}

int GetScore() //reads score from scores.txt
{
	ifstream i_ScoreFile("scores.txt");
		int current_high_score = 0;
		if (i_ScoreFile.is_open())
			{
				while (!i_ScoreFile.eof())
					{
						i_ScoreFile >> current_high_score;
					}
			}
		return current_high_score;
}

void DisplayHighestScore()
{
		string border (WindowWidth, ':');
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //change color of border
		cout << border << endl;
		cout << border << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		cout << "\n\n";

		CenterString("Highest score: " + to_string(GetScore()) + "\n\n");
        CenterString("Press 'ESC' to get back to the main menu\n\n\n");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //change color of border
		cout << border << endl;
		cout << border << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

        char k = getch();
        if (k == ESC)
        {
                ClearScreen(consoleHandle);
                Menu();
        }
}
