#ifndef PACMAN_H
#define PACMAN_H

#include "Vector2f.h"
#include "StateHelper.h"

struct SDL_Surface;
class Drawer;
class Avatar;
class World;
class GhostManager;

typedef enum {
	GAME_STATE_INIT,
	GAME_STATE_START,
	GAME_STATE_PLAY,
	GAME_STATE_DEATH_ANIMATION,
	GAME_STATE_DEATH_END,
	GAME_STATE_LOSE,
	GAME_STATE_WIN
} PacmanGameState;

class Pacman
{
public:
	static Pacman* Create(Drawer* aDrawer);
	~Pacman(void);

	bool Update(float aTime);
	bool Draw();

	void SetNextMovement(Vector2f& vec);

	const Avatar* GetAvatar() { return myAvatar; }
	World* GetWorld() { return myWorld; }

	float GetFrightenedGhostCounter();
private:
	Pacman(Drawer* aDrawer);
	bool Init();
	bool UpdateInput();
	void MoveAvatar();
	bool CheckEndGameCondition();

	Drawer* myDrawer;

	float myGhostGhostCounter;

	int myLives;
	int myScore;
	int myFps;

	Vector2f myNextMovement;

	Avatar* myAvatar;
	GhostManager* myGhostManager;
	World* myWorld;

	StateHelper<PacmanGameState> myPacmanGameState;
};

extern Pacman* gPacman;

#endif // PACMAN_H