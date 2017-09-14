#ifndef ORANGEGHOSTBEHAVIOUR_H
#define ORANGEGHOSTBEHAVIOUR_H

#include "BaseGhostBehaviour.h"

//this class is designed to be "plugged" into the Ghost class to incorporate the
//behaviour of the orange ghost

//The orange ghost fickle targeting is made by having its two states(Chase and scatter) switch back and forth 
//based on his proximity to the Avatar and not based on a timer like BaseGhostBehaviour
class OrangeGhostBehaviour : public BaseGhostBehaviour
{
public:
	OrangeGhostBehaviour(Ghost* ghostParent);

	const char* GetNormalImageName() override;

	void ChaseState(float dt, Vector2f & directionUnitVector) override;

	void ScatterState(float dt, Vector2f & directionUnitVector) override;
private:
};

#endif
