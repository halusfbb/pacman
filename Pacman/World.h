#ifndef WORLD_H
#define WORLD_H

#include <list>
#include <vector>
#include "Vector2f.h"
#include "ImageAssetCache.h"

class Drawer;
class PathmapTile;
class Dot;
class BigDot;
class Cherry;

class World
{
public:
	World(void);
	~World(void);

	void Init();

	void Draw(Drawer* aDrawer);
	bool TileIsValid(int anX, int anY);

	bool HasIntersectedDot(const Vector2f& aPosition);
	bool HasIntersectedBigDot(const Vector2f& aPosition);
	bool HasIntersectedCherry(const Vector2f& aPosition);

	void Update();

	void GetPath(int aFromX, int aFromY, int aToX, int aToY, std::list<PathmapTile*>& aList);
	PathmapTile* GetTile(int aFromX, int aFromY);

	int GetMapRowSize() const;
	int GetMapColSize() const;

	const std::list<Dot*>& GetListOfDots() const;
	const std::list<BigDot*> GetListOfBigDots() const;

	void ResetToOriginalDots();

private:

	int myMapRowSize;
	int myMapColSize;

	bool Pathfind(PathmapTile* aFromTile, PathmapTile* aToTile, std::list<PathmapTile*>& aList);
	bool ListDoesNotContain(PathmapTile* aFromTile, std::list<PathmapTile*>& aList);

	bool InitPathmap();
	bool InitDots();
	bool InitBigDots();
	void InitBoard();

	std::vector<PathmapTile*> myPathmapTiles; //instantiate the correct number of slots change back to list?
	std::list<Dot*> myDots;
	std::list<BigDot*> myBigDots;
	std::list<Cherry*> myCherry;
	std::list<Dot*> myOriginalDots;
	std::list<BigDot*> myOrignalBigDots;

	ImageAssetCacheSPtr mBoardImageAssetCache;

};

#endif // WORLD_H