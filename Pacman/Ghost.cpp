#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"
#include "Drawer.h"
#include "AssetManager.h"
#include "BaseGhostBehaviour.h"
#include "RedGhostBehaviour.h"
#include "PinkGhostBehaviour.h"
#include "CyanGhostBehaviour.h"
#include "OrangeGhostBehaviour.h"
#include "Pacman.h"
#include "globals.h"

#ifdef _DEBUG
const char* gGhostStateChar[] = {
	"GHOST_HOME",
	"GHOST_EXITING",
	"GHOST_CHASE",
	"GHOST_SCATTER",
	"GHOST_FRIGHTENED"
};
#endif

Ghost * Ghost::Create(const Vector2f & aPosition, GhostColor ghostColor)
{
	Ghost* ghost = new Ghost(aPosition, ghostColor);
	if (!ghost)
		return NULL;
	
	ghost->Init();
	return ghost;
}

Ghost::Ghost(const Vector2f& aPosition, GhostColor ghostColor)
: MovableGameEntity(aPosition, "")
, myIsClaimableFlag(false)
, myIsDeadFlag(false)
, mGhostColor(ghostColor)
, myIsResurrectedFlag(false)
, mOriginalStartPos(aPosition)
, mGhostSpeed(GHOST_BASE_SPEED)
{
}

Ghost::~Ghost(void)
{
}

void Ghost::Init()
{
	myDesiredMovementX = 0; //!!@remove??
	myDesiredMovementY = -1;

	switch (mGhostColor)
	{
	case GHOST_RED:
		mGhostBehaviour = new RedGhostBehaviour(this);
		break;
	case GHOST_PINK:
		mGhostBehaviour = new PinkGhostBehaviour(this);
		break;
	case GHOST_CYAN:
		mGhostBehaviour = new CyanGhostBehaviour(this);
		break;
	case GHOST_ORANGE:
		mGhostBehaviour = new OrangeGhostBehaviour(this);
		break;
	case GHOST_GRAY:
		mGhostBehaviour = new BaseGhostBehaviour(this);
		break;
	default:
		break;
	}

	SetImage(mGhostBehaviour->GetNormalImageName());
	MovableGameEntity::Init();
}

void Ghost::Die(World* aWorld)
{
	myPath.clear();
	aWorld->GetPath(myCurrentTileX, myCurrentTileY, 13, 13, myPath);
}

