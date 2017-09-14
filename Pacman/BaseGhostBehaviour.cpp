#include "BaseGhostBehaviour.h"
#include "Pacman.h"
#include "Avatar.h"
#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"
#include "globals.h"

int BaseGhostBehaviour::GetIndirectMagnitude(TileCoord a, TileCoord b)
{
	return abs(a.x - b.x) + abs(a.y - b.y);
}

BaseGhostBehaviour::BaseGhostBehaviour(Ghost * ghostParent)
	:mGhostParent(ghostParent)
	,mCurrentTileTargetX(0)
	,mCurrentTileTargetY(0)
	,mPreviousDirectionUnitVecX(0)
	,mPreviousDirectionUnitVecY(0)
	,mScatterTargetTileCoord{ 26, 30 }
	,mReverseFlag(false)
	,mBehaviourSpeedMultiplier(1.f)
	,mBlinkModeFlag(false)
	,mBlinkType(false)
	,mBlinkTimer(0.f)
{
}

void BaseGhostBehaviour::HomeStateInit(float dt)
{
}

void BaseGhostBehaviour::HomeState(float dt, Vector2f & directionUnitVector)
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

	//we will make the ghost repeatedly go up and down. Note that this state is only valid when
	MoveInSameDirectionVertically(tile, directionUnitVector);
}

void BaseGhostBehaviour::HomeStateCleanup(float dt)
{
}

void BaseGhostBehaviour::ExitStateInit(float dt)
{
}

void BaseGhostBehaviour::ExitState(float dt, Vector2f & directionUnitVector)
{
	//randomly chooseone of the gate tiles
	const std::vector<PathmapTile*>& gateTilesList = gPacman->GetWorld()->GetListOfGateTiles();

	int randomNum = rand() % gateTilesList.size();
	const PathmapTile* selectedTile = gateTilesList[randomNum];

	mCurrentTileTargetX = selectedTile->myX;
	mCurrentTileTargetY = selectedTile->myX - 1;
	
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

void BaseGhostBehaviour::ExitStateCleanup(float dt)
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
		int indexForGateTile = -1;
		PathmapTile* gateTile;

		//to remove the previous tile
		for (auto i = copyValidNeighbours.begin(); i != copyValidNeighbours.end();) 
		{
			if ((*i).x == previousTileX &&
				(*i).y == previousTileY)
			{
				i = copyValidNeighbours.erase(i);
			}
			else
			{
				++i;
			}
		}

		//to remove the gate tile so that ghosts don't move back inside the home
		for (auto i = copyValidNeighbours.begin(); i != copyValidNeighbours.end();)
		{
			gateTile = world->GetTile((*i).x, (*i).y);
			if (gateTile->myIsGateFlag)
			{
				i = copyValidNeighbours.erase(i);
			}
			else
			{
				++i;
			}
		}

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
	SetBlinkFlag(false);
}

