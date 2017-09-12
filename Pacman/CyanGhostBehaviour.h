#ifndef CYANGHOSTBEHAVIOUR_H
#define CYANGHOSTBEHAVIOUR_H

#include "BaseGhostBehaviour.h"

//this class is designed to be "plugged" into the Ghost class to incorporate the
//behaviour of the cyan ghost

class CyanGhostBehaviour : public BaseGhostBehaviour
{
public:
	CyanGhostBehaviour(Ghost* ghostParent);

	const char* GetNormalImageName() override;

private:
};

#endif
