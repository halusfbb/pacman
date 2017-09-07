#ifndef DOT_H
#define DOT_H

#include "StaticGameEntity.h"
#include "Vector2f.h"
#include "ImageAssetCache.h"

class ImageAssetCache;

class Dot : public StaticGameEntity
{
public:
	Dot(Vector2f aPosition);
	void Init();

	void Draw();

	~Dot(void);
private:
	ImageAssetCacheSPtr mImageAssetCache;
};

#endif // DOT_H