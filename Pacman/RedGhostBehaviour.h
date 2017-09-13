#ifndef REDGHOSTBEHAVIOUR_H
#define REDGHOSTBEHAVIOUR_H

#include "BaseGhostBehaviour.h"

//this class is designed to be "plugged" into the Ghost class to incorporate the
//behaviour of the red ghost

//the red ghosts increases in speed as the after a number of dots are consumed at two defined points and will stay in chase mode indefinitely at the second
class RedGhostBehaviour : public BaseGhostBehaviour
{
public:
	RedGhostBehaviour(Ghost* ghostParent);

	const char* GetNormalImageName() override;

	void ChaseState(float dt, Vector2f & directionUnitVector) override ;
	void ScatterState(float dt, Vector2f & directionUnitVector) override;

	void Update(float dt) override;

	void ResetBehaviour() override;
private:
	bool mRestrictToChaseMode;
	bool mFirstUpgrade;
	bool mSecondUpgrade;
};

#endif
