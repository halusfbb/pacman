#include "BaseGhostBehaviour.h"
#include "Pacman.h" //!!@ saving time and doing global access
#include "Avatar.h"
#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"
#include "globals.h"

int GetIndirectMagnitude(TileCoord a, TileCoord b)
{
	return abs(a.x - b.x) + abs(a.y - b.y);
}

BaseGhostBehaviour::BaseGhostBehaviour(Ghost * ghostParent)
	:mGhostParent(ghostParent)
	,mCurrentTileTargetX(0)
	,mCurrentTileTargetY(0)
	,mPreviousDirectionUnitVecX(0)
	,mPreviousDirectionUnitVecY(0)
	,mScatterTargetTileCoord{ 25, 29 }
{
}

void BaseGhostBehaviour::ChaseStateInit(float dt)
{
	mGhostParent->SetImage(GetNormalImageName());
}

void BaseGhostBehaviour::ChaseState(float dt, Vector2f & directionUnitVector)
{
	//getting avatar and his current tile position
	const Avatar* avatar = gPacman->GetAvatar();
	mCurrentTileTargetX = avatar->GetCurrentTileX();
	mCurrentTileTargetY = avatar->GetCurrentTileY();

	//get ghost current tile
	int ghostParentCurrentTileX = mGhostParent->GetCurrentTileX();
	int ghostParentCurrentTileY = mGhostParent->GetCurrentTileY();

	//these was where the ghost came from
	int previousTileX = ghostParentCurrentTileX - mPreviousDirectionUnitVecX;
	int previousTileY = ghostParentCurrentTileY - mPreviousDirectionUnitVecY;
	//get the world
	World* world = gPacman->GetWorld();
	PathmapTile* tile = world->GetTile(ghostParentCurrentTileX, ghostParentCurrentTileY);

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

void BaseGhostBehaviour::ChaseStateCleanup(float dt)
{
}

void BaseGhostBehaviour::FrightenedStateInit(float dt)
{
	mGhostParent->SetImage(GetFrightenedImageName());
}

void BaseGhostBehaviour::FrightenedState(float dt, Vector2f & directionUnitVector)
{
	//get ghost current tile
	int ghostParentCurrentTileX = mGhostParent->GetCurrentTileX();
	int ghostParentCurrentTileY = mGhostParent->GetCurrentTileY();

	//these was where the ghost came from
	int previousTileX = ghostParentCurrentTileX - mPreviousDirectionUnitVecX;
	int previousTileY = ghostParentCurrentTileY - mPreviousDirectionUnitVecY;

	//get the world
	World* world = gPacman->GetWorld();
	PathmapTile* tile = world->GetTile(ghostParentCurrentTileX, ghostParentCurrentTileY);

	if (tile->myValidNeighbours.size() > 2)
	{
		std::vector<TileCoord> copyValidNeighbours(tile->myValidNeighbours);
		int indexForPreviousTile;
		for (int j = 0; j < copyValidNeighbours.size(); j++)
		{
			if (copyValidNeighbours[j].x == previousTileX &&
				copyValidNeighbours[j].y == previousTileY)
			{
				indexForPreviousTile = j;
				break;
			}
		}

		copyValidNeighbours.erase(copyValidNeighbours.begin() + indexForPreviousTile);

		//get a random number to select the valid tiles
		int randomIndex = rand() % copyValidNeighbours.size();

		TileCoord direction = copyValidNeighbours[randomIndex] - TileCoord{ ghostParentCurrentTileX, ghostParentCurrentTileY };
		directionUnitVector.myX = mPreviousDirectionUnitVecX = direction.x; //also saves a record of the previous direction
		directionUnitVector.myY = mPreviousDirectionUnitVecY = direction.y; //also saves a record of the previous direction
	}
	else
	{
		MoveInSameDirection(tile, directionUnitVector);
	}
}

void BaseGhostBehaviour::FrightenedStateCleanup(float dt)
{
}

void BaseGhostBehaviour::ScatterStateInit(float dt)
{
}

void BaseGhostBehaviour::ScatterState(float dt, Vector2f & directionUnitVector)
{
	//getting avatar and his current tile position
	const Avatar* avatar = gPacman->GetAvatar();
	mCurrentTileTargetX = mScatterTargetTileCoord.x;
	mCurrentTileTargetY = mScatterTargetTileCoord.y;

	//get ghost current tile
	int ghostParentCurrentTileX = mGhostParent->GetCurrentTileX();
	int ghostParentCurrentTileY = mGhostParent->GetCurrentTileY();

	//these was where the ghost came from
	int previousTileX = ghostParentCurrentTileX - mPreviousDirectionUnitVecX;
	int previousTileY = ghostParentCurrentTileY - mPreviousDirectionUnitVecY;

	//get the world
	World* world = gPacman->GetWorld();
	PathmapTile* tile = world->GetTile(ghostParentCurrentTileX, ghostParentCurrentTileY);

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

void BaseGhostBehaviour::ScatterStateCleanup(float dt)
{
}

const char * BaseGhostBehaviour::GetNormalImageName()
{
	return GRAY_GHOST_IMAGE;
}

const char * BaseGhostBehaviour::GetFrightenedImageName()
{
	return FRIGHTENED_GHOST_IMAGE;
}

const char * BaseGhostBehaviour::GetDeadImageName()
{
	return DEAD_GHOST_IMAGE;
}

void BaseGhostBehaviour::SetScatterTileCoord(TileCoord& scatterTileCoord)
{
	mScatterTargetTileCoord = scatterTileCoord;
}

void BaseGhostBehaviour::MoveInSameDirection(PathmapTile* ghostParentCurrentTile, Vector2f& directionUnitVector)
{
	//get ghost current tile
	int ghostParentCurrentTileX = mGhostParent->GetCurrentTileX();
	int ghostParentCurrentTileY = mGhostParent->GetCurrentTileY();

	//proceed with same direction.
	int proposedTileX = ghostParentCurrentTileX + mPreviousDirectionUnitVecX;
	int proposedTileY = ghostParentCurrentTileY + mPreviousDirectionUnitVecY;

	//these was where the ghost came from
	int previousTileX = ghostParentCurrentTileX - mPreviousDirectionUnitVecX;
	int previousTileY = ghostParentCurrentTileY - mPreviousDirectionUnitVecY;

	//check if the tile is valid in the proposed direction proposedTileX, proposedTileY
	//iterate through the neighbour list
	int indexForNonPreviousTile = -1; //while iterating for valid tiles, we will also keep track of the tile that is not the previous tile
	int indexForPreviousTile = -1; //and for the previous tile
	for (int i = 0; i < ghostParentCurrentTile->myValidNeighbours.size(); i++)
	{
		if (ghostParentCurrentTile->myValidNeighbours[i].x == proposedTileX &&
			ghostParentCurrentTile->myValidNeighbours[i].y == proposedTileY)
		{
			//it is ok to proceed with the continue moving in the same direction proposal
			directionUnitVector.myX = mPreviousDirectionUnitVecX;
			directionUnitVector.myY = mPreviousDirectionUnitVecY;
			return;
		}

		//if moving in the same direction leads to a tile that is not valid, we will prepare ourselves and check for which tile we came from and where we were not from
		if (ghostParentCurrentTile->myValidNeighbours[i].x != previousTileX ||
			ghostParentCurrentTile->myValidNeighbours[i].y != previousTileY)
		{
			indexForNonPreviousTile = i;
		}
		else
		{
			indexForPreviousTile = i;
		}
	}

	//continuing in the same direction does not yield a valid tile, so we just go to the tile we did not come from
	//note: if the ghost started at this position, then technically any direction is valid since none of the valid neighbours
	//was a previous tile, so the first tile that was discoverd will be taken as the new direction
	TileCoord direction;
	if (indexForNonPreviousTile != -1)
	{
		direction = ghostParentCurrentTile->myValidNeighbours[indexForNonPreviousTile] - TileCoord{ ghostParentCurrentTileX, ghostParentCurrentTileY };
	}
	else
	{
		//last resort: well the ghost has to go somewhere, so just go to the previous tile it came from
		direction = ghostParentCurrentTile->myValidNeighbours[indexForPreviousTile] - TileCoord{ ghostParentCurrentTileX, ghostParentCurrentTileY };
	}
	directionUnitVector.myX = mPreviousDirectionUnitVecX = direction.x;
	directionUnitVector.myY = mPreviousDirectionUnitVecY = direction.y;
}
