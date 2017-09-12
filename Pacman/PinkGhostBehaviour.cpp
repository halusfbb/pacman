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
