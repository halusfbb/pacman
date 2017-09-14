#include "PinkGhostBehaviour.h"
#include "globals.h"
#include "Avatar.h"
#include "Pacman.h"
#include "Ghost.h"
#include "World.h"

PinkGhostBehaviour::PinkGhostBehaviour(Ghost * ghostParent)
	:BaseGhostBehaviour(ghostParent)
{
	SetScatterTileCoord(TileCoord{ 2, -2 });
}

const char * PinkGhostBehaviour::GetNormalImageName()
{
	return PINK_GHOST_IMAGE;
}

void PinkGhostBehaviour::ChaseState(float dt, Vector2f & directionUnitVector)
{
	//getting avatar and his current tile position
	const Avatar* avatar = gPacman->GetAvatar();
	mCurrentTileTargetX = avatar->GetCurrentTileX();
	mCurrentTileTargetY = avatar->GetCurrentTileY();

	eAvatarFacing avatarFaceDirection = avatar->GetAvatarFaceDirection();
	switch (avatarFaceDirection)
	{
	case AVATAR_FACING_UP:
		mCurrentTileTargetY -= 4;
		break;
	case AVATAR_FACING_LEFT:
		mCurrentTileTargetX -= 4;
		break;
	case AVATAR_FACING_DOWN:
		mCurrentTileTargetY += 4;
		break;
	case AVATAR_FACING_RIGHT:
		mCurrentTileTargetX += 4;
		break;
	default:
		break;
	}

	//get ghost current tile
	int ghostParentCurrentTileX = mGhostParent->GetCurrentTileX();
	int ghostParentCurrentTileY = mGhostParent->GetCurrentTileY();

	//these was where the ghost came from
	int previousTileX = ghostParentCurrentTileX - mPreviousDirectionUnitVecX;
	int previousTileY = ghostParentCurrentTileY - mPreviousDirectionUnitVecY;
	//get the world
	World* world = gPacman->GetWorld();
	PathmapTile* tile = world->GetTile(ghostParentCurrentTileX, ghostParentCurrentTileY);
	PathmapTile* destinationTile;

	if (mReverseFlag)
	{
		ReverseDirection(tile, directionUnitVector);
		mReverseFlag = false;
		return;
	}

	// if there are more than 2 valid neighbors, we are to use the target tile as an influence to this ghost's next direction,
	if (tile->myValidNeighbours.size() > 2)
	{
		//iterate through the neighbour list to determine shortest indirect magnitude between target tile and current ghost position
		int minMagnitude = INT_MAX;
		int indexToMaxMagnitude = 0;
		int indirectMagnitude;
		for (int i = 0; i < tile->myValidNeighbours.size(); i++)
		{
			if (tile->myValidNeighbours[i].x == previousTileX &&
				tile->myValidNeighbours[i].y == previousTileY)
			{
				continue;
			}
			destinationTile = world->GetTile(tile->myValidNeighbours[i].x, tile->myValidNeighbours[i].y);
			if (destinationTile->myIsGateFlag)
			{
				continue; //don't go back into the home zone
			}

			indirectMagnitude = GetIndirectMagnitude(tile->myValidNeighbours[i], TileCoord{ mCurrentTileTargetX, mCurrentTileTargetY });
			if (minMagnitude > indirectMagnitude)
			{
				minMagnitude = indirectMagnitude;
				indexToMaxMagnitude = i;
			}
		}

		//get direction vector to neighbour indexed by indexToMaxMagnitude
		TileCoord direction = tile->myValidNeighbours[indexToMaxMagnitude] - TileCoord{ ghostParentCurrentTileX, ghostParentCurrentTileY };
		directionUnitVector.myX = mPreviousDirectionUnitVecX = direction.x; //also saves a record of the previous direction
		directionUnitVector.myY = mPreviousDirectionUnitVecY = direction.y; //also saves a record of the previous direction
	}
	else
	{
		MoveInSameDirection(tile, directionUnitVector);
	}
}
