#include "World.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "PathmapTile.h"
#include "Dot.h"
#include "BigDot.h"
#include "Drawer.h"
#include "AssetManager.h"
#include "SDL.h"

World::World(void)
{
}

World::~World(void)
{
}

void World::Init()
{
	InitPathmap();
	InitDots();
	InitBigDots();
	InitBoard();
}

bool World::InitPathmap()
{
	std::string line;
	std::ifstream myfile ("map.txt");
	if (myfile.is_open())
	{
		int lineIndex = 0;
		while (!myfile.eof())
		{
			std::getline(myfile, line);
			for (unsigned int i = 0; i < line.length(); i++)
			{
				PathmapTile* tile = new PathmapTile(i, lineIndex, (line[i] == 'x'), (line[i] == 's'), line[i] == 'g');
				myPathmapTiles.push_back(tile);
			}

			lineIndex++;
		}
		myfile.close();
	}
	
	//iterate over each tile and determine how many valid tiles are its neighbours
	int x, y;
	for (auto pathmapTile : myPathmapTiles)
	{
		x = pathmapTile->myX;
		y = pathmapTile->myY;

		if (!TileIsValid(x, y))
			continue;

		//keeping the order of Up, Left, Down, Right in accordance to classical pacman pathfinding
		if (TileIsValid(x, y - 1))
		{
			pathmapTile->myValidNeighbours.push_back(TileCoord{x, y - 1});
		}

		if (TileIsValid(x - 1, y))
		{
			pathmapTile->myValidNeighbours.push_back(TileCoord{x - 1, y});
		}

		if (TileIsValid(x, y + 1))
		{
			pathmapTile->myValidNeighbours.push_back(TileCoord{x, y + 1});
		}

		if (TileIsValid(x + 1, y))
		{
			pathmapTile->myValidNeighbours.push_back(TileCoord{x + 1, y});
		}
	}

	return true;
}

bool World::InitDots()
{
	std::string line;
	std::ifstream myfile ("map.txt");
	if (myfile.is_open())
	{
		int lineIndex = 0;
		while (! myfile.eof() )
		{
			std::getline (myfile,line);
			for (unsigned int i = 0; i < line.length(); i++)
			{
				if (line[i] == '.')
				{
					Dot* dot = new Dot(Vector2f(i*22, lineIndex*22));
					dot->Init();
					myDots.push_back(dot);
				}
			}

			lineIndex++;
		}
		myfile.close();
	}

	return true;
}

bool World::InitBigDots()
{
	std::string line;
	std::ifstream myfile ("map.txt");
	if (myfile.is_open())
	{
		int lineIndex = 0;
		while (! myfile.eof() )
		{
			std::getline (myfile,line);
			for (unsigned int i = 0; i < line.length(); i++)
			{
				if (line[i] == 'o')
				{
					BigDot* dot = new BigDot(Vector2f(i*22, lineIndex*22));
					dot->Init();
					myBigDots.push_back(dot);
				}
			}

			lineIndex++;
		}
		myfile.close();
	}

	return true;
}

void World::InitBoard()
{
	mBoardImageAssetCache = std::dynamic_pointer_cast<ImageAssetCache>(AssetManager::GetInstance()->GetImageAsset("playfield.png"));
}

void World::Draw(Drawer* aDrawer)
{
	SDL_Rect sizeRect;
	sizeRect.x = 0;
	sizeRect.y = 0;
	sizeRect.w = mBoardImageAssetCache->GetWidth();
	sizeRect.h = mBoardImageAssetCache->GetHeight();

	SDL_Rect posRect;
	posRect.x = 0;
	posRect.y = 0;
	posRect.w = sizeRect.w;
	posRect.h = sizeRect.h;

	SDL_RenderCopy(mBoardImageAssetCache->GetRenderer(), mBoardImageAssetCache->GetTexture(), &sizeRect, &posRect);

	for(std::list<Dot*>::iterator list_iter = myDots.begin(); list_iter != myDots.end(); list_iter++)
	{
		Dot* dot = *list_iter;
		dot->Draw(aDrawer);
	}

	for(std::list<BigDot*>::iterator list_iter = myBigDots.begin(); list_iter != myBigDots.end(); list_iter++)
	{
		BigDot* dot = *list_iter;
		dot->Draw(aDrawer);
	}
}

