#ifndef GHOSTMANAGER_H
#define GHOSTMANAGER_H

#include <vector>
#include "Ghost.h"

class Ghost;
class World;
class Drawer;

class GhostManager
{
public:
	static GhostManager* Create();

	GhostManager();

	void Init();

	void Update(float aTime);
	void Draw(Drawer* aDrawer);

	void SetGhostsNextState(GhostState ghostState);
	void RevertToPreviousState(); //revert to the cyclic state given in mCyclic_Chase_Scatter
	void SwapCyclicState();

	const std::vector<Ghost*>& GetVectorOfGhost();
	const std::vector<Ghost*>& GetVectorOfHomeGhost();

	void ResetCycleCount();

private:

	float GetDurationForChase_ScatterState(GhostState ghoststate);
	float GetDurationForGhostRelease();

	std::vector<Ghost*> mGhostvec;
	std::vector<Ghost*> mGhostAtHomevec;

	GhostState mCurrentState;
	GhostState mCyclic_Chase_Scatter; //this keeps track of the current cycle of chase and scatter
	
	float mCurrentCycleStateTimer;

	int mNoOf_Chase_Scatter_Cycles;

	float mTimerBeforeGhostExit;
};
#endif
