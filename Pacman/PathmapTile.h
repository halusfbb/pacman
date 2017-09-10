#ifndef PATHMAPTILE_H
#define PATHMAPTILE_H

#include <vector>

typedef struct TC {
	int x;
	int y;

	TC& operator+(const TC& rhs) const { return TC{ x + rhs.x, y + rhs.y }; }
	TC& operator-(const TC& rhs) const { return TC{ x - rhs.x, y - rhs.y }; }
} TileCoord; //!!@consider using this to replace PathmapTile::myY and ::mX

class PathmapTile
{
public:
	PathmapTile(int anX, int anY, bool aIsBlockingFlag, bool aIsStartZoneFlag, bool myIsGateFlag);
	~PathmapTile(void);

	int myX;
	int myY;
	std::vector<TileCoord> myValidNeighbours;
	bool myIsBlockingFlag;
	bool myIsVisitedFlag;
	bool myIsStartZoneFlag;
	bool myIsGateFlag;

};

#endif // PATHMAPTILE_H