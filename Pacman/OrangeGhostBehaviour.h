#ifndef ORANGEGHOSTBEHAVIOUR_H
#define ORANGEGHOSTBEHAVIOUR_H

#include "BaseGhostBehaviour.h"

//this class is designed to be "plugged" into the Ghost class to incorporate the
//behaviour of the orange ghost

class OrangeGhostBehaviour : public BaseGhostBehaviour
{
public:
	OrangeGhostBehaviour(Ghost* ghostParent);

	const char* GetNormalImageName() override;

private:
};

#endif
