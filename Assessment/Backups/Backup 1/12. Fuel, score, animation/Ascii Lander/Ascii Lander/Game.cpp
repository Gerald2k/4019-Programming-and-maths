// This class' header
#include "Game.h"

// Includes
#include "Utility.h"
#include "Constants.h"
#include <iostream>
#include <sstream>

Game::Game()
{
	//Constructor
}

void Game::Initialise()
{
	// Set the console title
	SetConsoleTitle("LUNA LANDA");

	// Set screen buffer size											// Buffer is the RAM allocation alex
	SetConsoleScreenBufferSize(wHnd, bufferSize);						// Make an initilise function

	// Set the window size
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
}

void Game::Update(float deltaTime)
{
	static bool exitGame = false;

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
			// Draw background image
			//WriteImageToBuffer(consoleBuffer, background.CHARACTERS, nullptr, SCREEN_HEIGHT, SCREEN_WIDTH, 0, 0);
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

		//Print text options
		WriteTextToBuffer(consoleBuffer, "1. PLAY", (SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2));
		WriteTextToBuffer(consoleBuffer, "2. OPTIONS", (SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) + 1);
		WriteTextToBuffer(consoleBuffer, "3. SCOREBOARD", (SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) + 2);		//DO A HIGHLIGHT AND SELECT INSTEAD OF KEY NUM SELECTION
		WriteTextToBuffer(consoleBuffer, "4. QUIT", (SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) + 3);

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
			exitGame = true;
		}

		break;
	}
	case PLAY:
	{
		if (!player.hasLanded && !player.hasCrashed)
		{
			#pragma region
			// Take the input
			if (GetAsyncKeyState(KEY_ESC))
			{
				exitGame = true;
			}
			if (GetAsyncKeyState(KEY_W))
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
			if (player.isAccelerating)
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

			#pragma endregion Movement

			#pragma region
			// Get the two characters under our landing gear
			char bottomLeftChar = background.CHARACTERS[player.XPos + SCREEN_WIDTH * (player.YPos + (player.HEIGHT - 1))];
			char bottomRightChar = background.CHARACTERS[(player.XPos + (player.WIDTH - 1)) + SCREEN_WIDTH * (player.YPos + (player.HEIGHT - 1))];
			char scoreLeftChar = background.CHARACTERS[player.XPos + SCREEN_WIDTH * (player.YPos + (player.HEIGHT))];
			char scoreRightChar = background.CHARACTERS[(player.XPos + (player.WIDTH - 1)) + SCREEN_WIDTH * (player.YPos + (player.HEIGHT))];

			// Land or Crash?
			if (bottomLeftChar == '_' && bottomRightChar == '_')
			{
				player.hasLanded = true;

				#pragma region
				//Combining the characters beeneath to figure out the score value to be added
				int x = (int)scoreLeftChar - 48;
				int y = (int)scoreRightChar - 48;
				std::string scorenumstr = std::to_string(x) + std::to_string(y);
				int scorenumint;
				std::istringstream strtoint(scorenumstr);
				strtoint >> scorenumint;
				#pragma endregion Combining score characters

				// Add the score to the running total
				player.score = player.score + scorenumint;

				currentGameState = LANDED;
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

		//Score handling



		//Draw UI text
		WriteTextToBuffer(consoleBuffer, "SCORE: " + std::to_string(player.score), 1, 0);
		WriteTextToBuffer(consoleBuffer, "TIME: ", 1, 1);
		WriteTextToBuffer(consoleBuffer, "FUEL: "  + std::to_string(player.fuel), 1, 2);
		WriteTextToBuffer(consoleBuffer, "SPEED: " + std::to_string(player.Acceleration), 1, 3);
		

		break;
		#pragma endregion Clear And Draw
	}
	case GAME_OVER:
	{
		//Clear screen
		

		// Escape game over
		if (GetAsyncKeyState(KEY_ESC))
		{
			exitGame = true;
		}
		if (GetAsyncKeyState(KEY_ENTER))
		{
			player.Reset();
			currentGameState = MENU;
		}

		// Draw game over text
		if (player.hasLanded)
		{
			WriteTextToBuffer(consoleBuffer, "YOU SCORED AN EXTRA: ", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 10);
			WriteTextToBuffer(consoleBuffer, "Press ENTER to return to menu!", SCREEN_WIDTH - 40, 1);
		}
		else if (player.hasCrashed)
		{
			ClearScreen(consoleBuffer);
			WriteTextToBuffer(consoleBuffer, "GAME OVER!", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 10);
			WriteTextToBuffer(consoleBuffer, "Press ENTER to return to menu!", SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 11);
		}
		break;
	}
	case OPTIONS:
	{
		if (GetAsyncKeyState(KEY_ESC) || GetAsyncKeyState(KEY_ENTER))
		{
			currentGameState = MENU;
		}
		break;

		if (GetAsyncKeyState(KEY_1))
		{
			player.colourNum = NULL;
		}

		if (GetAsyncKeyState(KEY_2))
		{
			player.colourNum = 2;
		}

		if (GetAsyncKeyState(KEY_3))
		{
			player.colourNum = 3;
		}
	}
	case SCOREBOARD:
	{
		break;
	}
	case LANDED:
	{
		if (GetAsyncKeyState(KEY_W))
		{
			player.hasLanded = false;
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