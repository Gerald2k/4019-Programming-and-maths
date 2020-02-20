#include <windows.h>
#include <chrono> //Time operations

//Defines
#define WIDTH 150
#define HEIGHT 40
#define FRAME_RATE 30

// Console Parameters
#pragma region Console Buffer Params
//Window size coordinates, be sure to start at zero
SMALL_RECT windowSize{ 0, 0, WIDTH - 1, HEIGHT - 1 };

// A COORD strcut for specifying the console's screen buffer dimensions
COORD bufferSize = { WIDTH, HEIGHT };
CONSOLE_SCREEN_BUFFER_INFO SBInfo;

// Setting up different variables for passing to WriteConsoleOutput
COORD characterBufferSize = { WIDTH, HEIGHT };
COORD characterPosition = { 0,0 };
SMALL_RECT consoleWrtieArea = { 0, 0, WIDTH - 1, HEIGHT - 1 };

// A CHAR_INFO structure containing data about our frame
CHAR_INFO consoleBuffer[WIDTH * HEIGHT];

// Initialise handles
HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE rHnd = GetStdHandle(STD_INPUT_HANDLE);
#pragma endregion

// Type defs
typedef std::chrono::steady_clock::time_point Time;
typedef std::chrono::high_resolution_clock HiResClock;
typedef std::chrono::duration<float> TimeDiff;

// Game constants
const int playerCharacter = '@';
const int playerColour = 7;

const int KeyEsc = VK_ESCAPE;
const int KeyW = 'W';
const int KeyA = 'A';
const int KeyS = 'S';
const int KeyD = 'D';

// Game variables
unsigned int playerXPos = 0;
unsigned int playerYPos = 0;
bool exitGame = false;
float deltaTime = 0.0f;
unsigned int frameCounter = 0;
Time previousFrameTime = HiResClock::now();



int main() 
{
	// Set the console title
	SetConsoleTitle("LUNA LANDA");

	// Set screen buffer size							// Buffer is the RAM allocation alex
	SetConsoleScreenBufferSize(wHnd, bufferSize);

	// Set the window size
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

	

	//Main game loop
	
	while (!exitGame) 
	{
		//Calculate our delta time (time since last frame)
		Time currentFrameTime = HiResClock::now();
		TimeDiff diff = currentFrameTime - previousFrameTime;
		deltaTime = diff.count();
		frameCounter++;

		if (deltaTime >= (1.0f / FRAME_RATE)) 
		{
			// Cache the timestamp of this frame
			previousFrameTime = currentFrameTime;
			if (GetAsyncKeyState(KeyEsc))
			{
				exitGame = true;
			}

			if (GetAsyncKeyState(KeyW))
			{
				--playerYPos;
			}

			if (GetAsyncKeyState(KeyA))
			{
				--playerXPos;
			}

			if (GetAsyncKeyState(KeyS))
			{
				++playerYPos;
			}

			if (GetAsyncKeyState(KeyD))
			{
				++playerXPos;
			}

			for (int i = 0; i < (WIDTH * HEIGHT); ++i)
			{
				consoleBuffer[i].Char.AsciiChar = 0;
				consoleBuffer[i].Attributes = 0;
			}

			// Set the ASCII character and the colour for our player
			consoleBuffer[playerXPos + WIDTH * playerYPos].Char.AsciiChar = playerCharacter;
			consoleBuffer[playerXPos + WIDTH * playerYPos].Attributes = playerColour;

			WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, characterPosition, &consoleWrtieArea);
		}
		
	}

	return 0;
}
