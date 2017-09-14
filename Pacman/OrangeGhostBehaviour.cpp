#include "OrangeGhostBehaviour.h"
#include "globals.h"
#include "Avatar.h"
#include "Pacman.h"
#include "Ghost.h"
#include "World.h"

OrangeGhostBehaviour::OrangeGhostBehaviour(Ghost * ghostParent)
	:BaseGhostBehaviour(ghostParent)
{
	SetScatterTileCoord(TileCoord{ 0, 30 });
}

const char * OrangeGhostBehaviour::GetNormalImageName()
{
	return ORANGE_GHOST_IMAGE;
}

void OrangeGhostBehaviour::ChaseState(float dt, Vector2f & directionUnitVector)
{
	//get ghost current tile
	int ghostParentCurrentTileX = mGhostParent->GetCurrentTileX();
	int ghostParentCurrentTileY = mGhostParent->GetCurrentTileY();
	//get the world
	World* world = gPacman->GetWorld();
	PathmapTile* ghostTile = world->GetTile(ghostParentCurrentTileX, ghostParentCurrentTileY);

	//getting avatar and his current tile position
	const Avatar* avatar = gPacman->GetAvatar();
	int avatarCurrentX = avatar->GetCurrentTileX();
	int avatarCurrentY = avatar->GetCurrentTileY();
	PathmapTile* avatarTile = world->GetTile(avatarCurrentX, avatarCurrentY);

	//check if avatar is within 8 tiles away
	std::list<PathmapTile*> emptyList;
	int distanceFound;
	gPacman->GetWorld()->PathfindDestination(ghostTile, avatarTile, emptyList, distanceFound, ORANGE_GHOST_TO_AVATAR_PROXIMITY_TOLERANCE);

	if (distanceFound == INT_MAX)
	{
		//avatar is found within ORANGE_GHOST_TO_AVATAR_PROXIMITY tiles
		//go to chase mode
		BaseGhostBehaviour::ChaseState(dt, directionUnitVector);
	}
	else
	{
		//perform scatter behaviour (but note that ghost is still in chase state)
		BaseGhostBehaviour::ScatterState(dt, directionUnitVector);
	}
}

void OrangeGhostBehaviour::ScatterState(float dt, Vector2f & directionUnitVector)
{
	ChaseState(dt, directionUnitVector); // orange ghost scatter and chase state is dependent on its proximity to pacman. So we just use chase state logic
}
