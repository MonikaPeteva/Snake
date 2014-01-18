#include <iostream>
#include <vector>
#include <conio.h>
#include <time.h>
#include <string>
#include <fstream>
#include <cstdlib>

#include "ConsoleGaming.h"

using namespace std;
using namespace ConsoleColors;

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

// fruit / poison / wall / powerup fruit symbols 

        WallSymbol = 'X',
        PoisonSymbol = '-',
		PowerUpWallsSymbol = 'W';

char FruitSymbol; //not a constant, it can change;

ConsoleColor ColorOfFruit,
	ColorOfSnake = Green,
	ColorOfText = Green,
	ColorOfWalls = White;

// Game variables

unsigned long sleepDuration = 200;
const int ESC = 27;
enum SoundMood { happy, neutral, sad };
bool isSoundOn = true;

vector<GameObject> snake;
vector<GameObject> fruit;
vector<GameObject> poison;
vector<GameObject> wall;
vector<GameObject> powerupwalls;

// points, scores and skills

int Points;
unsigned int Score = 0;
unsigned int MaxScore = 0;
unsigned int MoveThroughWalls = 0;
bool isPowerUpWalls = false; // true if there is a PowerUp fruit; false otherwise


COORD direction ;

void Menu();
void GameInstructions();
void DisplayHighestScore();
int  QuitGame();
void Initialization();
void Draw();
void Update();
void GeneratingWall ();
COORD GeneratingCoordinations();
void CenterString(string s);
void PrintScore();
void SaveScore(int score);
int GetScore();
void SetDifficulty();
void QuitGameOver();
char RandomizeFruitSymbol();
void SaveGame( vector<GameObject> *snake ,vector<GameObject> *fruit, vector<GameObject> *poison, unsigned int* score  );
void UpdateHighestScore();
void ChangeOptions();
void GraphicsMenu();
void SetColorMenu(ConsoleColor& color);
void MakeSound( SoundMood mood );
void SetSoundMenu();
bool isCoordFree( COORD coordinates );

int main()
{
        consoleHandle = GetStdHandle( STD_OUTPUT_HANDLE );
        // Randomize stuff

        srand(time(NULL));

        FruitSymbol = RandomizeFruitSymbol();
        COORD CurrentCoordinates = GeneratingCoordinations();


        fruit.push_back(GameObject(CurrentCoordinates.X, CurrentCoordinates.Y, FruitSymbol));

                for (randomAccess_iterator i = fruit.begin(); i != fruit.end(); ++i)
                        {
                                i->Color= ColorOfFruit ;
                        }

        CurrentCoordinates = GeneratingCoordinations ();
        poison.push_back(GameObject(CurrentCoordinates.X, CurrentCoordinates.Y, PoisonSymbol));
                
                
        Menu();

        return 0;
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
        CenterString("Welcome to SNAKE GAME 1.0!\n");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        CenterString("Choose action:\n");
                cout << endl;
        CenterString("1. Play New Game\n");
        CenterString("2. See Game Instructions\n");
        CenterString("3. See Highest Score\n");;
		CenterString("4. Change Game Options\n");
        CenterString("5. Exit Game\n");

          MakeSound(neutral); //make sound when menu appears
        
				
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
             //play new game
			MakeSound(neutral);
                ClearScreen(consoleHandle);
                Initialization();
                MoveThroughWalls = 0;
                Draw();
                while (true)
                   {
                        Update();
                        Sleep(sleepDuration);
                   }
                break;
        case '2':  
             //see instructions
                MakeSound(neutral);
                ClearScreen(consoleHandle);
                GameInstructions();
                break;
        case '3': 
                        //see highest score
                MakeSound(neutral);
                ClearScreen(consoleHandle);
                DisplayHighestScore();
                break;
		case '4':
				MakeSound(neutral);
                ClearScreen(consoleHandle);
				ChangeOptions();
				break;
        case '5': 
                        //quit game
                MakeSound(neutral);
                QuitGame();
        }
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

		CenterString("Once you have more than 30 points, you may see a special 'W' fruit.\n");
		CenterString("It allows you to move through the walls.\n\n");

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

 void DisplayHighestScore() //show highest score achieved to date
{
                string border (WindowWidth, ':');
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //change color of menu border
                cout << border << endl;
                cout << border << endl;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                cout << "\n\n";

                CenterString("Highest To Date: " + to_string(GetScore()) + "\n\n");
				CenterString("Highest This Session: " + to_string(MaxScore) + "\n\n");

				CenterString("Press 'ESC' to get back to the main menu\n\n\n");

                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //change color of menu border
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


int QuitGame()
{
                //before quitting the game, check if highest score should be updated
		UpdateHighestScore();
        ClearScreen(consoleHandle);
        
    exit(0);
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
                                                for (randomAccess_iterator i = snake.begin(); i != snake.end(); ++i)
													i->Color = ColorOfSnake;
        }

        GeneratingWall ();
}

