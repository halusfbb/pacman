#include "RedGhostBehaviour.h"
#include "globals.h"

RedGhostBehaviour::RedGhostBehaviour(Ghost * ghostParent)
	:BaseGhostBehaviour(ghostParent)
{
	SetScatterTileCoord(TileCoord{ 24, -1 });
}

const char * RedGhostBehaviour::GetNormalImageName()
{
	return RED_GHOST_IMAGE;
}
