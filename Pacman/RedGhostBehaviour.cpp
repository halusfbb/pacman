#include "RedGhostBehaviour.h"
#include "globals.h"
#include "Avatar.h"
#include "Pacman.h"
#include "Ghost.h"
#include "World.h"

RedGhostBehaviour::RedGhostBehaviour(Ghost * ghostParent)
	:BaseGhostBehaviour(ghostParent)
{
	SetScatterTileCoord(TileCoord{ 24, -2 });
}

const char * RedGhostBehaviour::GetNormalImageName()
{
	return RED_GHOST_IMAGE;
}