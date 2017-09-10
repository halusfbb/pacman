#include "PathmapTile.h"

PathmapTile::PathmapTile(int anX, int anY, bool aIsBlockingFlag, bool myIsStartZoneFlag, bool myIsGateFlag)
: myX(anX)
, myY(anY)
, myIsBlockingFlag(aIsBlockingFlag)
, myIsStartZoneFlag(myIsStartZoneFlag)
, myIsGateFlag(myIsGateFlag)
, myIsVisitedFlag(false)
{
}

PathmapTile::~PathmapTile(void)
{
}
