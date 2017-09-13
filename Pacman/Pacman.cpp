#include "Pacman.h"
#include "Drawer.h"
#include "SDL.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <time.h>

#include "Avatar.h"
#include "World.h"
#include "GhostManager.h"
#include "globals.h"
#include "PathmapTile.h"

#ifdef _DEBUG
const char* gGameStateChar[] = {
	"GAME_STATE_INIT",
	"GAME_STATE_START",
	"GAME_STATE_PLAY",
	"GAME_STATE_DEATH_ANIMATION",
	"GAME_STATE_DEATH_END",
	"GAME_STATE_LOSE",
	"GAME_STATE_WIN"
};
#endif

Pacman* gPacman = NULL;

Pacman* Pacman::Create(Drawer* aDrawer)
{
	Pacman* pacman = new Pacman(aDrawer);

	if (!pacman->Init())
	{
		delete pacman;
		pacman = NULL;
	}

	gPacman = pacman;
	return pacman;
}

float Pacman::GetFrightenedGhostCounter()
{
	return myGhostGhostCounter;
}

Pacman::Pacman(Drawer* aDrawer)
: myDrawer(aDrawer)
, myNextMovement(-1.f,0.f)
, myScore(0)
, myFps(0)
, myLives(PACMAN_START_LIVES)
, myGhostGhostCounter(0.f)
, myTimerToNextState(0.f)
{
	myAvatar = new Avatar(Vector2f(13* TILE_SIZE,22* TILE_SIZE));
	myGhostManager = GhostManager::Create();
	myWorld = new World();

	myPacmanGameState.SetNextState(GAME_STATE_START, true);

	srand(time(NULL));
}

Pacman::~Pacman(void)
{
}

bool Pacman::Init()
{
	myWorld->Init();
	myAvatar->Init();

	return true;
}

