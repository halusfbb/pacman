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
