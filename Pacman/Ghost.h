#ifndef GHOST_H
#define GHOST_H

#include <list>
#include "MovableGameEntity.h"
#include "StateHelper.h"

class World;
class PathmapTile;
class BaseGhost;

typedef enum {
	UNDEFINED = -1,
	GHOST_START, //
	GHOST_CHASE, //chase the avatar
	GHOST_SCATTER, //spread out to a predetermined corner of the maze
	GHOST_FRIGHTENED //power pill eaten by the avatar
} GhostState;

class Ghost : public MovableGameEntity
{
public:
	Ghost(const Vector2f& aPosition);
	~Ghost(void);

	void Update(float aTime, World* aWorld);

	bool myIsDeadFlag;

	void SetImage(const char* anImage);

	void Die(World* aWorld);

	void Draw(Drawer* aDrawer);

	void SetIsClaimableFlag(bool value);
	bool GetIsClaimableFlag();

private:

	int myDesiredMovementX;
	int myDesiredMovementY;

	bool myIsClaimableFlag;

	std::list<PathmapTile*> myPath;

	BaseGhost* mGhostBehaviour;

	StateHelper<GhostState> mGhostState;

};

#endif // GHOST_H