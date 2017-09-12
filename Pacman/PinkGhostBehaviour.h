#ifndef PINKGHOSTBEHAVIOUR_H
#define PINKGHOSTBEHAVIOUR_H

#include "BaseGhostBehaviour.h"

//this class is designed to be "plugged" into the Ghost class to incorporate the
//behaviour of the pink ghost

class PinkGhostBehaviour : public BaseGhostBehaviour
{
public:
	PinkGhostBehaviour(Ghost* ghostParent);

	const char* GetNormalImageName() override;

private:
};

#endif
