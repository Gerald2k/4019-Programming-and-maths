// This class' header
#include "Game.h"

// Includes
#include "Utility.h"
#include "Constants.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>

void Game::Initialise()
{
	// Set the console title
	SetConsoleTitle("LUNAR LANDER");

	// Set screen buffer size											
	SetConsoleScreenBufferSize(wHnd, bufferSize);						

	// Set the window size
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
}

void Game::Update(float deltaTime)
{
	switch (currentGameState)
	{
	case SPLASH:
	{
		// Create new splash screen
		static Splash splash;

		// Calculate current splash duration
		splash.DURATION += deltaTime;

		if (splash.DURATION < 3.0f)
		{
			// Draw splash image
			WriteImageToBuffer(consoleBuffer, splash.CHARACTERS, splash.COLOURS, SCREEN_HEIGHT, SCREEN_WIDTH, 0, 0);
		}
		else
		{
			splash.DURATION = 0.0f;
			currentGameState = MENU;
		}

		break;
	}
	case MENU:
	{
		// Clear previous
		ClearScreen(consoleBuffer);

		WriteImageToBuffer(consoleBuffer, background.MENUCHARACTERS, nullptr, SCREEN_HEIGHT, SCREEN_WIDTH, 0, 0);

		if (GetAsyncKeyState(KEY_1))
		{
			currentGameState = PLAY;
		}
		if (GetAsyncKeyState(KEY_2))
		{
			currentGameState = OPTIONS;
		}
		if (GetAsyncKeyState(KEY_3))
		{
			currentGameState = SCOREBOARD;
		}
		if (GetAsyncKeyState(KEY_4))
		{
			exit(0);
		}

		break;
	}

	case PLAY:
	{
		if (!player.hasLanded && !player.hasCrashed)
		{
			static bool outOfFuel = false;
			if (player.fuel <= 0)
			{
				bool outOfFuel = true;
				player.Acceleration -= (ACCELERATION_RATE * deltaTime);
			}
			#pragma region
			// Take the input
			if (GetAsyncKeyState(KEY_ESC))
			{
				exit(0);
			}
			if (GetAsyncKeyState(KEY_W) && outOfFuel == false)
			{
				player.isAccelerating = true;
			}
			if (GetAsyncKeyState(KEY_A))
			{
				--player.XPos;
			}
			if (GetAsyncKeyState(KEY_D))
			{
				++player.XPos;
			}

			// Acceleration
			if (player.isAccelerating && outOfFuel == false)
			{
				player.Acceleration += (ACCELERATION_RATE * deltaTime);
				--player.fuel;
			}
			else
			{
				player.Acceleration -= (DECELERATION_RATE * deltaTime);
			}

			// Reset acceleration flag
			player.isAccelerating = false;

			//Clamp our acceleration
			player.Acceleration = ClampFloat(player.Acceleration, 0.0f, 0.8f);

			if (player.Acceleration >= 0.4f)
			{
				player.YPos--;
			}
			else if (player.Acceleration < 0.3f)
			{
				player.YPos++;
			}

			// Clamp the input
			player.XPos = ClampInt(player.XPos, 0, (SCREEN_WIDTH - player.WIDTH));
			player.YPos = ClampInt(player.YPos, 0, (SCREEN_HEIGHT - player.HEIGHT));
			player.fuel = ClampInt(player.fuel, 0, 100);

			#pragma endregion Movement

			#pragma region
				// Get the two characters under our landing gear
				char bottomLeftChar = background.CHARACTERS[player.XPos + SCREEN_WIDTH * (player.YPos + (player.HEIGHT - 1))];
				char bottomRightChar = background.CHARACTERS[(player.XPos + (player.WIDTH - 1)) + SCREEN_WIDTH * (player.YPos + (player.HEIGHT - 1))];
				char underLeftChar = background.CHARACTERS[player.XPos + SCREEN_WIDTH * (player.YPos + (player.HEIGHT))];
				char underRightChar = background.CHARACTERS[(player.XPos + (player.WIDTH - 1)) + SCREEN_WIDTH * (player.YPos + (player.HEIGHT))];

				// Land or Crash?
				if (bottomLeftChar == '_' && bottomRightChar == '_' && player.Acceleration > 0.10f)
				{
					player.hasLanded = true;
					currentGameState = LANDED;

				#pragma region
				//Combining the characters beneath to figure out the score value to be added
								int x = (int)underLeftChar - 48;
								int y = (int)underRightChar - 48;
								std::string scorenumstr = std::to_string(x) + std::to_string(y);
								int scoreNumInt;
								std::istringstream strtoint(scorenumstr);
								strtoint >> scoreNumInt;
				#pragma endregion Combining score characters

				#pragma region

				// First land handling
				static int prevScore = 0;
				static bool firstLand = true;
				if (firstLand == true && underLeftChar == 'H' || underLeftChar == 'O' || underLeftChar == 'M') // First land is home
				{
					firstLand = false;
					currentGameState = GAME_OVER;
				}
				else if (firstLand == true)
				{
					firstLand = false;
					player.score = player.score + scoreNumInt;
					prevScore = scoreNumInt;
					currentGameState = LANDED;
				}

				if (prevScore != scoreNumInt && underLeftChar != 79 && underLeftChar != 77 && underLeftChar != 72)
				{
					prevScore = scoreNumInt;
					player.score = player.score + scoreNumInt;
				}
				#pragma endregion Score handling

					if (underLeftChar == 'H' || underLeftChar == 'O' || underLeftChar == 'M')
					{
						player.finalLanding = true;
						player.score = player.score + player.fuel * 0.8;
						currentGameState = GAME_OVER;

					}
				}
				else if (bottomLeftChar != ' ' || bottomRightChar != ' ')
				{
					player.hasCrashed = true;
					currentGameState = GAME_OVER;
				}
				#pragma endregion Collision Detection
		}

		#pragma region
		// Clear the previous frame before building the next
		ClearScreen(consoleBuffer);

		// Draw background image
		WriteImageToBuffer(consoleBuffer, background.CHARACTERS, nullptr, SCREEN_HEIGHT, SCREEN_WIDTH, 0, 0);

		// Draw player Image
		WriteImageToBuffer(consoleBuffer, player.CHARACTERS, player.COLOURS, player.HEIGHT, player.WIDTH, player.XPos, player.YPos);

		WriteTextToBuffer(consoleBuffer, "ACCELERATION SPEED: " + std::to_string(player.Acceleration), 1, 0);
		WriteTextToBuffer(consoleBuffer, "SCORE: " + std::to_string(player.score), 1, 1);
		WriteTextToBuffer(consoleBuffer, "FUEL: " + std::to_string(player.fuel), 1, 2);

		break;
		#pragma endregion Clear And Draw

	}
	case GAME_OVER:
	{
		// Escape game over
		if (GetAsyncKeyState(KEY_ESC))
		{
			exit(0);
		}
		if (GetAsyncKeyState(KEY_ENTER))
		{
			player.Reset();
			currentGameState = MENU;
		}

		// Draw game over text
		if (player.finalLanding)
		{
			ClearScreen(consoleBuffer);
			WriteTextToBuffer(consoleBuffer, "YOU GOT HOME!", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 10);
			WriteTextToBuffer(consoleBuffer, "FINAL SCORE: " + std::to_string(player.score), SCREEN_WIDTH / 2 - 2, SCREEN_HEIGHT / 2 + 11);

		}
		else if (player.hasCrashed)
		{
			ClearScreen(consoleBuffer);
			WriteTextToBuffer(consoleBuffer, "YOU DIED!", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 10);
			WriteTextToBuffer(consoleBuffer, "FINAL SCORE: " + std::to_string(player.score), SCREEN_WIDTH / 2 - 2, SCREEN_HEIGHT / 2 + 11);
			WriteTextToBuffer(consoleBuffer, "Press ENTER to return to menu!", SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 12);
		}
		break;
	}
	case OPTIONS:
	{
		ClearScreen(consoleBuffer);
		WriteImageToBuffer(consoleBuffer, options.CHARACTERS, nullptr, SCREEN_HEIGHT, SCREEN_WIDTH, 0, 0);
		WriteTextToBuffer(consoleBuffer, "In this game, you must try and score as many points as you can before you run out of fuel.", SCREEN_WIDTH / 2 - 45, SCREEN_HEIGHT / 2 - 8);
		WriteTextToBuffer(consoleBuffer, "Dont land too fast though! You will crash if you dont land slow enough or if you collide with the mountain side.", SCREEN_WIDTH / 2 - 55, SCREEN_HEIGHT / 2 - 5);
		WriteTextToBuffer(consoleBuffer, "You will see the score for each landing pad, however you cant land on the same one twice in a row.", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 2);
		WriteTextToBuffer(consoleBuffer, "Conserve your fuel, as if you land on home to finish your game, you will get extra points!", SCREEN_WIDTH / 2 - 45, SCREEN_HEIGHT / 2 + 1);

		if (GetAsyncKeyState(KEY_ESC) || GetAsyncKeyState(KEY_ENTER))
		{
			currentGameState = MENU;
		}
		break;
	}
	case SCOREBOARD:
	{
		// Clear screen
		ClearScreen(consoleBuffer);
		WriteTextToBuffer(consoleBuffer, "Press ESC to go back to menu. " , SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 15);
		std::string str;
		std::string h_score = "hello";
		std::fstream scoreBoardFile("ScoreBoard.txt");
		scoreBoardFile.open("ScoreBoard.txt");
		if (scoreBoardFile.is_open()) 
		{ /* ok, proceed with output */ 
			scoreBoardFile << "Writing this to a file.\n";
			scoreBoardFile << h_score;
			//std::getline(scoreBoardFile, str);
			WriteTextToBuffer(consoleBuffer, "Highscore: " + str, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 10);
			scoreBoardFile.close();
		}
		
		//scoreBoardFile.open("ScoreBoard.txt");
		//scoreBoardFile >> o_score;
		
		
		//for h_Score
		/*if (myfile.is_open())
		{
			myfile << "This is a line.\n";
			myfile << "This is another line. Score: " + player.score;
			myfile.close();
		}
		else std::cout << "Can't open file";*/

		// Print out the leaderboard
		/*for (int i = 0; i < 10; i++)
		{
			std::cout << scoreboard.h_ScoreNums[i];
			std::cout << scoreboard.h_ScoreNames[i];


		if (GetAsyncKeyState(KEY_ESC))
		{
			currentGameState = MENU;
		}
		// If end game score is bigger than the current looked at point then increase pointer on the scoreboard list
		for (int i = 0; i < 10; i++)
		{
			if (player.score <= h_Score + "h_ScorePointer")
			{
				// Replace current score
				temp = h_Score + "h_ScorePointer";
				h_Score + "h_ScorePointer" = player.score;

				if (h_Score + "h_ScorePointer" < 10)
				// Move the previous high score and any others than also need moving down
				for (int i = 0; i < 10; i++)
				{
					temp = h_Score + "h_ScorePointer";
					h_Score + "h_ScorePointer" = player.score;
				}
			}
			else
			{
				//LET LOOP INCREMENT
			}*/
		if (GetAsyncKeyState(KEY_ESC) || GetAsyncKeyState(KEY_ENTER))
		{
			currentGameState = MENU;
		}
	}
	break;

	case LANDED:
	{
		if (GetAsyncKeyState(KEY_W))
		{
			player.hasLanded = false;
			(player.fuel + 3);
			player.Acceleration = 0.8f;
			currentGameState = PLAY;
		}
		break;
	}
	}
}

void Game::Draw()
{
	// Kick the draw
	WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
}