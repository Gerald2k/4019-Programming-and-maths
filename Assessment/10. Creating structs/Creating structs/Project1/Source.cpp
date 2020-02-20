#include <windows.h>
#include <chrono> //Time operations
#include <string>
#include "GameObjects.h"


// Console Parameters
#pragma region Console Buffer Params
//Window size coordinates, be sure to start at zero
SMALL_RECT windowSize{ 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

// A COORD strcut for specifying the console's screen buffer dimensions
COORD bufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
CONSOLE_SCREEN_BUFFER_INFO SBInfo;

// Setting up different variables for passing to WriteConsoleOutput
COORD characterBufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
COORD characterPosition = { 0,0 };
SMALL_RECT consoleWriteArea = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

// A CHAR_INFO structure containing data about our frame
CHAR_INFO consoleBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

// Initialise handles
HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE rHnd = GetStdHandle(STD_INPUT_HANDLE);
#pragma endregion

// Type defs
typedef std::chrono::steady_clock::time_point Time;
typedef std::chrono::high_resolution_clock HiResClock;
typedef std::chrono::duration<float> TimeDiff;

const int KEY_ENTER = VK_RETURN;
const int KEY_ESC = VK_ESCAPE;
const int KEY_W = 'W';
const int KEY_A = 'A';
const int KEY_S = 'S';
const int KEY_D = 'D';
const int KEY_1 = '1';
const int KEY_2 = '2';
const int KEY_3 = '3';
const int KEY_4 = '4';



// Enums
enum GAME_STATE
{
	SPLASH,
	MENU,
	PLAY,
	GAME_OVER,
	OPTIONS,
	SCOREBOARD,
};
// Game variables

bool exitGame = false;

GAME_STATE currentGameState = SPLASH;
Background background;
Player player;

// Functions
void Initialise();
void Update(float deltaTime);
void Draw();
int ClampInt(int intToClamp, int lowerLimit, int upperLimit);
void WriteImageToBuffer(CHAR_INFO* consoleBuffer, const int charsToPrint[], const int coloursToPrint[], const int imageHeight, const int imageWidth, int imageXPos, int imageYPos);
void ClearScreen(CHAR_INFO* consoleBuffer);
void WriteTextToBuffer(CHAR_INFO* consoleBuffer, std::string stringToPrint, int textXPos, int textYPos);
float ClampFloat(float floatToClamp, float lowerLimit, float upperLimit);

int main()
{
	// Initialise our console window
	Initialise();

	//Initilise variables
	float deltaTime = 0.0f;
	Time currentFrameTime = HiResClock::now();
	Time previousFrameTime = HiResClock::now();
	//Main game loop
	while (!exitGame)
	{
		//Calculate our delta time (time since last frame)
		currentFrameTime = HiResClock::now();
		TimeDiff diff = currentFrameTime - previousFrameTime;
		deltaTime = diff.count();
		//frameCounter++;

		if (deltaTime >= (1.0f / FRAME_RATE))
		{
			Update(deltaTime);
			// Cache the timestamp of this frame
			previousFrameTime = currentFrameTime;
		}
		Draw();
	}

	return 0;
}

void Initialise()
{
	// Set the console title
	SetConsoleTitle("LUNA LANDA");

	// Set screen buffer size											// Buffer is the RAM allocation alex
	SetConsoleScreenBufferSize(wHnd, bufferSize);						// Make an initilise function

	// Set the window size
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
}

void Update(float deltaTime)
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
			WriteImageToBuffer(consoleBuffer, splash.CHARACTERS, splash.COLOURS, splash.HEIGHT, splash.WIDTH, (SCREEN_WIDTH / 2) - (splash.WIDTH / 2), (SCREEN_HEIGHT / 2) - (splash.HEIGHT / 2));
			break;
		}
		else
		{
			splash.DURATION = 0.0f;
			currentGameState = PLAY;
		}

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

		if (GetAsyncKeyState(KEY_2))
		{
			exitGame = true;
		}

		break;
	}
	case PLAY:
	{
		if (!player.hasLanded && !player.hasCrashed)
		{
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
				player.Acceleration += (player.ACCELERATION_RATE * deltaTime);
			}
			else
			{
				player.Acceleration -= (player.DECELERATION_RATE * deltaTime);
			}

			// Reset acceleration flag
			player.isAccelerating = false;

			//Clamp our acceleration
			player.Acceleration = ClampFloat(player.Acceleration, 0.0f, 1.5f);

			if (player.Acceleration >= 1.0f)
			{
				player.YPos--;
			}
			else if (player.Acceleration < 0.5f)
			{
				player.YPos++;
			}
			// Clamp the input
			player.XPos = ClampInt(player.XPos, 0, (SCREEN_WIDTH - player.WIDTH));
			player.YPos = ClampInt(player.YPos, 0, (SCREEN_HEIGHT - player.HEIGHT));

			// Get the two characters under our landing gear
			char bottomLeftChar = background.CHARACTERS[player.XPos + SCREEN_WIDTH * (player.YPos + (player.HEIGHT - 1))];
			char bottomRightChar = background.CHARACTERS[(player.XPos + (player.WIDTH - 1)) + SCREEN_WIDTH * (player.YPos + (player.HEIGHT - 1))];

			// Land or Crash?
			if (bottomLeftChar == '_' && bottomRightChar == '_')
			{
				player.hasLanded = true;
				currentGameState = GAME_OVER;
			}
			else if (bottomLeftChar != ' ' || bottomRightChar != ' ')
			{
				player.hasCrashed = true;
				currentGameState = GAME_OVER;
			}
		}


		// Clear the previous frame before building the next
		ClearScreen(consoleBuffer);

		// Draw background image
		WriteImageToBuffer(consoleBuffer, background.CHARACTERS, nullptr, SCREEN_HEIGHT, SCREEN_WIDTH, 0, 0);

		// Draw player Image
		WriteImageToBuffer(consoleBuffer, player.CHARACTERS, player.COLOURS, player.HEIGHT, player.WIDTH, player.XPos, player.YPos);

		

		//Draw UI text
		WriteTextToBuffer(consoleBuffer, "SCORE: ", 1, 0);
		WriteTextToBuffer(consoleBuffer, "TIME: ", 1, 1);
		WriteTextToBuffer(consoleBuffer, "FUEL: ", 1, 2);

		break;
	}
	case GAME_OVER:
	{
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
			WriteTextToBuffer(consoleBuffer, "THE EAGLE HAS LANDED", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
			WriteTextToBuffer(consoleBuffer, "Press ENTER to return to menu!", SCREEN_WIDTH - 40, 1);
		}
		else if (player.hasCrashed)
		{
			WriteTextToBuffer(consoleBuffer, "fuuuuuuuck", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
			WriteTextToBuffer(consoleBuffer, "Press ENTER to return to menu!", SCREEN_WIDTH - 40, 1);
		}
		break;
	}
	case OPTIONS:
	{
		break;
	}
	case SCOREBOARD:
	{
		break;
	}
	}
}

