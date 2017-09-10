#include "BaseGhost.h"
#include "Pacman.h" //!!@ saving time and doing global access
#include "Avatar.h"
#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"


int GetIndirectMagnitude(TileCoord a, TileCoord b)
{

	int indirectLength = abs(a.x - b.x) + abs(a.y - b.y);

	return indirectLength;
}

BaseGhost::BaseGhost(Ghost * ghostParent)
	:mGhostParent(ghostParent)
	,mCurrentTileTargetX(0)
	,mCurrentTileTargetY(0)
	,mPreviousDirectionUnitVecX(0)
	,mPreviousDirectionUnitVecY(0)
{
}

void BaseGhost::ChaseStateInit(float dt)
{
}

void BaseGhost::ChaseState(float dt, Vector2f & directionUnitVector)
{
	//basic ghost will get current avatar current location and set that as its target tile
	//if at an intersection tile, ghost will choose the shortest the tile with the shortest direct line

	//getting avatar and his current tile position
	const Avatar* avatar = gPacman->GetAvatar();
	mCurrentTileTargetX = avatar->GetCurrentTileX(); //@ change to a function that returns a vector?? but vector is float
	mCurrentTileTargetY = avatar->GetCurrentTileY();

	//get ghost current tile
	int ghostParentCurrentTileX = mGhostParent->GetCurrentTileX();
	int ghostParentCurrentTileY = mGhostParent->GetCurrentTileY();

	//get the world
	World* world = gPacman->GetWorld();
	//checks if this is a decision making tile. which is a tile of more than 2 exits.
	PathmapTile* tile = world->GetTile(ghostParentCurrentTileX, ghostParentCurrentTileY); //!!@inefficient call

	int validNeighboursRequirement;
	if (tile->myIsStartZoneFlag)//due to the start zone having different structures compared to the rest of the maze
	{
		validNeighboursRequirement = 3;
	}
	else
	{
		validNeighboursRequirement = 2; //!!@ dont like this
	}

	if (tile->myValidNeighbours.size() > validNeighboursRequirement)
	{

		//iterate through the neighbour list to determine shortest indirect magnitude between target tile and current ghost position
		int minMagnitude = INT_MAX;
		int indexToMaxMagnitude = 0;
		int indirectMagnitude;
		for (int i = 0; i < tile->myValidNeighbours.size(); i++)
		{
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
		//proceed with same direction.
		int proposedTileX = ghostParentCurrentTileX + mPreviousDirectionUnitVecX;
		int proposedTileY = ghostParentCurrentTileY + mPreviousDirectionUnitVecY;

		//these was where the ghost came from
		int previousTileX = ghostParentCurrentTileX - mPreviousDirectionUnitVecX;
		int previousTileY = ghostParentCurrentTileY - mPreviousDirectionUnitVecY;
		//check if the tile is valid in that direction
		//iterate through the neighbour list
		int indexForNonPreviousTile = -1; //while iterating for valid tiles, we will also keep track of the tile that is not the previous tile
		int indexForPreviousTile = -1; //and for the previous tile
		for (int i = 0; i < tile->myValidNeighbours.size(); i++)
		{
			if (tile->myValidNeighbours[i].x == proposedTileX &&
				tile->myValidNeighbours[i].y == proposedTileY)
			{
				//it is ok to proceed with the continue moving in the same direction
				directionUnitVector.myX = mPreviousDirectionUnitVecX;
				directionUnitVector.myY = mPreviousDirectionUnitVecY;
				return;
			}

			//if moving in the same direction leads to a tile that is not valid, we will prepare ourselves and check for which tile we came from and where we were not from
			if (tile->myValidNeighbours[i].x != previousTileX ||
				tile->myValidNeighbours[i].y != previousTileY)
			{
				indexForNonPreviousTile = i;
			}
			else
			{
				indexForPreviousTile = i;
			}
		}

		//continuing in the same direction does not yield a valid tile, so we just go to the tile we did not come from
		TileCoord direction;
		if (indexForNonPreviousTile != -1)
		{
			direction = tile->myValidNeighbours[indexForNonPreviousTile] - TileCoord{ ghostParentCurrentTileX, ghostParentCurrentTileY };
		}
		else
		{
			//last resort: well the ghost has to go somewhere, so just go to the previous tile it came from
			direction = tile->myValidNeighbours[indexForPreviousTile] - TileCoord{ ghostParentCurrentTileX, ghostParentCurrentTileY };
		}
		directionUnitVector.myX = mPreviousDirectionUnitVecX = direction.x;
		directionUnitVector.myY = mPreviousDirectionUnitVecY = direction.y;

	}
}

void BaseGhost::ChaseStateCleanup(float dt)
{
}
