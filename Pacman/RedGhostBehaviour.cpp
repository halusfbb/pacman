#include "RedGhostBehaviour.h"
#include "globals.h"
#include "Avatar.h"
#include "Pacman.h"
#include "Ghost.h"
#include "World.h"
#include "globals.h"

RedGhostBehaviour::RedGhostBehaviour(Ghost * ghostParent)
	:BaseGhostBehaviour(ghostParent)
	, mRestrictToChaseMode(false)
	, mFirstUpgrade(false)
	, mSecondUpgrade(false)
{
	SetScatterTileCoord(TileCoord{ 24, -2 });
}

const char * RedGhostBehaviour::GetNormalImageName()
{
	return RED_GHOST_IMAGE;
}

void RedGhostBehaviour::ChaseState(float dt, Vector2f & directionUnitVector)
{
	BaseGhostBehaviour::ChaseState(dt, directionUnitVector);
}

void RedGhostBehaviour::ScatterState(float dt, Vector2f & directionUnitVector)
{
	if (mRestrictToChaseMode)
	{
		mGhostParent->SetNextState(GHOST_CHASE);
		ChaseState(dt, directionUnitVector);
	}
	else
	{
		BaseGhostBehaviour::ScatterState(dt, directionUnitVector);
	}
}

void RedGhostBehaviour::Update(float dt)
{
	if (!mFirstUpgrade || !mSecondUpgrade)
	{
		int listDots = gPacman->GetWorld()->GetListOfDots().size();
		int listOfOrignalDots = gPacman->GetWorld()->GetOriginalListOfDots().size();

		int difference = listOfOrignalDots - listDots;

		if (!mFirstUpgrade)
		{
			if (difference > RED_GHOST_DOT_TOLERANCE)
			{	//during this first upgrade, ghost speed increases
				mBehaviourSpeedMultiplier *= RED_GHOSt_SPEED_UPGRADE;
				float currentSpeed = mGhostParent->GetCurrentSpeed();
				mGhostParent->SetCurrentSpeed(currentSpeed*mBehaviourSpeedMultiplier); //set the speed so of the current state will have increased speed
				mFirstUpgrade = true;
			}
		}
		else if (!mSecondUpgrade)
		{	//during this second upgrade, there will be a further speed increase and the red ghost will not go to scatter state
			if (difference > (2 * RED_GHOST_DOT_TOLERANCE))
			{
				mBehaviourSpeedMultiplier *= RED_GHOSt_SPEED_UPGRADE;
				float currentSpeed = mGhostParent->GetCurrentSpeed();
				mGhostParent->SetCurrentSpeed(currentSpeed*mBehaviourSpeedMultiplier);
				mRestrictToChaseMode = true;
				mSecondUpgrade = true;
			}
		}
	}

	BaseGhostBehaviour::Update(dt);
}

void RedGhostBehaviour::ResetBehaviour()
{
	mRestrictToChaseMode = false;
	mFirstUpgrade = false;
	mSecondUpgrade = false;

	BaseGhostBehaviour::ResetBehaviour();
}
