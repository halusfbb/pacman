#ifndef BASEGHOST_H
#define BASEGHOST_H

#include "Vector2f.h"

class Ghost;

//this class is designed to be "plugged" into the Ghost class to incorporate the
//behaviour of a simple base ghost.
class BaseGhost
{
public:
	BaseGhost(Ghost* ghostParent);

	virtual void ChaseStateInit(float dt);
	virtual void ChaseState(float dt, Vector2f & directionUnitVector);
	virtual void ChaseStateCleanup(float dt);
private:

	void ScatterStateInit();
	void ScatterState();
	void ScatterStateCleanup();

	void FrightenedStateInit();
	void FrightenedState();
	void FrightenedStateCleanup();

	Ghost* mGhostParent;
	int mCurrentTileTargetX;
	int mCurrentTileTargetY;

	int mPreviousDirectionUnitVecX;
	int mPreviousDirectionUnitVecY;
};



#endif