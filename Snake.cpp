#include <iostream>
#include <vector>
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
const int WindowHeight = 25;
//Растоянията от краищата на прозореца до рамката на игралното поле
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

int Score;
int MaxScore = 0;
//За определяне на къде ще върви змията
COORD direction ;

void Initialization();
void CenterString(string s);
void DisplayHighestScore();
void PrintScore();
COORD GanaratingCoordinations();
void GeneratingWall ();
void Draw();
void Update();
void Menu();
void GameInstructions();
void QuitGameOver();
int  QuitGame();

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
//				DrawWall();
				Initialization();
				Draw();
				while (true)
                        {
                                Update();
//                                Draw();
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

//	Initialization ();	  
//        for (int i = SnakeStartingLength-1; i > -1; --i)
//        {
//			snake.push_back(GameObject(i+BorderX+1, 3, SnakeSymbol));
//        }
//
//		GeneratingWall (WindowWidth, WindowHeight);

		COORD CurrentCoordinates = GanaratingCoordinations ();
	    //int x1 = rand() % WindowWidth;
        //int y1 = rand() % WindowHeight;
		fruit.push_back(GameObject(CurrentCoordinates.X, CurrentCoordinates.Y, FruitSymbol));

		CurrentCoordinates = GanaratingCoordinations ();
		//int x2 = rand() % WindowWidth;
		//int y2 = rand() % WindowHeight;
		poison.push_back(GameObject(CurrentCoordinates.X, CurrentCoordinates.Y, PoisonSymbol));
		
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

void QuitGameOver()
{
	if (MaxScore<Score) MaxScore=Score;

	GameObject current (0,9,' ');
//	i->Coordinates.X = 0;
//	i->Coordinates.Y = 9;
//	i->Symbol = ' ';
	current.Color = 0x4;
	current.Draw(consoleHandle);

	//ClearScreen(consoleHandle);
	cout<<"\n";
	cout<<" #     #                  #                            \n";
	cout<<"  #   #   ####  #    #    #        ####   ####  ###### \n";
	cout<<"   # #   #    # #    #    #       #    # #      #      \n";
	cout<<"    #    #    # #    #    #       #    #  ####  #####  \n";
	cout<<"    #    #    # #    #    #       #    #      # #      \n";
	cout<<"    #    #    # #    #    #       #    # #    # #      \n";
	cout<<"    #     ####   ####     #######  ####   ####  ###### \n";
	
	GameObject tail = *(snake.end() - 1);
//	tail.Color = 0x1;
	tail.Draw(consoleHandle);

	char k = getch();
	//system ("pause");
	ClearScreen(consoleHandle);
	Menu();
}

int QuitGame() //Trqbva da se dobavi maxscore
{
	ClearScreen(consoleHandle);
	//cout << "\n\n\n\n";
	//CenterString("You Lose!\n\n");
	
    return 0;
}

COORD GanaratingCoordinations ()
{
	COORD momental;
	momental.X = rand() % (WindowWidth-2*(BorderX+1))+BorderX+1;
	momental.Y = rand() % (WindowHeight-2*(BorderY+1))+BorderY+1;
	return momental;
}

void GeneratingWall ()
{
	for (int x = BorderX; x <= WindowWidth-BorderX; ++x)
        {
                wall.push_back(GameObject(x, BorderY, WallSymbol));
				wall.push_back(GameObject(x, WindowHeight-BorderY, WallSymbol));
        }
	for (int y = BorderY; y <= WindowHeight-BorderY; ++y)
        {
                wall.push_back(GameObject(BorderX, y, WallSymbol));
				wall.push_back(GameObject(WindowWidth-BorderX, y, WallSymbol));
        }
}

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
						Score -= 3;
						PrintScore();

                        poison.erase(i);

						tail = *(snake.end() - 1);
						tail.Symbol = ' ';
						tail.Draw(consoleHandle);

                        snake.erase(snake.end() - 1);

                        // Add a new poison						
						COORD CurrentCoordinates = GanaratingCoordinations ();
						tail = GameObject(CurrentCoordinates.X, CurrentCoordinates.Y, PoisonSymbol);
						poison.push_back(tail);
						tail.Draw(consoleHandle);

                        sleepDuration *= (sleepDuration > 50) * 0.1 + 0.9;
                        
						// Break, since you can't eat more than one fruit at the same time.
                        break;
					}
					else
					{
						QuitGameOver();
					}
                }

        }

		//if the snake touch the border
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

		//if the snake touch the snake
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

void Initialization ()
{
	Score = 0;
	direction.X = 1;
	direction.Y = 0;

	while(snake.begin() != snake.end())
		snake.erase(snake.end() - 1);

	for (int i = SnakeStartingLength-1; i > -1; --i)
        {
			snake.push_back(GameObject(i+BorderX+1, BorderY+1, SnakeSymbol));
        }

		GeneratingWall ();
}

void PrintScore()
{
	string text;
	GameObject current (0,0,' ');
	current.Color = 0x2;
	current.Draw(consoleHandle);
	text = "Max Score: " +  to_string(MaxScore) + "        Score: " + to_string(Score);
	CenterString(text);
	current.Color = 15;
}

void Draw()
{
		PrintScore();
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