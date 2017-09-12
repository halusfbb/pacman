#include "CyanGhostBehaviour.h"
#include "globals.h"
#include "Avatar.h"
#include "Pacman.h"
#include "Ghost.h"
#include "World.h"

CyanGhostBehaviour::CyanGhostBehaviour(Ghost * ghostParent)
	:BaseGhostBehaviour(ghostParent)
{
	SetScatterTileCoord(TileCoord{ 26, 30 });
}

const char * CyanGhostBehaviour::GetNormalImageName()
{
	return CYAN_GHOST_IMAGE;
}
