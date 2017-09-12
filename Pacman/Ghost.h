#ifndef GHOST_H
#define GHOST_H

#include <list>
#include "MovableGameEntity.h"
#include "StateHelper.h"

class World;
class PathmapTile;
class BaseGhostBehaviour;

typedef enum {
	GHOST_STATE_UNDEFINED = -1,
	GHOST_START, //
	GHOST_CHASE, //chase the avatar
	GHOST_SCATTER, //spread out to a predetermined corner of the maze
	GHOST_FRIGHTENED //power pill eaten by the avatar
} GhostState;

#ifdef _DEBUG
extern const char* gGhostStateChar[];
#endif

typedef enum {
	GHOST_COLOR_UNDEFINED = -1,
	GHOST_RED,
	GHOST_PINK,
	GHOST_CYAN,
	GHOST_ORANGE,
	GHOST_GRAY
} GhostColor;

class Ghost : public MovableGameEntity
{
public:
	static Ghost* Create(const Vector2f& aPosition, GhostColor ghostColor);

	Ghost(const Vector2f& aPosition, GhostColor ghostColor);
	~Ghost(void);

	void Init() override;

	void Update(float aTime, World* aWorld);

	bool myIsDeadFlag;

	void Die(World* aWorld);

	void Draw(Drawer* aDrawer);

	void SetIsClaimableFlag(bool value);
	bool GetIsClaimableFlag();

	void SetNextState(GhostState ghostState);
	GhostState GetCurrentState();

private:

	int myDesiredMovementX;
	int myDesiredMovementY;

	bool myIsClaimableFlag;

	std::list<PathmapTile*> myPath;

	BaseGhostBehaviour* mGhostBehaviour;

	StateHelper<GhostState> mGhostState;

	GhostColor mGhostColor;
};

#endif // GHOST_H