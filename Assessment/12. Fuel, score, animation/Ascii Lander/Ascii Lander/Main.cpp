// Main control file that calls and manages the main loops

#include <windows.h>
#include <chrono> //Time operations
#include "GameObjects.h"
#include "Game.h"
#include "Utility.h"


int main()
{
	Game gameInstance;
	// Initialise our console window
	gameInstance.Initialise();

	//Initilise variables
	float deltaTime = 0.0f;
	Time currentFrameTime = HiResClock::now();
	Time previousFrameTime = HiResClock::now();
	int frameCount = 0;
	static bool exitGame = false;

	//Main game loop
	while(!exitGame)
	{
		//Calculate our delta time (time since last frame)
		currentFrameTime = HiResClock::now();
		TimeDiff diff = currentFrameTime - previousFrameTime;
		deltaTime = diff.count();

		if (deltaTime >= (1.0f / FRAME_RATE))
		{
			// Update our application
			++frameCount;
			gameInstance.Update(deltaTime);

			// Cache the timestamp of this frame
			previousFrameTime = currentFrameTime;
		}
		gameInstance.Draw();
	}
	return 0;
}
