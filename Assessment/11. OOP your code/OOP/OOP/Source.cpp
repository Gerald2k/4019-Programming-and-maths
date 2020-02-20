#include <windows.h>
#include <chrono> //Time operations
#include "GameObjects.h"
#include "Game.h"

// Type defs
typedef std::chrono::steady_clock::time_point Time;
typedef std::chrono::high_resolution_clock HiResClock;
typedef std::chrono::duration<float> TimeDiff;

int main()
{
	Game gameInstance;
	// Initialise our console window
	gameInstance.Initialise();

	//Initilise variables
	float deltaTime = 0.0f;
	Time currentFrameTime = HiResClock::now();
	Time previousFrameTime = HiResClock::now();
	//Main game loop
	bool exitGame = false;
	while (!exitGame)
	{
		//Calculate our delta time (time since last frame)
		currentFrameTime = HiResClock::now();
		TimeDiff diff = currentFrameTime - previousFrameTime;
		deltaTime = diff.count();
		//frameCounter++;

		if (deltaTime >= (1.0f / FRAME_RATE))
		{
			// Update our application
			gameInstance.Update(deltaTime);

			// Cache the timestamp of this frame
			previousFrameTime = currentFrameTime;
		}
		gameInstance.Draw();
	}
	return 0;
}