void Ghost::Update(float aTime, World* aWorld)
{
	Vector2f unitDirection = Vector2f(0.f,0.f);

	mGhostState.CheckAndSwapState();
	mGhostBehaviour->Update(aTime);

	switch (mGhostState.GetCurrentState())
	{
	case GHOST_HOME:
		//initialize
		if (!mGhostState.IsInitialized())
		{
			if (!mGhostState.IsInSubState())
			{
				mGhostState.SetSubState();
				mGhostBehaviour->HomeStateInit(aTime);
			}
			else
			{
				mGhostState.SetInitalizingDone();
			}
		}

		//main
		else if (!mGhostState.IsChangingState())
		{
			if (IsAtDestination())
				mGhostBehaviour->HomeState(aTime, unitDirection);
		}

		//cleanup
		else if (!mGhostState.IsCleanedUp())
		{
			if (!mGhostState.IsInSubState())
			{
				mGhostState.SetSubState();
				mGhostBehaviour->HomeStateCleanup(aTime);
			}
			else
			{
				mGhostState.SetCleanUpDone();
			}
		}
		break;
	case GHOST_EXITING:
		//initialize
		if (!mGhostState.IsInitialized())
		{
			if (!mGhostState.IsInSubState())
			{
				mGhostState.SetSubState();
				mGhostBehaviour->ExitStateInit(aTime);
			}
			else
			{
				mGhostState.SetInitalizingDone();
			}
		}

		//main
		else if (!mGhostState.IsChangingState())
		{
			if (IsAtDestination())
				mGhostBehaviour->ExitState(aTime, unitDirection);
		}

		//cleanup
		else if (!mGhostState.IsCleanedUp())
		{
			if (!mGhostState.IsInSubState())
			{
				mGhostState.SetSubState();
				mGhostBehaviour->ExitStateCleanup(aTime);
			}
			else
			{
				mGhostState.SetCleanUpDone();
			}
		}
		break;
	case GHOST_CHASE:
		//initialize
		if (!mGhostState.IsInitialized())
		{
			if (!mGhostState.IsInSubState())
			{
				SetNormalSpeed();
				mGhostState.SetSubState();
				mGhostBehaviour->ChaseStateInit(aTime);
			}
			else
			{
				mGhostState.SetInitalizingDone();
			}
		}

		//main
		else if (!mGhostState.IsChangingState())
		{	
			if(IsAtDestination())
				mGhostBehaviour->ChaseState(aTime, unitDirection);
		}

		//cleanup
		else if (!mGhostState.IsCleanedUp())
		{
			if (!mGhostState.IsInSubState())
			{
				mGhostState.SetSubState();
				mGhostBehaviour->ChaseStateCleanup(aTime);
			}
			else
			{
				mGhostState.SetCleanUpDone();
			}
		}

		break;

	case GHOST_SCATTER:
		//initialize
		if (!mGhostState.IsInitialized())
		{
			if (!mGhostState.IsInSubState())
			{
				SetNormalSpeed();
				mGhostState.SetSubState();
				mGhostBehaviour->ScatterStateInit(aTime);
			}
			else
			{
				mGhostState.SetInitalizingDone();
			}
		}

		//main
		else if (!mGhostState.IsChangingState())
		{
			if (IsAtDestination())// !!@Temporarily removing. howvver there is a bug here. If ghost has reached destination, then there is no way of asking the behaviour for another destination
				mGhostBehaviour->ScatterState(aTime, unitDirection);
		}

		//cleanup
		else if (!mGhostState.IsCleanedUp())
		{
			if (!mGhostState.IsInSubState())
			{
				mGhostState.SetSubState();
				mGhostBehaviour->ScatterStateCleanup(aTime);
			}
			else
			{
				mGhostState.SetCleanUpDone();
			}
		}

		break;

	case GHOST_FRIGHTENED:
		//initialize
		if (!mGhostState.IsInitialized())
		{
			if (!mGhostState.IsInSubState())
			{
				mGhostState.SetSubState();
				mGhostBehaviour->FrightenedStateInit(aTime);
				SetFrightenedSpeed();
				SetIsClaimableFlag(true);
			}
			else
			{
				mGhostState.SetInitalizingDone();
			}
		}

		//main
		else if (!mGhostState.IsChangingState())
		{
			if (IsAtDestination())
			{
				mGhostBehaviour->FrightenedState(aTime, unitDirection);
			}
		}

		//cleanup
		else if (!mGhostState.IsCleanedUp())
		{
			if (!mGhostState.IsInSubState())
			{
				mGhostState.SetSubState();
				mGhostBehaviour->FrightenedStateCleanup(aTime);
			}
			else
			{
				mGhostState.SetCleanUpDone();
			}
		}

		break;

	default:
		break;
	}

	if (!myPath.empty())
	{
		if (IsAtDestination())
		{
			PathmapTile* nextTile = myPath.front();
			myPath.pop_front();
			SetNextTile(nextTile->myX, nextTile->myY);
		}
	}
	else if (unitDirection.Length() != 0)
	{
		if (myIsDeadFlag) //if previously dead
		{
			myIsResurrectedFlag = true;
			myIsDeadFlag = false;
			SetNextState(GHOST_EXITING);
		}

		int nextTileX = GetCurrentTileX() + unitDirection.myX;
		int nextTileY = GetCurrentTileY() + unitDirection.myY;

		SetNextTile(nextTileX, nextTileY);
	}

	int tileSize = TILE_SIZE;
	Vector2f destination(myNextTileX * tileSize, myNextTileY * tileSize);
	Vector2f direction = destination - myPosition;

	float distanceToMove = aTime * mGhostSpeed;

	if (distanceToMove > direction.Length())
	{
		myPosition = destination;
		myCurrentTileX = myNextTileX;
		myCurrentTileY = myNextTileY;
	}
	else
	{
		direction.Normalize();
		myPosition += direction * distanceToMove;
	}

	if (myIsDeadFlag)
	{
		if (mGhostState.GetCurrentState() == GHOST_HOME)
		{
			SetNormalImage();
			SetNormalSpeed();
		}
		else
		{
			SetDeadImage();
			SetDeadSpeed();
		}
	}
}