void Draw()
{
        PrintScore();
		cout << "\n";
        CenterString("Press 'K' to save game and ESC to quit and return to the main menu\n");

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

                for (randomAccess_iterator brick = wall.begin(); brick != wall.end(); ++brick)
        {
				brick ->Color = ColorOfWalls;
                brick->Draw(consoleHandle);
        }
}

void Update()
{
        // Save the tail, we might need it later.
				GameObject tail = *(snake.end() - 1);
                tail.Symbol = ' ';
                tail.Color = ColorOfSnake;

        for (randomAccess_iterator i = snake.end() - 1; i != snake.begin(); --i)
        {
                GameObject next = *(i - 1);
                i->Color = ColorOfSnake;
                i->UpdateCoordinates(next.Coordinates);
        }

        if (kbhit())
        {
                char key = getch();
                switch (key)
                {
                                        case 'a':
                                                MakeSound(neutral);
                        direction.X = -SnakeSpeed;
                        direction.Y = 0;
                        break;
                                        case 'w':
                                                MakeSound(neutral);
                        direction.X = 0;
                        direction.Y = -SnakeSpeed;
                        break;
                    case 'd':
                                                MakeSound(neutral);
                        direction.X = SnakeSpeed;
                        direction.Y = 0;
                        break;
                    case 's':
                                                MakeSound(neutral);
                        direction.X = 0;
                        direction.Y = SnakeSpeed;
                        break;
					case ESC:

                     //checking if MaxScore has to be updated
                        if (MaxScore < Score)
                        {
                              MaxScore=Score;
                        }
                   
                       UpdateHighestScore();   //checking if MaxScore is the highest score achieved in the game
					   isPowerUpWalls = false;  // there will be no power up fruit generated
                        ClearScreen(consoleHandle);
                        Menu();
                        break;
					case 'k':
						SaveGame( &snake , &fruit, &poison, &Score );
						break;

					default:
						break;
                };
        }

        snake.begin()->Coordinates.X += direction.X;
        snake.begin()->Coordinates.Y += direction.Y;

		GameObject head = *snake.begin();
        head.Color = ColorOfSnake;
				
        head.Draw(consoleHandle);
        tail.Draw(consoleHandle);


                for (randomAccess_iterator i = fruit.begin(); i != fruit.end(); ++i)
        {
                if (head.Coordinates.X == i->Coordinates.X && head.Coordinates.Y == i->Coordinates.Y)
                {
                        //increase score and print new score
                        Score += Points;
                        PrintScore();
						
						// Remove the old fruit, increase the snake's size
                        fruit.erase(i);

                        snake.push_back(tail);

                        tail.Symbol = '*';
                        tail.Color = ColorOfSnake;
                        tail.Draw(consoleHandle);
                        MakeSound(happy);

                        // Add a new fruit
                                                                                                
                       COORD CurrentCoordinates = GeneratingCoordinations ();
                       FruitSymbol = RandomizeFruitSymbol();
                       tail = GameObject(CurrentCoordinates.X, CurrentCoordinates.Y, FruitSymbol);
                       fruit.push_back(tail);

                       for (randomAccess_iterator i = fruit.begin(); i != fruit.end(); ++i)
                                        {
                                                tail.Color= ColorOfFruit;
                                        }

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
                                           if (Score == 0 || Score <= 5)
											{
												QuitGameOver();
											}
											else
											{
												Score -= 5;
											}

                                PrintScore();

                                poison.erase(i);

                                tail = *(snake.end() - 1);
                                tail.Symbol = ' ';
                                tail.Draw(consoleHandle);

                                snake.erase(snake.end() - 1);
                                MakeSound(sad);

                        // Add a new poisonous fruit
                                                
                                COORD CurrentCoordinates = GeneratingCoordinations ();
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


				if ( Score > 30 && ( isPowerUpWalls == false ) ) //generate PowerUp fruit only if player has more than 30 points and there are no other PW fruits
				{
					srand (time(NULL));
					if ( ( rand() % 13 ) == 0 )		// generate PowerUp fruits at random
					{
						COORD CurrentCoordinates = GeneratingCoordinations ();
						tail = GameObject(CurrentCoordinates.X, CurrentCoordinates.Y, PowerUpWallsSymbol);
						powerupwalls.push_back(tail);
						tail.Color = Yellow;
						tail.Draw(consoleHandle);
						isPowerUpWalls = true;
					}
				}

				  for (randomAccess_iterator i = powerupwalls.begin(); i != powerupwalls.end(); ++i)
				{
					if (head.Coordinates.X == i->Coordinates.X && head.Coordinates.Y == i->Coordinates.Y)
					{
						MakeSound(happy);
						 powerupwalls.erase(i);
						 isPowerUpWalls = false;
						 MoveThroughWalls++;
						 PrintScore();
						 break;
					}
				  }
                                

			if ( MoveThroughWalls == 0) //if the snake hasn't eaten a PowerUp fruit, it dies when it touches one of the border;
			{ 
                for (randomAccess_iterator i = wall.begin(); i != wall.end(); ++i)
                {
                     if (head.Coordinates.X == i->Coordinates.X && head.Coordinates.Y == i->Coordinates.Y)
					{
                          tail.Symbol = ' ';
                          tail.Draw(consoleHandle);
                          i->Symbol = '*';
                          i->Color = Red;
                          i->Draw(consoleHandle);
                          QuitGameOver();
					}
				}
             }



				 if ( MoveThroughWalls > 0 ) // if the snake has eaten a PowerUp fruit, it moves through the wall;
					{
						for (randomAccess_iterator i = snake.begin(); i != snake.end(); ++i )
						{
							if (i->Coordinates.X == ( BorderX ) && direction.X == -1) // if some part of the snake is about to touch the left wall, flip x coordinates
							{
								MoveThroughWalls--;
								PrintScore();
								i->Coordinates.X = ( WindowWidth - BorderX - 1);
								
								for (randomAccess_iterator brick = wall.begin(); brick != wall.end(); ++brick) // draw wall again
								{
									brick->Draw(consoleHandle);
									brick->Color = ColorOfWalls;
								}

								i->Color = ColorOfSnake;
								i->Draw(consoleHandle);

							}

							if (i->Coordinates.X == ( WindowWidth - BorderX ) && direction.X == 1) // if some part of the snake is about to touch the right wall, flip x coordinates
							{
								MoveThroughWalls--;
								PrintScore();
								i->Coordinates.X = ( BorderX + 1);

								for (randomAccess_iterator brick = wall.begin(); brick != wall.end(); ++brick)  // draw wall again
								{
									brick->Draw(consoleHandle);
									brick->Color = ColorOfWalls;
								}

								i->Color = ColorOfSnake;
								i->Draw(consoleHandle);
							}

							if (i->Coordinates.Y == ( BorderY ) && direction.Y == -1) // if some part of the snake is about to touch the upper wall, flip y coordinates
							{
								MoveThroughWalls--;
								PrintScore();
								i->Coordinates.Y = ( WindowHeight - BorderY - 1 );

								for (randomAccess_iterator brick = wall.begin(); brick != wall.end(); ++brick)  // draw wall again
								{
									brick->Draw(consoleHandle);
									brick->Color = ColorOfWalls;
								}

								i->Color = ColorOfSnake;
								i->Draw(consoleHandle);i->Draw(consoleHandle);
							}

							if (i->Coordinates.Y == ( WindowHeight - BorderY ) && direction.Y == 1) // if some part of the snake is about to touch the bottom wall, flip y coordinates
							{
								MoveThroughWalls--;
								PrintScore();
								i->Coordinates.Y = ( BorderY + 1 );

								for (randomAccess_iterator brick = wall.begin(); brick != wall.end(); ++brick) // draw wall again
								{
									brick->Draw(consoleHandle);
									brick->Color = ColorOfWalls;
								}
								
								i->Color = ColorOfSnake;
								i->Draw(consoleHandle);

							}
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
                                        i->Color = Red;
                                        i->Draw(consoleHandle);
                                                                                QuitGameOver();
                                                        }

                                }
}

void GeneratingWall ()
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

COORD GeneratingCoordinations()
{
        COORD momental;
		srand(time(NULL));

		do
		{
			momental.X = rand() % (WindowWidth-2*(BorderX+1))+BorderX+1;
			momental.Y = rand() % (WindowHeight-2*(BorderY+1))+BorderY+1;
		}
		
		while ( isCoordFree ( momental ) == false ); 

		

        return momental;
}

bool isCoordFree( COORD coordinates ) //check if coordinates are free
{
	if ( fruit.begin() != fruit.end())  // if there is a fruit, check if coordinates are the same
		{
			for (randomAccess_iterator i = fruit.begin(); i != fruit.end(); ++i)
			{ 
				if ( ( coordinates.X == i->Coordinates.X ) && ( coordinates.Y == i->Coordinates.Y ))
				{
					return false;
				}
			}
		}

		if ( poison.begin() != poison.end())  // if there is a poisonous fruit, check if coordinates are the same
		{
			for (randomAccess_iterator i = poison.begin(); i != poison.end(); ++i)
			{ 
				if ( ( coordinates.X == i->Coordinates.X ) && ( coordinates.Y == i->Coordinates.Y ) )
				{
					return false;
				}
			}
		}

			for (randomAccess_iterator i = snake.begin(); i != snake.end(); ++i) // check all the coordinates of the snake
             {
                if ( ( coordinates.X == i->Coordinates.X ) && ( coordinates.Y == i->Coordinates.Y ) )
				{
					return false;
				}
			}

	  return true;
}

void CenterString(string str)
{
        int l = str.length();
        int pos = (int)((WindowWidth - l)/2);
        for (int i = 0; i < pos; ++i)
                cout << " ";
        cout << str;
}

void PrintScore()
{
        string text;
        GameObject current (0,0,' ');
        current.Color = ColorOfText;
        current.Draw(consoleHandle);
        text = "Highest Score This Session: " +  to_string(MaxScore) + "    Score: " + to_string(Score) + "  W: " + to_string(MoveThroughWalls);
        CenterString(text);
        current.Color = ColorOfText;
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

void SaveGame( vector<GameObject> *snake ,vector<GameObject> *fruit, vector<GameObject> *poison, unsigned int* score )
{
	ofstream o_SavedGameFile("saved game.txt");

	GameObject head = (*snake)[0]; //set 'head' equal to the first element of 'snake'
	GameObject tail = (*snake)[(*snake).size() - 1]; //set 'tail' equal to the last element of 'snake'
	GameObject normal_fruit = (*fruit)[0];
	GameObject pois_fruit = (*poison)[0];

	//save coordinates and score
        if (o_SavedGameFile.is_open())
        {
			o_SavedGameFile << head.Coordinates.X << " " << head.Coordinates.Y << "\n";
			o_SavedGameFile << tail.Coordinates.X << " " << tail.Coordinates.Y << "\n";
			o_SavedGameFile << normal_fruit.Coordinates.X << " " << normal_fruit.Coordinates.Y << "\n";
			o_SavedGameFile << pois_fruit.Coordinates.X << " " << pois_fruit.Coordinates.Y << "\n";
			o_SavedGameFile << *score << "\n";
			o_SavedGameFile.close();
		}
}

int GetScore() //reads score from scores.txt
{
        ifstream i_ScoreFile("scores.txt");
                int current_high_score = 0;
                if (i_ScoreFile.is_open())
                        {
                                i_ScoreFile >> current_high_score;
                        }
                return current_high_score;
}

void LoadGame()
{
	ifstream i_SavedGameFile("scores.txt");
	if (i_SavedGameFile.is_open())
	{
		//must add later
	}
}

void SetDifficulty()
{
        string border (WindowWidth, ':');
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //change color of border
        cout << border << endl;
        cout << border << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

        cout << "\n\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); //change color of text
                CenterString("Select Difficulty Level: \n\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                CenterString("1. Easy\n");
                CenterString("2. Normal\n");
        CenterString("3. Hard\n");
        CenterString("4. Suicide\n\n");

        CenterString("Press 'ESC' to go back\n\n\n");

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //change color of border
        cout << border << endl;
        cout << border << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);


        char k = getch();
        switch(k)
        {
        case '1':
                       MakeSound(neutral);
                        sleepDuration = 400;
                        ClearScreen(consoleHandle);
                        SetDifficulty();
                        break;
                case '2':
                        MakeSound(neutral);
                        sleepDuration = 200;
                        ClearScreen(consoleHandle);
                        SetDifficulty();
                        break;
                case '3':
                       MakeSound(neutral);
                        sleepDuration = 100;
                        ClearScreen(consoleHandle);
                        SetDifficulty();
                        break;
                case '4':
                        MakeSound(neutral);
                        sleepDuration = 50;
                        ClearScreen(consoleHandle);
                        SetDifficulty();
                        break;
                case ESC:
						ClearScreen(consoleHandle);
						 ChangeOptions();
                        break;
				default:
					break;
        }
        
}

void QuitGameOver()
{
		isPowerUpWalls = false;

        if (MaxScore < Score) 
                        {
                                MaxScore = Score;
                        }

		//before quitting the game, check if highest score should be updated
		UpdateHighestScore();


        MakeSound(sad);
        GameObject current (0,9,' ');
        current.Color = Red;
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
		tail.Color = ColorOfSnake;
        tail.Draw(consoleHandle);

        char k = getch();
        ClearScreen(consoleHandle);
        Menu();
}

char RandomizeFruitSymbol()
{
        char Symbol;
    char fruits[7];
    fruits[0]= '@';
    fruits[1]= '#';
    fruits[2]= '$';
    fruits[3]= '^';
    fruits[4]= '&';
    fruits[5]= '*';
    fruits[6]= '+';
    int p=0;
    srand(time(0));
    p = rand();
        int number;
        number = p % 7;
        char ChoseFruit;
        ChoseFruit = fruits[number];

                switch (ChoseFruit)
                {
                case '@':
                        Symbol = '@';
                                                ColorOfFruit = LightBlue;
                                                Points = 2;
                        break;
                case '#':
                        Symbol = '#';
                                                ColorOfFruit = Green;
                                                Points = 4;
                        break;
                case '$':
                        Symbol = '$';
                                                ColorOfFruit = Red;
                                                Points = 6;
                        break;
                case '^':
                        Symbol = '^';
                                                ColorOfFruit = Pink;
                                                Points = 8;
                        break;
                case '&':
                        Symbol = '&';
                                                ColorOfFruit = Cyan;
                                                Points = 10;
                        break;
                case '*':
                        Symbol = '*';
                                                ColorOfFruit = White;
                                                Points = 12;
                        break;
                case '+':
                        Symbol = '+';
                                                ColorOfFruit = Green;
                                                Points = 14;
                        break;
                };
                return Symbol;
}

void UpdateHighestScore()
{
	  int score = GetScore();
      if ( MaxScore > score )
      {
          SaveScore(MaxScore);
      }
}


void ChangeOptions()
{

	 string border (WindowWidth, ':');
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //change color of border
        cout << border << endl;
        cout << border << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

        cout << "\n\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); //change color of text
                CenterString("Options: \n\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                CenterString("1. Graphics\n");
                CenterString("2. Sound\n");
				CenterString("3. Difficulty Level\n\n");

        CenterString("Press 'ESC' to get back to the main menu\n\n\n");

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //change color of border
        cout << border << endl;
        cout << border << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);


        char k = getch();
        switch(k)
        {
        case '1':
			MakeSound(neutral);
            ClearScreen(consoleHandle);
			GraphicsMenu();
			break;
		case '2':
			MakeSound(neutral);
            ClearScreen(consoleHandle);
			SetSoundMenu();
			break;
		case '3':
                MakeSound(neutral);
                ClearScreen(consoleHandle);
                SetDifficulty();
                break;
		case ESC:
			MakeSound(neutral);
			ClearScreen(consoleHandle);
			Menu();
		default:
			break;
		}
}

void GraphicsMenu()
{
	string border (WindowWidth, ':');
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //change color of border
        cout << border << endl;
        cout << border << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

        cout << "\n\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); //change color of text
                CenterString("Change: \n\n");
				 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                CenterString("1. Snake Color\n");
                CenterString("2. Text Color\n");
				CenterString("3. Color of Walls\n");

        CenterString("Press 'ESC' to go back\n\n\n");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //change color of border
        cout << border << endl;
        cout << border << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);


        char k = getch();
        switch(k)
        {
        case '1':
			MakeSound(neutral);
            ClearScreen(consoleHandle);
			SetColorMenu( ColorOfSnake ) ;
			break;
		case '2':
			MakeSound(neutral);
            ClearScreen(consoleHandle);
			SetColorMenu( ColorOfText );
			break;
		case '3':
			MakeSound(neutral);
            ClearScreen(consoleHandle);
			SetColorMenu( ColorOfWalls );
			break;
		case ESC:
			MakeSound(neutral);
			ClearScreen(consoleHandle);
			ChangeOptions();
            break;
		default:
			break;
		}
}

void SetColorMenu(ConsoleColor& color)
{

	//Printing color options

	string border (WindowWidth, ':');
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //change color of border
        cout << border << endl;
        cout << border << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

        cout << "\n\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); //change color of text
                CenterString("Select Color: \n\n");
				 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                CenterString("1. Light Blue\n");
                CenterString("2. Green\n");
				CenterString("3. Red\n");
				CenterString("4. Yellow\n");
				CenterString("5. Pink\n");
				CenterString("6. Cyan\n");
				CenterString("7. White\n");

        CenterString("Press 'ESC' to go back\n\n\n");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //change color of border
        cout << border << endl;
        cout << border << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

		//changing color depending on option

		char k = getch();
        switch(k)
        {
        case '1':
			color = LightBlue;
			MakeSound(neutral);
            ClearScreen(consoleHandle);
			GraphicsMenu();
			break;
		case '2':
			color = Green;
			MakeSound(neutral);
            ClearScreen(consoleHandle);
			GraphicsMenu();
			break;
		case '3':
			color = Red;
			MakeSound(neutral);
            ClearScreen(consoleHandle);
			GraphicsMenu();
			break;
		case '4':
			color = Yellow;
			MakeSound(neutral);
            ClearScreen(consoleHandle);
			GraphicsMenu();
			break;
		case '5':
			color = Pink;
			MakeSound(neutral);
            ClearScreen(consoleHandle);
			GraphicsMenu();
			break;
		case '6':
			color = Cyan;
			MakeSound(neutral);
            ClearScreen(consoleHandle);
			GraphicsMenu();
			break;
		case '7':
			color = Cyan;
			MakeSound(neutral);
            ClearScreen(consoleHandle);
			GraphicsMenu();
			break;
		case ESC:
			MakeSound(neutral);
			ClearScreen(consoleHandle);
			GraphicsMenu();
            break;
		default:
			break;
	}
}

void SetSoundMenu()
{
	//Printing sound options

	string border (WindowWidth, ':');
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //change color of border
        cout << border << endl;
        cout << border << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

        cout << "\n\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); //change color of text
                CenterString("Sounds: \n\n");
				 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                CenterString("1. On\n");
                CenterString("2. Off\n");


        CenterString("Press 'ESC' to go back\n\n\n");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //change color of border
        cout << border << endl;
        cout << border << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

		//changing sound depending on option
		char k = getch();
        switch(k)
        {
        case '1':
			isSoundOn = true;
			MakeSound(neutral);
            ClearScreen(consoleHandle);
			ChangeOptions();
			break;
		case '2':
			isSoundOn = false;
			MakeSound(neutral);
            ClearScreen(consoleHandle);
			ChangeOptions();
		case ESC:
			MakeSound(neutral);
			ClearScreen(consoleHandle);
			ChangeOptions();
            break;
		default:
			break;
		}
}

void MakeSound( SoundMood mood )
{
   if ( isSoundOn )
	{
		switch(mood)
		{
		case happy:
			Beep(720,200);
			break;
		case neutral:
			Beep(420,200);
			break;
		case sad:
			Beep(220,200);
			break;
	}
  }
}
