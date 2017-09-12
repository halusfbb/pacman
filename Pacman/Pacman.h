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
	GAME_STATE_LOSE_LIFE,
	GAME_STATE_LOSE,
	GAME_STATE_WIN
} PacmanGameState;

#ifdef _DEBUG
extern const char* gGameStateChar[];
#endif

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

	float myTimerToNextState;
};

extern Pacman* gPacman;

#endif // PACMAN_H