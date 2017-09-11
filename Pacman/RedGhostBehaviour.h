#ifndef REDGHOSTBEHAVIOUR_H
#define REDGHOSTBEHAVIOUR_H

#include "BaseGhostBehaviour.h"

//this class is designed to be "plugged" into the Ghost class to incorporate the
//behaviour of the red ghost

class RedGhostBehaviour : public BaseGhostBehaviour
{
public:
	RedGhostBehaviour(Ghost* ghostParent);

	const char* GetNormalImageName() override;

private:
};

#endif
