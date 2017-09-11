#ifndef BASEGHOST_H
#define BASEGHOST_H

#include "Vector2f.h"

class Ghost;
class PathmapTile;

//this class is designed to be "plugged" into the Ghost class to incorporate the
//behaviour of a simple base ghost.
class BaseGhost
{
public:
	BaseGhost(Ghost* ghostParent);

	virtual void ChaseStateInit(float dt);
	virtual void ChaseState(float dt, Vector2f & directionUnitVector);
	virtual void ChaseStateCleanup(float dt);

	void FrightenedStateInit(float dt);
	void FrightenedState(float dt, Vector2f & directionUnitVector);
	void FrightenedStateCleanup(float dt);
private:

	void ScatterStateInit();
	void ScatterState();
	void ScatterStateCleanup();

	//this is for use on a two exit tile for which the ghost should move in the same direction or
	//exit by the other available way (at corners). If all else fails, then the ghost will retreat back in the same direction
	void MoveInSameDirection(PathmapTile* ghostParentCurrentTile, Vector2f& directionUnitVector);

	Ghost* mGhostParent;
	int mCurrentTileTargetX;
	int mCurrentTileTargetY;

	int mPreviousDirectionUnitVecX;
	int mPreviousDirectionUnitVecY;
};



#endif