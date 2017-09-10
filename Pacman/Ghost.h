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
	GHOST_START,
	GHOST_CHASE,
	GHOST_SCATTER,
	GHOST_FRIGHTENED
} GhostState;

class Ghost : public MovableGameEntity
{
public:
	Ghost(const Vector2f& aPosition);
	~Ghost(void);

	void Update(float aTime, World* aWorld);

	bool myIsClaimableFlag;
	bool myIsDeadFlag;

	void SetImage(const char* anImage);

	void Die(World* aWorld);

	void Draw(Drawer* aDrawer);

	void SetPosition(const Vector2f& aPosition) override;


private:

	int myDesiredMovementX;
	int myDesiredMovementY;

	std::list<PathmapTile*> myPath;

	//!!@temporary declaration
	BaseGhost* mGhostBehaviour;

	StateHelper<GhostState> mGhostState;

};

#endif // GHOST_H