bool Pacman::Update(float aTime)
{
	if (!UpdateInput())
		return false;

	myPacmanGameState.CheckAndSwapState();

	switch (myPacmanGameState.GetCurrentState())
	{
	case GAME_STATE_INIT:
		//initialize
		if (!myPacmanGameState.IsInitialized())
		{
			if (!myPacmanGameState.IsInSubState())
			{
				myPacmanGameState.SetSubState();
			}
			else
			{
				myPacmanGameState.SetInitalizingDone();
			}
		}
		//main
		else if (!myPacmanGameState.IsChangingState())
		{
			myWorld->ResetToOriginalDots();
			myLives = PACMAN_START_LIVES;
			myGhostManager->ResetCycleCount();
			myPacmanGameState.SetNextState(GAME_STATE_START, true);
		}
		//cleanup
		else if (!myPacmanGameState.IsCleanedUp())
		{
			if (!myPacmanGameState.IsInSubState())
			{
				myPacmanGameState.SetSubState();
			}
			else
			{
				myPacmanGameState.SetCleanUpDone();
			}
		}
		break;

	case GAME_STATE_START:
		//initialize
		if (!myPacmanGameState.IsInitialized())
		{
			if (!myPacmanGameState.IsInSubState())
			{
				myPacmanGameState.SetSubState();

				myGhostManager->RevertToPreviousState(); //so that we don't start in frighten state if avatar dies during ghost frighten state

				myTimerToNextState = 4.f;
				myAvatar->ResetAvatar();
				SetNextMovement(Vector2f(-1, 0));

				for (auto ghost : myGhostManager->GetVectorOfGhost())
				{
					ghost->ResetGhost();
				}

				for (auto ghost : myGhostManager->GetVectorOfHomeGhost())
				{
					ghost->ResetGhost();
				}
			}
			else
			{
				myPacmanGameState.SetInitalizingDone();
			}
		}

		//main
		else if (!myPacmanGameState.IsChangingState())
		{
			myTimerToNextState -= aTime;

			if (myTimerToNextState < 0)
			{
				myPacmanGameState.SetNextState(GAME_STATE_PLAY);
			}
		}

		//cleanup
		else if (!myPacmanGameState.IsCleanedUp())
		{
			if (!myPacmanGameState.IsInSubState())
			{
				myPacmanGameState.SetSubState();
			}
			else
			{
				myPacmanGameState.SetCleanUpDone();
			}
		}

		break;

	case GAME_STATE_PLAY:
		//initialize
		if (!myPacmanGameState.IsInitialized())
		{
			if (!myPacmanGameState.IsInSubState())
			{
				myPacmanGameState.SetSubState();
			}
			else
			{
				myPacmanGameState.SetInitalizingDone();
			}
		}

		//main
		else if (!myPacmanGameState.IsChangingState())
		{
			MoveAvatar();
			myAvatar->Update(aTime);
			myGhostManager->Update(aTime);

			if (myWorld->HasIntersectedDot(myAvatar->GetPosition()))
				myScore += 10;

			myGhostGhostCounter -= aTime;

			if (myWorld->HasIntersectedBigDot(myAvatar->GetPosition()))
			{
				myScore += 20;
				myGhostGhostCounter = (float)FRIGHTEN_DURATION;
				myGhostManager->SetGhostsNextState(GHOST_FRIGHTENED);
				myGhostCounterCountdown = true;
			}

			if (myGhostGhostCounter <= 0 && myGhostCounterCountdown)
			{
				myGhostManager->RevertToPreviousState();
				myGhostCounterCountdown = false;
			}

			for (auto ghost : myGhostManager->GetVectorOfGhost())
			{
				if ((ghost->GetPosition() - myAvatar->GetPosition()).Length() < 10.f)
				{
					if (ghost->GetCurrentState() != GHOST_FRIGHTENED && !ghost->myIsDeadFlag)
					{
						myPacmanGameState.SetNextState(GAME_STATE_LOSE_LIFE, true);
					}
					else if (ghost->GetCurrentState() == GHOST_FRIGHTENED && !ghost->myIsDeadFlag)
					{
						myScore += 50;
						ghost->myIsDeadFlag = true;
						ghost->Die(myWorld);
					}
				}
			}

			if (CheckEndGameCondition())
			{
				myPacmanGameState.SetNextState(GAME_STATE_WIN, true);
			}
			else if (myLives <= 0)
			{
				myPacmanGameState.SetNextState(GAME_STATE_LOSE, true);
			}
		}

		//cleanup
		else if (!myPacmanGameState.IsCleanedUp())
		{
			if (!myPacmanGameState.IsInSubState())
			{
				myPacmanGameState.SetSubState();
			}
			else
			{
				myPacmanGameState.SetCleanUpDone();
			}
		}

		break;

	case GAME_STATE_LOSE_LIFE:
		//initialize
		if (!myPacmanGameState.IsInitialized())
		{
			if (!myPacmanGameState.IsInSubState())
			{
				myPacmanGameState.SetSubState();
				myTimerToNextState = 2.f;
				for (auto ghost : myGhostManager->GetVectorOfGhost())
				{
					ghost->SetAlpha(0);
				}

				for (auto ghost : myGhostManager->GetVectorOfHomeGhost())
				{
					ghost->SetAlpha(0);
				}
			}
			else
			{
				myPacmanGameState.SetInitalizingDone();
			}
		}

		else if (!myPacmanGameState.IsChangingState())
		{
			myTimerToNextState -= aTime;
			if (myTimerToNextState < 0)
			{
				myLives--;
				if (myLives <= 0)
				{
					myPacmanGameState.SetNextState(GAME_STATE_LOSE, true);
				}
				else
				{
					myPacmanGameState.SetNextState(GAME_STATE_START, true);
				}
			}
			else
			{
				myAvatar->SetAlpha(255 / 2.f * myTimerToNextState);
			}

		}
		//cleanup
		else if (!myPacmanGameState.IsCleanedUp())
		{
			if (!myPacmanGameState.IsInSubState())
			{
				myPacmanGameState.SetSubState();			
			}
			else
			{
				myPacmanGameState.SetCleanUpDone();
			}
		}
		break;

	case GAME_STATE_LOSE:
		//initialize
		if (!myPacmanGameState.IsInitialized())
		{
			if (!myPacmanGameState.IsInSubState())
			{
				myPacmanGameState.SetSubState();
				myTimerToNextState = 4.f;
			}
			else
			{
				myPacmanGameState.SetInitalizingDone();
			}
		}

		else if (!myPacmanGameState.IsChangingState())
		{
			myTimerToNextState -= aTime;
			if (myTimerToNextState < 0)
				myPacmanGameState.SetNextState(GAME_STATE_INIT);
		}

		//cleanup
		else if (!myPacmanGameState.IsCleanedUp())
		{
			if (!myPacmanGameState.IsInSubState())
			{
				myPacmanGameState.SetSubState();
			}
			else
			{
				myPacmanGameState.SetCleanUpDone();
			}
		}

		break;

	case GAME_STATE_WIN:
		
		//initialize
		if (!myPacmanGameState.IsInitialized())
		{
			if (!myPacmanGameState.IsInSubState())
			{
				myPacmanGameState.SetSubState();
				myTimerToNextState = 4.f;
			}
			else
			{
				myPacmanGameState.SetInitalizingDone();
			}
		}

		else if (!myPacmanGameState.IsChangingState())
		{
			myTimerToNextState -= aTime;
			if (myTimerToNextState < 0)
				myPacmanGameState.SetNextState(GAME_STATE_INIT);
		}

		//cleanup
		else if (!myPacmanGameState.IsCleanedUp())
		{
			if (!myPacmanGameState.IsInSubState())
			{
				myPacmanGameState.SetSubState();
			}
			else
			{
				myPacmanGameState.SetCleanUpDone();
			}
		}

		break;

	default:
		break;
	}	

	if (aTime > 0)
		myFps = (int)(1 / aTime);


	return true;
}

