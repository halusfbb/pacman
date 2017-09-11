#ifndef BASEGHOSTBEHAVIOUR_H
#define BASEGHOSTBEHAVIOUR_H

#include "Vector2f.h"
#include "PathmapTile.h"

class Ghost;

//this class is designed to be "plugged" into the Ghost class to incorporate the
//behaviour of a simple base ghost.
class BaseGhostBehaviour
{
public:
	BaseGhostBehaviour(Ghost* ghostParent);

	virtual void ChaseStateInit(float dt);
	virtual void ChaseState(float dt, Vector2f & directionUnitVector);
	virtual void ChaseStateCleanup(float dt);

	virtual void FrightenedStateInit(float dt);
	virtual void FrightenedState(float dt, Vector2f & directionUnitVector);
	virtual void FrightenedStateCleanup(float dt);

	virtual void ScatterStateInit(float dt);
	virtual void ScatterState(float dt, Vector2f & directionUnitVector);
	virtual void ScatterStateCleanup(float dt);

	virtual const char* GetNormalImageName();
	const char* GetFrightenedImageName();
	const char* GetDeadImageName();

	TileCoord GetTileCurrentTargetTile();

protected:
	void SetScatterTileCoord(TileCoord& scatterTileCoord);

private:

	//this is for use on a two exit tile for which the ghost should move in the same direction or
	//exit by the other available way (at corners). If all else fails, then the ghost will retreat back in the same direction
	void MoveInSameDirection(PathmapTile* ghostParentCurrentTile, Vector2f& directionUnitVector);

	Ghost* mGhostParent;
	//!!@potentialy change these x,y corrdinates to use Tilecoord
	int mCurrentTileTargetX;
	int mCurrentTileTargetY;

	int mPreviousDirectionUnitVecX;
	int mPreviousDirectionUnitVecY;

	TileCoord mScatterTargetTileCoord;

};



#endif