#ifndef PATHMAPTILE_H
#define PATHMAPTILE_H

#include <vector>

typedef struct TC {
	int x;
	int y;

	TC& operator+(const TC& rhs) const { return TC{ x + rhs.x, y + rhs.y }; }
	TC& operator-(const TC& rhs) const { return TC{ x - rhs.x, y - rhs.y }; }
} TileCoord;

class PathmapTile
{
public:
	PathmapTile(int anX, int anY, bool aIsBlockingFlag, bool aIsStartZoneFlag, bool myIsGateFlag, bool myIsLoopFlag);
	~PathmapTile(void);

	int myX;
	int myY;
	std::vector<TileCoord> myValidNeighbours;
	bool myIsBlockingFlag;
	bool myIsVisitedFlag;
	bool myIsStartZoneFlag; //indicates the home location of the ghosts
	bool myIsGateFlag;	//this is the gate that ghosts have to cross to get from spawning zone to the maze
	bool myLoopFlag; //this is the loop tile where avatar and ghosts can loop around the maze by entering the tile ahead

};

#endif // PATHMAPTILE_H