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
	GAME_STATE_UNDEFINED = -1,
	GAME_STATE_INIT,  //does a hard reset of game items
	GAME_STATE_START, //displays "Get Ready" and does a soft reset of game items
	GAME_STATE_PLAY,
	GAME_STATE_LOSE_LIFE,
	GAME_STATE_LOSE, //no lives left
	GAME_STATE_WIN	//all dots eaten
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

	const Avatar* GetAvatar() const { return myAvatar; }
	World* GetWorld() const { return myWorld; }
	const GhostManager* GetGhostManager() const { return myGhostManager; }

	float GetFrightenedGhostCounter();
private:
	Pacman(Drawer* aDrawer);
	bool Init();
	bool UpdateInput();
	void MoveAvatar();
	bool CheckEndGameCondition();

	Drawer* myDrawer;

	float myGhostGhostCounter;
	bool myGhostCounterCountdown;

	int myLives;
	int myScore;
	int myFps;

	Vector2f myNextMovement;
	Vector2f myPrevMovement;

	Avatar* myAvatar;
	GhostManager* myGhostManager;
	World* myWorld;

	StateHelper<PacmanGameState> myPacmanGameState;

	float myTimerToNextState;
};

extern Pacman* gPacman;

#endif // PACMAN_H