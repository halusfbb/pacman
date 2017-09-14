#ifndef CYANGHOSTBEHAVIOUR_H
#define CYANGHOSTBEHAVIOUR_H

#include "BaseGhostBehaviour.h"

//this class is designed to be "plugged" into the Ghost class to incorporate the
//behaviour of the cyan ghost

//the blue ghost chases the avatar based on the line interpolated from the red ghost and a tile that is 2 tiles ahead of the avatars facing direction.
//The interpolated line is twice in length as to the length adjoining the two points mentioned. 
class CyanGhostBehaviour : public BaseGhostBehaviour
{
public:
	CyanGhostBehaviour(Ghost* ghostParent);

	const char* GetNormalImageName() override;

	void ChaseState(float dt, Vector2f & directionUnitVector) override;

private:
};

#endif