bool World::TileIsValid(int anX, int anY)
{
	for(std::list<PathmapTile*>::iterator list_iter = myPathmapTiles.begin(); list_iter != myPathmapTiles.end(); list_iter++)
	{
		PathmapTile* tile = *list_iter;

		if (anX == tile->myX && anY == tile->myY && !tile->myIsBlockingFlag)
			return true;
	}

	return false;
}

bool World::HasIntersectedDot(const Vector2f& aPosition)
{
	for(std::list<Dot*>::iterator list_iter = myDots.begin(); list_iter != myDots.end(); list_iter++)
	{
		Dot* dot = *list_iter;
		if ((dot->GetPosition() - aPosition).Length() < 5.f)
		{
			myDots.remove(dot);
			delete dot;
			return true;
		}
	}

	return false;
}

bool World::HasIntersectedBigDot(const Vector2f& aPosition)
{
	for(std::list<BigDot*>::iterator list_iter = myBigDots.begin(); list_iter != myBigDots.end(); list_iter++)
	{
		BigDot* dot = *list_iter;
		if ((dot->GetPosition() - aPosition).Length() < 5.f)
		{
			myBigDots.remove(dot);
			delete dot;
			return true;
		}
	}

	return false;
}

bool World::HasIntersectedCherry(const Vector2f& aPosition)
{
	return true;
}

void World::GetPath(int aFromX, int aFromY, int aToX, int aToY, std::list<PathmapTile*>& aList)
{
	PathmapTile* fromTile = GetTile(aFromX, aFromY);
	PathmapTile* toTile = GetTile(aToX, aToY);

	for(std::list<PathmapTile*>::iterator list_iter = myPathmapTiles.begin(); list_iter != myPathmapTiles.end(); list_iter++)
	{
		PathmapTile* tile = *list_iter;
		tile->myIsVisitedFlag = false;
	}

	Pathfind(fromTile, toTile, aList);
}

PathmapTile* World::GetTile(int aFromX, int aFromY)
{
	for(std::list<PathmapTile*>::iterator list_iter = myPathmapTiles.begin(); list_iter != myPathmapTiles.end(); list_iter++)
	{
		PathmapTile* tile = *list_iter;
		if (tile->myX == aFromX && tile->myY == aFromY)
		{
			return tile;
		}
	}

	return NULL;
}

bool World::ListDoesNotContain(PathmapTile* aFromTile, std::list<PathmapTile*>& aList)
{
	for(std::list<PathmapTile*>::iterator list_iter = aList.begin(); list_iter != aList.end(); list_iter++)
	{
		PathmapTile* tile = *list_iter;
		if (tile == aFromTile)
		{
			return false;
		}
	}

	return true;
}

bool SortFromGhostSpawn(PathmapTile* a, PathmapTile* b)
{
	int la = abs(a->myX - 13) + abs(a->myY - 13);
	int lb = abs(b->myX - 13) + abs(b->myY - 13);

    return la < lb;
}

bool World::Pathfind(PathmapTile* aFromTile, PathmapTile* aToTile, std::list<PathmapTile*>& aList)
{
	aFromTile->myIsVisitedFlag = true;

	if (aFromTile->myIsBlockingFlag)
		return false;

	if (aFromTile == aToTile)
		return true;

	std::list<PathmapTile*> neighborList;

	PathmapTile* up = GetTile(aFromTile->myX, aFromTile->myY - 1);
	if (up && !up->myIsVisitedFlag && !up->myIsBlockingFlag && ListDoesNotContain(up, aList))
		neighborList.push_front(up);

	PathmapTile* down = GetTile(aFromTile->myX, aFromTile->myY + 1);
	if (down && !down->myIsVisitedFlag && !down->myIsBlockingFlag && ListDoesNotContain(down, aList))
		neighborList.push_front(down);

	PathmapTile* right = GetTile(aFromTile->myX + 1, aFromTile->myY);
	if (right && !right->myIsVisitedFlag && !right->myIsBlockingFlag && ListDoesNotContain(right, aList))
		neighborList.push_front(right);

	PathmapTile* left = GetTile(aFromTile->myX - 1, aFromTile->myY);
	if (left && !left->myIsVisitedFlag && !left->myIsBlockingFlag && ListDoesNotContain(left, aList))
		neighborList.push_front(left);

	neighborList.sort(SortFromGhostSpawn);

	for(std::list<PathmapTile*>::iterator list_iter = neighborList.begin(); list_iter != neighborList.end(); list_iter++)
	{
		PathmapTile* tile = *list_iter;

		aList.push_back(tile);

		if (Pathfind(tile, aToTile, aList))
			return true;

		aList.pop_back();
	}

	return false;
}