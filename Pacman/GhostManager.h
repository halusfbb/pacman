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

private:

	float GetDurationForChase_ScatterState(GhostState ghoststate);

	std::vector<Ghost*> mGhostvec;

	GhostState mCurrentState;
	GhostState mCyclic_Chase_Scatter; //this keeps track of the current cycle of chase and scatter
	
	float mCurrentCycleStateTimer;

	int mNoOf_Chase_Scatter_Cycles;
};
#endif
