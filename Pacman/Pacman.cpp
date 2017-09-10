#include "Pacman.h"
#include "Drawer.h"
#include "SDL.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Avatar.h"
#include "World.h"
#include "Ghost.h"

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

Pacman::Pacman(Drawer* aDrawer)
: myDrawer(aDrawer)
, myTimeToNextUpdate(0.f)
, myNextMovement(-1.f,0.f)
, myScore(0)
, myFps(0)
, myLives(3)
, myGhostGhostCounter(0.f)
, myCurrentGameState(GAME_STATE_INIT)
, myNextGameState(GAME_STATE_INIT)
{
	myAvatar = new Avatar(Vector2f(13*22,22*22));
	//myGhost = new Ghost(Vector2f(13*22,13*22));
	myGhost = new Ghost(Vector2f(10*22,12*22));
	myWorld = new World();
}

Pacman::~Pacman(void)
{
}

bool Pacman::Init()
{
	myWorld->Init();
	myGhost->Init();
	myAvatar->Init();

	return true;
}

bool Pacman::Update(float aTime)
{
	if (!UpdateInput())
		return false;

	myCurrentGameState = myNextGameState;

	switch (myCurrentGameState)
	{
	case GAME_STATE_INIT:
		myNextGameState = GAME_STATE_PLAY;
		break;

	case GAME_STATE_START:
		break;

	case GAME_STATE_PLAY:
		MoveAvatar();
		myAvatar->Update(aTime);
		myGhost->Update(aTime, myWorld);

		if (myWorld->HasIntersectedDot(myAvatar->GetPosition()))
			myScore += 10;

		myGhostGhostCounter -= aTime;

		if (myWorld->HasIntersectedBigDot(myAvatar->GetPosition()))
		{
			myScore += 20;
			myGhostGhostCounter = 20.f;
			myGhost->myIsClaimableFlag = true;
		}

		if (myGhostGhostCounter <= 0) //!!@ no idea what this is for. this seems like a timer of some sort, ok looks like a powerpill
		{
			myGhost->myIsClaimableFlag = false;
		}

		if ((myGhost->GetPosition() - myAvatar->GetPosition()).Length() < 10.f)
		{
			if (myGhostGhostCounter <= 0.f)
			{
				myNextGameState = GAME_STATE_DEATH_ANIMATION;
			}
			else if (myGhost->myIsClaimableFlag && !myGhost->myIsDeadFlag)
			{
				myScore += 50;
				myGhost->myIsDeadFlag = true;
				myGhost->Die(myWorld);
			}
		}

		if (CheckEndGameCondition())
		{
			myNextGameState = GAME_STATE_WIN;
		}
		else if (myLives <= 0)
		{
			myNextGameState = GAME_STATE_LOSE;
		}

		break;

	case GAME_STATE_DEATH_ANIMATION:

		myLives--;

		myGhost->SetAlpha(0);
		myGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
		myGhost->ResetTilesToCurrentPosition();

		myNextGameState = GAME_STATE_DEATH_END;

		break;

	case GAME_STATE_DEATH_END:
		myAvatar->SetPosition(Vector2f(13 * 22, 22 * 22));
		myAvatar->ResetTilesToCurrentPosition();
		SetNextMovement(Vector2f(-1, 0));
		myGhost->SetAlpha(255);
		myNextGameState = GAME_STATE_PLAY;
		break;

	case GAME_STATE_LOSE:

		myDrawer->DrawText("You lose!", "freefont-ttf\\sfd\\FreeMono.ttf", 20, 70, 24);
		return true;

		break;

	case GAME_STATE_WIN:
		
		myDrawer->DrawText("You win!", "freefont-ttf\\sfd\\FreeMono.ttf", 20, 70, 24);
		return true;

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
	int nextTileX = myAvatar->GetCurrentTileX() + myNextMovement.myX;
	int nextTileY = myAvatar->GetCurrentTileY() + myNextMovement.myY;

	if (myAvatar->IsAtDestination())
	{
		if (myWorld->TileIsValid(nextTileX, nextTileY))
		{
			myAvatar->SetNextTile(nextTileX, nextTileY);
		}
	}
}

bool Pacman::CheckEndGameCondition()
{
	return false;
}

bool Pacman::Draw()
{
	myWorld->Draw(myDrawer);
	myAvatar->Draw(myDrawer);
	myGhost->Draw(myDrawer);

	std::string scoreString;
	std::stringstream scoreStream;
	scoreStream << myScore;
	scoreString = scoreStream.str();
	myDrawer->DrawText("Score", "freefont-ttf\\sfd\\FreeMono.ttf", 20, 50, 24);
	myDrawer->DrawText(scoreString.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", 90, 50, 24);

	std::string livesString;
	std::stringstream liveStream;
	liveStream << myLives;
	livesString = liveStream.str();
	myDrawer->DrawText("Lives", "freefont-ttf\\sfd\\FreeMono.ttf", 20, 80, 24);
	myDrawer->DrawText(livesString.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", 90, 80, 24);

	myDrawer->DrawText("FPS", "freefont-ttf\\sfd\\FreeMono.ttf", 880, 50, 24);
	std::string fpsString;
	std::stringstream fpsStream;
	fpsStream << myFps;
	fpsString = fpsStream.str();
	myDrawer->DrawText(fpsString.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", 930, 50, 24);

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
