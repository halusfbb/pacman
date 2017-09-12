#include "GhostManager.h"
#include "Pacman.h"
#include "globals.h"
#include "SDL.h"
#include "Drawer.h"
#include <string>

#ifdef _DEBUG
#include <sstream>
#endif

GhostManager * GhostManager::Create()
{
	GhostManager* ghostManager = new GhostManager();
	if (ghostManager)
		ghostManager->Init();

	return ghostManager;
}

GhostManager::GhostManager()
	:mCurrentState(GHOST_SCATTER)
	,mCyclic_Chase_Scatter(GHOST_SCATTER)
	,mNoOf_Chase_Scatter_Cycles(0)
{
}

void GhostManager::Init()
{
	//initialie the ghosts
	mGhostvec.resize(5);
	Ghost* redGhost = Ghost::Create(Vector2f(10 * 22, 25 * 22), GHOST_RED);
	Ghost* pinkGhost = Ghost::Create(Vector2f(25 * 22, 0 * 22), GHOST_PINK);
	Ghost* cyanGhost = Ghost::Create(Vector2f(24 * 22, 28 * 22), GHOST_CYAN);
	Ghost* orangeGhost = Ghost::Create(Vector2f(3 * 22, 7 * 22), GHOST_ORANGE);
	Ghost* grayGhost = Ghost::Create(Vector2f(8 * 22, 4 * 22), GHOST_GRAY);
	mGhostvec[GHOST_RED] = redGhost;
	mGhostvec[GHOST_PINK] = pinkGhost;
	mGhostvec[GHOST_CYAN] = cyanGhost;
	mGhostvec[GHOST_ORANGE] = orangeGhost;
	mGhostvec[GHOST_GRAY] = grayGhost;

	//get the duration for the state in mCyclicChase_Scatter
	mCurrentCycleStateTimer = GetDurationForChase_ScatterState(mCyclic_Chase_Scatter);
	SetGhostsNextState(mCyclic_Chase_Scatter);
}

void GhostManager::Update(float aTime)
{
	if (mCurrentState == mCyclic_Chase_Scatter)
	{
		mCurrentCycleStateTimer -= aTime;
		if (mCurrentCycleStateTimer <= 0)
		{
			SwapGhostsState();
		}
	}

	for (auto ghost : mGhostvec)
	{
		ghost->Update(aTime, gPacman->GetWorld());
	}
}

void GhostManager::Draw(Drawer * aDrawer)
{
	for (auto ghost : mGhostvec)
	{
		ghost->Draw(aDrawer);
	}
#ifdef _DEBUG
	float timer;
	if (mCurrentState == GHOST_FRIGHTENED)
	{
		timer = gPacman->GetFrightenedGhostCounter();
	}
	else
	{
		timer = mCurrentCycleStateTimer;
	}
	std::string str = "State: " + std::string(gGhostStateChar[mCurrentState]) + " " + std::to_string(timer);
	aDrawer->DrawText(str.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", 20, 275, 24, SDL_Color{255,0,0});
#endif
}

void GhostManager::SetGhostsNextState(GhostState ghostState)
{
	for (auto ghost : mGhostvec)
	{
		ghost->SetNextState(ghostState);
	}
	
	mCurrentState = ghostState;
}

void GhostManager::RevertToPreviousState()
{
	SetGhostsNextState(mCyclic_Chase_Scatter);
}

void GhostManager::SwapGhostsState()
{
	if (mCyclic_Chase_Scatter == GHOST_CHASE)
	{
		mCyclic_Chase_Scatter = GHOST_SCATTER;
		mNoOf_Chase_Scatter_Cycles++;
	}
	else
	{
		mCyclic_Chase_Scatter = GHOST_CHASE;
		mNoOf_Chase_Scatter_Cycles++;
	}

	mCurrentCycleStateTimer = GetDurationForChase_ScatterState(mCyclic_Chase_Scatter);
	SetGhostsNextState(mCyclic_Chase_Scatter);
}

const std::vector<Ghost*>& GhostManager::GetVectorOfGhost()
{
	return mGhostvec;
}

float GhostManager::GetDurationForChase_ScatterState(GhostState ghoststate)
{
	switch (ghoststate)
	{
	case GHOST_CHASE:
		if (mNoOf_Chase_Scatter_Cycles >= 7)
		{
			return INT_MAX; //indefinitely on chase
		}
		else
		{
			return (float)(rand() % (CHASE_STATE_MAX_DURATION - CHASE_STATE_MIN_DURATION + 1) + CHASE_STATE_MIN_DURATION);
		}
		break;
	case GHOST_SCATTER:
		return (float)(rand() % (SCATTER_STATE_MAX_DURATION - SCATTER_STATE_MIN_DURATION + 1) + SCATTER_STATE_MIN_DURATION);
		break;
	default:
		SDL_Log("Unkown ghost state for cyclic behaviour!!!");
		break;
	}

	return 0.f;
}