void BaseGhostBehaviour::ScatterStateInit(float dt)
{
	mGhostParent->SetImage(GetNormalImageName());
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

void BaseGhostBehaviour::ScatterStateCleanup(float dt)
{
}

void BaseGhostBehaviour::Update(float dt)
{
	if((mGhostParent->GetCurrentState() == GHOST_FRIGHTENED || mGhostParent->GetCurrentState() == GHOST_HOME) && //ghosts at home might be "frightened" based on their image name
		!mGhostParent->IsChangingState()) //if we are transitioning out of the frighten state, don't flip the blink flag
	{
		const char* ghostImageName = mGhostParent->GetImageName();
		const char* frightenedImageName = GetFrightenedImageName();
		if (gPacman->GetFrightenedGhostCounter() < GHOST_BLINK_BEFORE_FRIGHTEN_END && 
			!GetBlinkFlag() &&
			!strcmp(ghostImageName, frightenedImageName))
		{
			SetBlinkFlag(true);
		}
	}

	if (mBlinkModeFlag)
	{
		if (mBlinkType)
		{
			if (mBlinkTimer <= 0.f)
			{
				mGhostParent->SetAlpha(255);
			}

			mBlinkTimer += dt;
			
			if (mBlinkTimer >= GHOST_BLINK_ON_DURATION)
			{
				mBlinkType = false;
				mBlinkTimer = 0.f;
			}
		}
		else
		{
			if (mBlinkTimer <= 0.f)
			{
				mGhostParent->SetAlpha(0);
			}

			mBlinkTimer += dt;

			if (mBlinkTimer >= GHOST_BLINK_OFF_DURATION)
			{
				mBlinkType = true;
				mBlinkTimer = 0.f;
			}
		}
	}
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

TileCoord BaseGhostBehaviour::GetTileCurrentTargetTile()
{
	return TileCoord{ mCurrentTileTargetX, mCurrentTileTargetY };
}

void BaseGhostBehaviour::SetReverseFlag()
{
	mReverseFlag = true;
}

float BaseGhostBehaviour::GetBehaviourSpeedModifier()
{
	return mBehaviourSpeedMultiplier;
}

void BaseGhostBehaviour::ResetBehaviour()
{
	mBehaviourSpeedMultiplier = 1.0f;
	mReverseFlag = false;
}

void BaseGhostBehaviour::SoftResetBehaviour()
{
	mPreviousDirectionUnitVecX = 0;
	mPreviousDirectionUnitVecY = 0;

	SetBlinkFlag(false);
	
}

void BaseGhostBehaviour::SetBlinkFlag(bool blinkMode)
{
	mBlinkModeFlag = blinkMode;

	if (!mBlinkModeFlag)
	{
		mGhostParent->SetAlpha(255);
		mBlinkType = true;
		mBlinkTimer = 0.f;
	}
}

bool BaseGhostBehaviour::GetBlinkFlag() const
{
	return mBlinkModeFlag;
}

void BaseGhostBehaviour::SetScatterTileCoord(TileCoord& scatterTileCoord)
{
	mScatterTargetTileCoord = scatterTileCoord;
}

void BaseGhostBehaviour::MoveInSameDirection(PathmapTile* ghostParentCurrentTile, Vector2f& directionUnitVector)
{
	//SDL_Log("");
	//SDL_Log("");
	//SDL_Log("");
	//SDL_Log("~~~~~~~~~~~~~~~~~~~~~~~~~~");
	//SDL_Log("MoveInSameDirection x,y ");

	//std::string str = " x: " + std::to_string(ghostParentCurrentTile->myX)
	//	+ " y: " + std::to_string(ghostParentCurrentTile->myY)
	//	+ " v: " + std::to_string(ghostParentCurrentTile->myValidNeighbours.size());
	//SDL_Log(str.c_str());

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

void BaseGhostBehaviour::MoveInSameDirectionVertically(PathmapTile * ghostParentCurrentTile, Vector2f & directionUnitVector)
{
	//SDL_Log("");
	//SDL_Log("");
	//SDL_Log("");
	//SDL_Log("***********************************");
	//SDL_Log("MoveInSameDirectionVertically x,y ");

	//std::string str = " x: " + std::to_string(ghostParentCurrentTile->myX)
	//	+ " y: " + std::to_string(ghostParentCurrentTile->myY)
	//	+ " v: " + std::to_string(ghostParentCurrentTile->myValidNeighbours.size());
	//SDL_Log(str.c_str());
	//get ghost current tile
	int ghostParentCurrentTileX = mGhostParent->GetCurrentTileX();
	int ghostParentCurrentTileY = mGhostParent->GetCurrentTileY();

	//proceed with same direction if it is going vertically
	int proposedTileX;
	int proposedTileY;
	if (mPreviousDirectionUnitVecX != 0)
	{
		mPreviousDirectionUnitVecX = 0;
		mPreviousDirectionUnitVecY = -1;
	}

	proposedTileX = ghostParentCurrentTileX + mPreviousDirectionUnitVecX;
	proposedTileY = ghostParentCurrentTileY + mPreviousDirectionUnitVecY;

	//check if the tile is valid in the proposed direction proposedTileX, proposedTileY
	//iterate through the neighbour list
	int indexForReversingTile = -1;

	for (int i = 0; i < ghostParentCurrentTile->myValidNeighbours.size(); i++)
	{
		if (ghostParentCurrentTile->myValidNeighbours[i].x == proposedTileX &&
			ghostParentCurrentTile->myValidNeighbours[i].y == proposedTileY)
		{
			//it is ok to proceed with the continue moving in the same direction proposal
			directionUnitVector.myX = mPreviousDirectionUnitVecX;
			directionUnitVector.myY = mPreviousDirectionUnitVecY;
			//str = "DirectionA x: " + std::to_string(directionUnitVector.myX) + " y: " + std::to_string(directionUnitVector.myY);
			//SDL_Log(str.c_str());
			return;
		}

		//if moving in the same direction leads to a tile that is not valid, we will prepare ourselves and check for which tile we came from and where we were not from
		if (ghostParentCurrentTile->myValidNeighbours[i].x == proposedTileX ||
			ghostParentCurrentTile->myValidNeighbours[i].y == proposedTileY * -1)
		{
			indexForReversingTile = i;
		}
	}

	//continuing in the same direction does not yield a valid tile, so we just reverse
	TileCoord direction = ghostParentCurrentTile->myValidNeighbours[indexForReversingTile] - TileCoord{ ghostParentCurrentTileX, ghostParentCurrentTileY };
	directionUnitVector.myX = mPreviousDirectionUnitVecX = direction.x;
	directionUnitVector.myY = mPreviousDirectionUnitVecY = direction.y;
	//str = "DirectionB x: " + std::to_string(directionUnitVector.myX) + " y: " + std::to_string(directionUnitVector.myY);
	//SDL_Log(str.c_str());
}

void BaseGhostBehaviour::ReverseDirection(PathmapTile * ghostParentCurrentTile, Vector2f & directionUnitVector)
{
	//get ghost current tile
	int ghostParentCurrentTileX = ghostParentCurrentTile->myX;
	int ghostParentCurrentTileY = ghostParentCurrentTile->myY;

	//these was where the ghost came from
	int previousTileX = ghostParentCurrentTileX - mPreviousDirectionUnitVecX;
	int previousTileY = ghostParentCurrentTileY - mPreviousDirectionUnitVecY;

	TileCoord direction = TileCoord{ previousTileX ,previousTileY } -TileCoord{ ghostParentCurrentTileX, ghostParentCurrentTileY };
	directionUnitVector.myX = mPreviousDirectionUnitVecX = direction.x;
	directionUnitVector.myY = mPreviousDirectionUnitVecY = direction.y;
}
