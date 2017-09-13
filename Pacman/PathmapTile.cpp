#include "PathmapTile.h"

PathmapTile::PathmapTile(int anX, int anY, bool aIsBlockingFlag, bool myIsStartZoneFlag, bool myIsGateFlag, bool myIsLoopFlag)
: myX(anX)
, myY(anY)
, myIsBlockingFlag(aIsBlockingFlag)
, myIsStartZoneFlag(myIsStartZoneFlag)
, myIsGateFlag(myIsGateFlag)
, myLoopFlag(myIsLoopFlag)
{
}

PathmapTile::~PathmapTile(void)
{
}
