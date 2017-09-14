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

	virtual void HomeStateInit(float dt);
	virtual void HomeState(float dt, Vector2f & directionUnitVector);
	virtual void HomeStateCleanup(float dt);

	virtual void ExitStateInit(float dt);
	virtual void ExitState(float dt, Vector2f & directionUnitVector);
	virtual void ExitStateCleanup(float dt);

	virtual void ChaseStateInit(float dt);
	virtual void ChaseState(float dt, Vector2f & directionUnitVector);
	virtual void ChaseStateCleanup(float dt);

	virtual void FrightenedStateInit(float dt);
	virtual void FrightenedState(float dt, Vector2f & directionUnitVector);
	virtual void FrightenedStateCleanup(float dt);

	virtual void ScatterStateInit(float dt);
	virtual void ScatterState(float dt, Vector2f & directionUnitVector);
	virtual void ScatterStateCleanup(float dt);

	virtual void Update(float dt);

	virtual const char* GetNormalImageName();
	const char* GetFrightenedImageName();
	const char* GetDeadImageName();

	TileCoord GetTileCurrentTargetTile();

	void SetReverseFlag();

	float GetBehaviourSpeedModifier();

	virtual void ResetBehaviour();
	void SoftResetBehaviour();

	void SetBlinkFlag(bool blinkMode);
	bool GetBlinkFlag() const;
protected:
	void SetScatterTileCoord(TileCoord& scatterTileCoord);
	int GetIndirectMagnitude(TileCoord a, TileCoord b);

	//!!@potentialy change these x,y corrdinates to use Tilecoord
	int mCurrentTileTargetX;
	int mCurrentTileTargetY;

	Ghost* mGhostParent;

	int mPreviousDirectionUnitVecX;
	int mPreviousDirectionUnitVecY;

	//this is for use on a two exit tile for which the ghost should move in the same direction or
	//exit by the other available way (at corners). If all else fails, then the ghost will retreat back in the same direction
	void MoveInSameDirection(PathmapTile* ghostParentCurrentTile, Vector2f& directionUnitVector);

	void MoveInSameDirectionVertically(PathmapTile* ghostParentCurrentTile, Vector2f& directionUnitVector);

	void ReverseDirection(PathmapTile* ghostParentCurrentTile, Vector2f& directionUnitVector);

	float mBehaviourSpeedMultiplier;

	bool mReverseFlag; //this forces ghost to reverse their direction

private:

	TileCoord mScatterTargetTileCoord;
	
	bool mBlinkModeFlag;
	bool mBlinkType;	//false would be transparent and true would be 255
	float mBlinkTimer;
};



#endif