void Ghost::Draw(Drawer* aDrawer)
{
	GameEntity::Draw(aDrawer);
#ifdef _DEBUG
	std::string str;
	if (mGhostState.GetCurrentState() == GHOST_STATE_UNDEFINED)
	{
		str = "State: UNDEFINED";
	}
	else
	{
		str = "State: " + std::string(gGhostStateChar[mGhostState.GetCurrentState()]);
	}
	aDrawer->DrawText(str.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", (int)myPosition.myX + 20 + ENTITIY_OFFSET_SIZE_X, (int)myPosition.myY - 20  + ENTITIY_OFFSET_SIZE_Y, 20, SDL_Color{ 255,0,0 });

	SDL_SetRenderDrawColor(aDrawer->myRenderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
	if (mGhostState.GetCurrentState() != GHOST_FRIGHTENED)
	{
		SDL_RenderDrawLine(aDrawer->myRenderer, (int)myPosition.myX + 11 + ENTITIY_OFFSET_SIZE_X, (int)myPosition.myY + 11 + ENTITIY_OFFSET_SIZE_Y, mGhostBehaviour->GetTileCurrentTargetTile().x * 22 + 11 + 220, mGhostBehaviour->GetTileCurrentTargetTile().y * 22 + 11 + 60);
	}
#endif
}

void Ghost::SetIsClaimableFlag(bool value)
{
	myIsClaimableFlag = value;
}

bool Ghost::GetIsClaimableFlag()
{
	return myIsClaimableFlag;
}

void Ghost::SetNextState(GhostState ghostState)
{
	if (ghostState == GHOST_FRIGHTENED) //because we have a cleanup logic in ghost frighted state
	{
		mGhostState.SetNextState(ghostState, true, true);
	}
	else
	{
		mGhostState.SetNextState(ghostState, true);
	}
}

GhostState Ghost::GetCurrentState()
{
	return mGhostState.GetCurrentState();
}

bool Ghost::IsChangingState()
{
	return mGhostState.IsChangingState();
}

bool Ghost::IsGhostAtHome()
{
	//get the world
	World* world = gPacman->GetWorld();
	PathmapTile* tile;
	if (world)
	{
		tile = world->GetTile(myCurrentTileX, myCurrentTileY);
		return tile->myIsStartZoneFlag;
	}

	return false;
}

void Ghost::SetFrightenedImage()
{
	SetImage(mGhostBehaviour->GetFrightenedImageName());
}

void Ghost::SetNormalImage()
{
	SetImage(mGhostBehaviour->GetNormalImageName());
}

void Ghost::SetDeadImage()
{
	SetImage(mGhostBehaviour->GetDeadImageName());
}

void Ghost::SetBlink(bool flag) const
{
	mGhostBehaviour->SetBlinkFlag(false);
}

void Ghost::ResetGhostBehaviour()
{
	mGhostBehaviour->ResetBehaviour();
}

void Ghost::SoftReset()
{
	myPosition = mOriginalStartPos;
	ResetTilesToCurrentPosition();
	myIsClaimableFlag = false;
	myIsDeadFlag = false;
	myNextTileX = myCurrentTileX;
	myNextTileY = myCurrentTileY;
	myIsResurrectedFlag = false;
	SetImage(mGhostBehaviour->GetNormalImageName());
	SetAlpha(255);

	mGhostBehaviour->SoftResetBehaviour();
}

void Ghost::SetFrightenedSpeed()
{
	mGhostSpeed = GHOST_BASE_SPEED * GHOST_FRIGHTENED_SPEED_FACTOR * mGhostBehaviour->GetBehaviourSpeedModifier();
}

void Ghost::SetDeadSpeed()
{
	mGhostSpeed = GHOST_BASE_SPEED * GHOST_DEAD_SPEED_FACTOR * mGhostBehaviour->GetBehaviourSpeedModifier();
}

void Ghost::SetNormalSpeed()
{
	mGhostSpeed = GHOST_BASE_SPEED * mGhostBehaviour->GetBehaviourSpeedModifier();
}

float Ghost::GetCurrentSpeed()
{
	return mGhostSpeed;
}

void Ghost::SetCurrentSpeed(float speed)
{
	mGhostSpeed = speed;
}

void Ghost::SetBehaviourReverseFlag()
{
	mGhostBehaviour->SetReverseFlag();
}
