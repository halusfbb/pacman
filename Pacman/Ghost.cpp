#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"
#include "Drawer.h"
#include "AssetManager.h"

Ghost::Ghost(const Vector2f& aPosition)
: MovableGameEntity(aPosition, "ghost_32.png")
{
	myIsClaimableFlag = false;
	myIsDeadFlag = false;

	myDesiredMovementX = 0;
	myDesiredMovementY = -1;
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
	float speed = 30.f;
	int nextTileX = GetCurrentTileX() + myDesiredMovementX;
	int nextTileY = GetCurrentTileY() + myDesiredMovementY;

	if (myIsDeadFlag)
		speed = 120.f;

	if (IsAtDestination())
	{
		if (!myPath.empty())
		{
			PathmapTile* nextTile = myPath.front();
			myPath.pop_front();
			SetNextTile(nextTile->myX, nextTile->myY);
		}
		else if (aWorld->TileIsValid(nextTileX, nextTileY))
		{
			SetNextTile(nextTileX, nextTileY);
		}
		else
		{
			if (myDesiredMovementX == 1)
			{
				myDesiredMovementX = 0;
				myDesiredMovementY = 1;
			} else if (myDesiredMovementY == 1)
			{
				myDesiredMovementX = -1;
				myDesiredMovementY = 0;			
			} else if (myDesiredMovementX == -1)
			{
				myDesiredMovementX = 0;
				myDesiredMovementY = -1;
			} else
			{
				myDesiredMovementX = 1;
				myDesiredMovementY = 0;
			}

			myIsDeadFlag = false;
		}
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
	else if (myIsClaimableFlag)
		SetImage("Ghost_Vulnerable_32.png");
	else
		SetImage("ghost_32.png");
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
