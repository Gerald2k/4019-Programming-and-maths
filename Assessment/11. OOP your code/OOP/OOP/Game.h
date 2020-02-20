#ifndef GAME_H
#define GAME_H

// Includes
#include <Windows.h>
#include "GameObjects.h"

class Game
{
public:
	// Functions
	Game();
	void Initialise();
	void Update(float deltaTime);
	void Draw();

private:
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
	//Window size coordinates, be sure to start at zero
	SMALL_RECT windowSize{ 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

	// A COORD strcut for specifying the console's screen buffer dimensions
	COORD bufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };

	// Setting up different variables for passing to WriteConsoleOutput
	COORD characterBufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	COORD characterPosition = { 0,0 };
	SMALL_RECT consoleWriteArea = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

	// A CHAR_INFO structure containing data about our frame
	CHAR_INFO consoleBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

	// Initialise handles
	HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE rHnd = GetStdHandle(STD_INPUT_HANDLE);

	// Game variables
	GAME_STATE currentGameState = SPLASH;
	Background background;
	Player player;

};
#endif
