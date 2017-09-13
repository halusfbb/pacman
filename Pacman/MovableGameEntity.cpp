#include "MovableGameEntity.h"
#include "globals.h"

MovableGameEntity::MovableGameEntity(const Vector2f& aPosition, const char* anImage)
: GameEntity(aPosition, anImage)
{
	myCurrentTileX = myNextTileX =  myPosition.myX / TILE_SIZE;
	myCurrentTileY = myNextTileY =  myPosition.myY / TILE_SIZE;
}

MovableGameEntity::~MovableGameEntity(void)
{
}

bool MovableGameEntity::IsAtDestination()
{
	if (myCurrentTileX == myNextTileX && myCurrentTileY == myNextTileY)
	{


		return true;
	}

	return false;
}

void MovableGameEntity::ResetTilesToCurrentPosition()
{
	myCurrentTileX = myNextTileX = myPosition.myX / TILE_SIZE;
	myCurrentTileY = myNextTileY = myPosition.myY / TILE_SIZE;
}

void MovableGameEntity::SetNextTile(int anX, int anY)
{
	myNextTileX = anX;
	myNextTileY = anY;
}
