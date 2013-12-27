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
        WallSymbol = 'X',
        PoisonSymbol = '-';
char FruitSymbol;
int Points;
ConsoleColor ColorOfFruit;

// Game variables
unsigned long sleepDuration = 200;
const int ESC = 27;
vector<GameObject> snake;
vector<GameObject> fruit;
vector<GameObject> poison;
vector<GameObject> wall;

unsigned int Score = 0;
unsigned int MaxScore = 0;
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
        CenterString("3. See Highest Score\n");
                CenterString("4. Difficulty Settings\n");
        CenterString("5. Exit Game\n");

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
             //play new game
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
             //see instructions
                Beep(520,200);
                ClearScreen(consoleHandle);
                GameInstructions();
                break;
        case '3': 
                        //see highest score
                                Beep(520,200);
                ClearScreen(consoleHandle);
                DisplayHighestScore();
                break;
		case '4':
                Beep(520,200);
                ClearScreen(consoleHandle);
                SetDifficulty();
                break;
        case '5': 
                        //quit game
                Beep(520,200);
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
                                                        i->Color = Green;
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

                for (const_iterator brick = wall.begin(); brick != wall.end(); ++brick)
        {
                brick->Draw(consoleHandle);
        }
}

void Update()
{
        // Save the tail, we might need it later.
				GameObject tail = *(snake.end() - 1);
                tail.Symbol = ' ';
                                tail.Color = Green;

        for (randomAccess_iterator i = snake.end() - 1; i != snake.begin(); --i)
        {
                GameObject next = *(i - 1);
                                i->Color = Green;
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
                       UpdateHighestScore();
                        ClearScreen(consoleHandle);
                        Menu();
                        break;
					case 'k':
						SaveGame( &snake , &fruit, &poison, &Score );
						break;
                };
        }

        snake.begin()->Coordinates.X += direction.X;
        snake.begin()->Coordinates.Y += direction.Y;

				GameObject head = *snake.begin();
                head.Color = Green;

                head.Draw(consoleHandle);
                tail.Draw(consoleHandle);


                for (randomAccess_iterator i = fruit.begin(); i != fruit.end(); ++i)
        {
                if (head.Coordinates.X == i->Coordinates.X && head.Coordinates.Y == i->Coordinates.Y)
                {
                        // Remove the old fruit, increase the snake's size
                        Score += Points;
                        PrintScore();

                        fruit.erase(i);

                        snake.push_back(tail);

                        tail.Symbol = '*';
                        tail.Color = Green;
                        tail.Draw(consoleHandle);
                        Beep(700,850);

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

                                 

                //if the snake touches one of the borders
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

COORD GeneratingCoordinations ()
{
        COORD momental;
        momental.X = rand() % (WindowWidth-2*(BorderX+1))+BorderX+1;
        momental.Y = rand() % (WindowHeight-2*(BorderY+1))+BorderY+1;
        return momental;
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
        current.Color = Green;
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

        CenterString("Press 'ESC' to get back to the main menu\n\n\n");

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //change color of border
        cout << border << endl;
        cout << border << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);


        char k = getch();
        switch(k)
        {
        case '1':
                        Beep(520,200);
                        sleepDuration = 400;
                        ClearScreen(consoleHandle);
                        SetDifficulty();
                        break;
                case '2':
                        Beep(520,200);
                        sleepDuration = 200;
                        ClearScreen(consoleHandle);
                        SetDifficulty();
                        break;
                case '3':
                        Beep(520,200);
                        sleepDuration = 100;
                        ClearScreen(consoleHandle);
                        SetDifficulty();
                        break;
                case '4':
                        Beep(520,200);
                        sleepDuration = 50;
                        ClearScreen(consoleHandle);
                        SetDifficulty();
                        break;
                case 27:
						ClearScreen(consoleHandle);
						 Menu();
                        break;
        }
        
}

void QuitGameOver()
{
        if (MaxScore < Score) 
                        {
                                MaxScore = Score;
                        }
		//before quitting the game, check if highest score should be updated
		UpdateHighestScore();
                Beep(520,2000);
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
