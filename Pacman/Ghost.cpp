#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"
#include "Drawer.h"
#include "AssetManager.h"
#include "BaseGhostBehaviour.h"
#include "RedGhostBehaviour.h"

#ifdef _DEBUG
const char* gStateChar[] = {
	"GHOST_START",
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
		break;
	case GHOST_CYAN:
		break;
	case GHOST_ORANGE:
		break;
	case GHOST_GRAY:
		mGhostBehaviour = new BaseGhostBehaviour(this);
		break;
	default:
		break;
	}

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

				SetIsClaimableFlag(true);
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
#ifdef _DEBUG
	std::string str;
	if (mGhostState.GetCurrentState() == GHOST_STATE_UNDEFINED)
	{
		str = "State: UNDEFINED";
	}
	else
	{
		str = "State: " + std::string(gStateChar[mGhostState.GetCurrentState()]);
	}
	aDrawer->DrawText(str.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", (int)myPosition.myX + 20 + 220, (int)myPosition.myY - 20  + 60, 20);

	SDL_SetRenderDrawColor(aDrawer->myRenderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(aDrawer->myRenderer,(int)myPosition.myX + 11 + 220, (int)myPosition.myY + 11 + 60, mGhostBehaviour->GetTileCurrentTargetTile().x * 22 + 11 + 220, mGhostBehaviour->GetTileCurrentTargetTile().y * 22 + 11 + 60);
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
	mGhostState.SetNextState(ghostState, true);
}

GhostState Ghost::GetCurrentState()
{
	return mGhostState.GetCurrentState();
}