bool Pacman::UpdateInput()
{
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_UP])
		myNextMovement = Vector2f(0.f, -1.f);
	else if (keystate[SDL_SCANCODE_DOWN])
		myNextMovement = Vector2f(0.f, 1.f);
	else if (keystate[SDL_SCANCODE_RIGHT])
		myNextMovement = Vector2f(1.f, 0.f);
	else if (keystate[SDL_SCANCODE_LEFT])
		myNextMovement = Vector2f(-1.f, 0.f);

	if (keystate[SDL_SCANCODE_ESCAPE])
		return false;

	return true;
}

void Pacman::MoveAvatar()
{
	if (myAvatar->IsAtDestination())
	{
		int currentTileX = myAvatar->GetCurrentTileX();
		int currentTileY = myAvatar->GetCurrentTileY();

		int nextTileX = currentTileX + myNextMovement.myX;
		int nextTileY = currentTileY + myNextMovement.myY;

		bool isTileValid = myWorld->TileIsValid(nextTileX, nextTileY);
		if (isTileValid)
		{
			myAvatar->SetNextTile(nextTileX, nextTileY);
		}
		else
		{
			//checks if destination is a loop tile
			PathmapTile* tile = myWorld->GetTile(currentTileX, currentTileY);
			bool isLoopTile = tile->myLoopFlag;

			if (isLoopTile)
			{
				//check if avatar wants to move out of the maze and not the walls
				if (myNextMovement.myY == 0)
				{
					//this is a loop tile, so send the avatar across the maze directly
					if (myNextMovement.myX == -1)
					{
						nextTileX = myWorld->GetMapColSize() - 1;
					}
					else
					{
						nextTileX = 0;
					}
					myAvatar->SetPosition(Vector2f(nextTileX * TILE_SIZE, nextTileY * TILE_SIZE));
					myAvatar->ResetTilesToCurrentPosition();
				}
			}
		}
	}
}

bool Pacman::CheckEndGameCondition()
{
	//game is won when all small and big dots dissapear
	if (myWorld->GetListOfDots().empty() &&
		myWorld->GetListOfBigDots().empty())
	{
		return true;
	}

	return false;
}

bool Pacman::Draw()
{
	myWorld->Draw(myDrawer);
	myAvatar->Draw(myDrawer);
	myGhostManager->Draw(myDrawer);

	std::string scoreString;
	std::stringstream scoreStream;
	scoreStream << myScore;
	scoreString = scoreStream.str();
	myDrawer->DrawText("Score", "freefont-ttf\\sfd\\FreeMono.ttf", 20, 50, 24, SDL_Color{ 255,0,0 });
	myDrawer->DrawText(scoreString.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", 90, 50, 24, SDL_Color{ 255,0,0 });

	std::string livesString;
	std::stringstream liveStream;
	liveStream << myLives;
	livesString = liveStream.str();
	myDrawer->DrawText("Lives", "freefont-ttf\\sfd\\FreeMono.ttf", 20, 80, 24, SDL_Color{ 255,0,0 });
	myDrawer->DrawText(livesString.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", 90, 80, 24, SDL_Color{ 255,0,0 });

	myDrawer->DrawText("FPS", "freefont-ttf\\sfd\\FreeMono.ttf", 880, 50, 24, SDL_Color{ 255,0,0 });
	std::string fpsString;
	std::stringstream fpsStream;
	fpsStream << myFps;
	fpsString = fpsStream.str();
	myDrawer->DrawText(fpsString.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", 930, 50, 24, SDL_Color{ 255,0,0 });

	if (myPacmanGameState.GetCurrentState() == GAME_STATE_START)
	{
		myDrawer->DrawText("GetReady!", "freefont-ttf\\sfd\\FreeMonoBold.ttf", 512, 425, 32, SDL_Color{ 255,255,255 }, true);
	}
	else if (myPacmanGameState.GetCurrentState() == GAME_STATE_WIN)
	{
		myDrawer->DrawText("You Win!", "freefont-ttf\\sfd\\FreeMonoBold.ttf", 512, 300, 100, SDL_Color{ 255,255,0 },true);
	}
	else if (myPacmanGameState.GetCurrentState() == GAME_STATE_LOSE)
	{
		myDrawer->DrawText("You lose!", "freefont-ttf\\sfd\\FreeMonoBold.ttf", 512, 300, 100, SDL_Color{ 255,129,0 }, true);
	}

#ifdef _DEBUG
	float timer = myTimerToNextState;
	std::string str = "GameState: " + std::string(gGameStateChar[myPacmanGameState.GetCurrentState()]);
	std::string str2 = "Time: " + std::to_string(timer);
	myDrawer->DrawText(str.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", 20, 350, 24, SDL_Color{ 0,255,0 });
	myDrawer->DrawText(str2.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", 20, 375, 24, SDL_Color{ 0,255,0 });
#endif

	return true;
}

void Pacman::SetNextMovement(Vector2f & vec)
{
	if (vec.Length() != 1)
		vec.Normalize();

	if (vec.myX == 0 || vec.myY == 0) // this is to ensure vectors aligned to the grid
	{
		myNextMovement = vec;
	}
}
