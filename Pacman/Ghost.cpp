#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"
#include "Drawer.h"
#include "AssetManager.h"
#include "BaseGhost.h"

Ghost::Ghost(const Vector2f& aPosition)
: MovableGameEntity(aPosition, "ghost_32.png")
{
	myIsClaimableFlag = false;
	myIsDeadFlag = false;

	myDesiredMovementX = 0;
	myDesiredMovementY = -1;

	//!!@temp
	mGhostBehaviour = new BaseGhost(this);
	mGhostState.SetNextState(GHOST_CHASE, true);
}

Ghost::~Ghost(void)
{
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

	switch (mGhostState.GetCurrentState())
	{
	case GHOST_START:

		break;
	case GHOST_CHASE:
		//initialize
		if (!mGhostState.IsInitialized())
		{
			if (!mGhostState.IsInSubState())
			{
				mGhostState.SetSubState();
				mGhostBehaviour->ChaseStateInit(aTime);
			}
			else
			{
				mGhostState.SetInitalizingDone();
			}
		}

		//main
		if (!mGhostState.IsChangingState())
		{	
			if(IsAtDestination())
				mGhostBehaviour->ChaseState(aTime, unitDirection);
		}

		//cleanup
		if (!mGhostState.IsCleanedUp())
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
			}
			else
			{
				mGhostState.SetInitalizingDone();
			}
		}

		//main
		if (!mGhostState.IsChangingState())
		{
			if (IsAtDestination())
				mGhostBehaviour->FrightenedState(aTime, unitDirection);
		}

		//cleanup
		if (!mGhostState.IsCleanedUp())
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

	float speed = 30.f;
	if (unitDirection.Length() != 0)
	{
		int nextTileX = GetCurrentTileX() + unitDirection.myX;
		int nextTileY = GetCurrentTileY() + unitDirection.myY;

//		if (aWorld->TileIsValid(nextTileX, nextTileY)) //!!@ this check might be unnessary if ghost behaviours already made sure of this. and this is expensive operate at the moment
//		{
			SetNextTile(nextTileX, nextTileY);
//		}
	}

	int tileSize = 22;
	Vector2f destination(myNextTileX * tileSize, myNextTileY * tileSize);
	Vector2f direction = destination - myPosition;

	float distanceToMove = aTime * speed;

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
		SetImage("Ghost_Dead_32.png");
}

void Ghost::SetImage(const char* anImage)
{
	if (myImageAssetCache)
	{
		if (strcmp(anImage, myImageAssetCache->getImageName().c_str()) == 0)
			return; // image has no changes
	}
	
	myImageAssetCache = std::dynamic_pointer_cast<ImageAssetCache>(AssetManager::GetInstance()->GetImageAsset(anImage));
}

void Ghost::Draw(Drawer* aDrawer)
{
	aDrawer->Draw(myImageAssetCache, (int)myPosition.myX + 220, (int)myPosition.myY + 60);
}

void Ghost::SetIsClaimableFlag(bool value)
{
	myIsClaimableFlag = value;
	if(myIsClaimableFlag)
		mGhostState.SetNextState(GHOST_FRIGHTENED, true);
	else
		mGhostState.SetNextState(GHOST_CHASE); //!!@ this needs to be extended 
}

bool Ghost::GetIsClaimableFlag()
{
	return myIsClaimableFlag;
}
