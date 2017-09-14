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
	:mCurrentState(GHOST_CHASE)
	,mCyclic_Chase_Scatter(INITIAL_CYCLIC_GHOST_STATE)
	,mNoOf_Chase_Scatter_Cycles(0)
	,mTimerBeforeGhostExit(0.f)
	,mRedGhost(NULL)
{
}

void GhostManager::Init()
{
	//!!@cclear the memeory at destructor
	//initialie the ghosts
	Ghost* redGhost = Ghost::Create(Vector2f(12 * 22, 10 * 22), GHOST_RED); // red ghost always start outside
	//all ghosts inside
	Ghost* pinkGhost = Ghost::Create(Vector2f(12 * 22, 13 * 22), GHOST_PINK);
	Ghost* cyanGhost = Ghost::Create(Vector2f(14 * 22, 13 * 22), GHOST_CYAN);
	Ghost* orangeGhost = Ghost::Create(Vector2f(10 * 22, 13 * 22), GHOST_ORANGE);
	//Ghost* grayGhost = Ghost::Create(Vector2f(15 * 22, 13 * 22), GHOST_GRAY);
	//all ghosts outide
	//Ghost* pinkGhost = Ghost::Create(Vector2f(25 * 22, 0 * 22), GHOST_PINK);
	//Ghost* cyanGhost = Ghost::Create(Vector2f(24 * 22, 28 * 22), GHOST_CYAN);
	//Ghost* orangeGhost = Ghost::Create(Vector2f(3 * 22, 7 * 22), GHOST_ORANGE);
	//Ghost* grayGhost = Ghost::Create(Vector2f(8 * 22, 4 * 22), GHOST_GRAY);

	mGhostvec.push_back(redGhost);
	mGhostvec.push_back(pinkGhost);
	mGhostvec.push_back(cyanGhost);
	mGhostvec.push_back(orangeGhost);
	//mGhostvec.push_back(grayGhost);

	mRedGhost = redGhost;

	//get the duration for the state in mCyclicChase_Scatter
	mCurrentCycleStateTimer = GetDurationForChase_ScatterState(mCyclic_Chase_Scatter);
	SetGhostsNextState(mCyclic_Chase_Scatter, true);

	mTimerBeforeGhostExit = GetDurationForGhostRelease();
}

void GhostManager::Update(float aTime)
{
	//checks for ghosts that are at home
	for (auto i = mGhostvec.begin(); i != mGhostvec.end();) {
		if ((*i)->IsGhostAtHome())
		{
			(*i)->SetNextState(GHOST_HOME);
			mGhostAtHomevec.push_back(*i);
			i = mGhostvec.erase(i);
		}
		else
		{
			++i;
		}
	}

	//checks for ghosts that left home. Although we are iterating over elements that was just added. With 4 ghosts, the overhead isn't goign to be much.
	for (auto i = mGhostAtHomevec.begin(); i != mGhostAtHomevec.end();)
	{
		if (!(*i)->IsGhostAtHome())
		{
			if ((*i)->myIsResurrectedFlag)
			{
				(*i)->SetNextState(mCyclic_Chase_Scatter);
			}
			else
			{
				(*i)->SetNextState(mCurrentState);
			}
			mGhostvec.push_back(*i);
			i = mGhostAtHomevec.erase(i);
		}
		else
		{
			++i;
		}
	}

	if (mCurrentState == mCyclic_Chase_Scatter)
	{
		mCurrentCycleStateTimer -= aTime;
		if (mCurrentCycleStateTimer <= 0)
		{
			SwapCyclicState();
		}
	}

	if (mGhostAtHomevec.size() > 0)
	{
		mTimerBeforeGhostExit -= aTime;
		if (mTimerBeforeGhostExit < 0)
		{
			mGhostAtHomevec[0]->SetNextState(GHOST_EXITING);
			mTimerBeforeGhostExit = GetDurationForGhostRelease();
		}
	}

	for (auto ghost : mGhostvec)
	{
		ghost->Update(aTime, gPacman->GetWorld());
	}

	for (auto ghost : mGhostAtHomevec)
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

	for (auto ghost : mGhostAtHomevec)
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

void GhostManager::SetGhostsNextState(GhostState ghostState, bool isRevertingFromPreviousState)
{
	for (auto ghost : mGhostvec)
	{
		ghost->SetNextState(ghostState);
		if (!isRevertingFromPreviousState)
		{
			ghost->SetBehaviourReverseFlag();
		}
	}

	//for ghosts in the home zone
	if (ghostState == GHOST_FRIGHTENED)
	{
		//set ghosts in the home also to frightened image
		for (auto ghost : mGhostAtHomevec)
		{
			ghost->SetFrightenedImage();
			ghost->SetFrightenedSpeed();
		}
	}
	else
	{
		for (auto ghost : mGhostAtHomevec)
		{
			if (ghost->myIsDeadFlag)
			{
				ghost->SetDeadImage();
			}
			else
			{
				ghost->SetNormalImage();
				ghost->SetBlink(false);
				ghost->SetNormalSpeed();
			}
		}
	}
	
	mCurrentState = ghostState;
}

void GhostManager::RevertToPreviousState()
{
	SetGhostsNextState(mCyclic_Chase_Scatter, true);
}

void GhostManager::SwapCyclicState()
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
	SetGhostsNextState(mCyclic_Chase_Scatter, false);
}

const std::vector<Ghost*>& GhostManager::GetVectorOfHomeGhost()
{
	return mGhostAtHomevec;
}

const Ghost * GhostManager::GetRedGhost() const
{
	return mRedGhost;
}

void GhostManager::ResetCycleCount()
{
	mNoOf_Chase_Scatter_Cycles = 0;
	mCurrentCycleStateTimer = 0.f;
	mCyclic_Chase_Scatter = INITIAL_CYCLIC_GHOST_STATE;
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
		if (mNoOf_Chase_Scatter_Cycles >= CHASE_SCATTER_SWAPS)
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

float GhostManager::GetDurationForGhostRelease()
{
	return (float)(rand() % (MAX_TIMER_BEFORE_GHOSTS_EXIT - MIN_TIMER_BEFORE_GHOST_EXIT + 1) + MIN_TIMER_BEFORE_GHOST_EXIT);
}