void Draw()
{
	// Kick the draw
	WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
}

#pragma region UTILITY FUNCTIONS

int ClampInt(int intToClamp, int lowerLimit, int upperLimit)
{
	if (intToClamp <= lowerLimit)
	{
		return lowerLimit;
	}
	else if (intToClamp >= upperLimit)
	{
		return upperLimit;
	}
	else
	{
		return intToClamp;
	}
}

float ClampFloat(float floatToClamp, float lowerLimit, float upperLimit)
{
	return floatToClamp <= lowerLimit ? lowerLimit : floatToClamp >= upperLimit ? upperLimit : floatToClamp;
}

void WriteImageToBuffer(CHAR_INFO* consoleBuffer, const int charsToPrint[], const int coloursToPrint[], const int imageHeight, const int imageWidth, int imageXPos, int imageYPos)
{
	for (int y = 0; y < imageHeight; ++y)
	{
		for (int x = 0; x < imageWidth; ++x)
		{
			consoleBuffer[(imageXPos + x) + SCREEN_WIDTH * (imageYPos + y)].Char.AsciiChar = charsToPrint[x + imageWidth * y];
			// If colours array is given the it will set colours to that otherwise it will be white
			if (coloursToPrint)
			{
				consoleBuffer[(imageXPos + x) + SCREEN_WIDTH * (imageYPos + y)].Attributes = coloursToPrint[x + imageWidth * y];
			}
			else
			{
				consoleBuffer[(imageXPos + x) + SCREEN_WIDTH * (imageYPos + y)].Attributes = 7;
			}
		}
	}
}

void ClearScreen(CHAR_INFO* consoleBuffer)
{
	for (int i = 0; i < (SCREEN_WIDTH * SCREEN_HEIGHT); ++i)
	{
		consoleBuffer[i].Char.AsciiChar = 0;
		consoleBuffer[i].Attributes = 0;
	}
}

void WriteTextToBuffer(CHAR_INFO* consoleBugger, std::string stringToPrint, int textXPos, int textYPos)
{
	for (int x = 0; x < stringToPrint.length(); ++x)
	{
		consoleBuffer[(textXPos + x) + SCREEN_WIDTH * textYPos].Char.AsciiChar = stringToPrint[x];
		consoleBuffer[(textXPos + x) + SCREEN_WIDTH * textYPos].Attributes = 0xF;
	}
}
#pragma endregion