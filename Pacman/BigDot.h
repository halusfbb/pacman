#ifndef BIGDOT_H
#define BIGDOT_H

#include "StaticGameEntity.h"
#include "Vector2f.h"
#include "ImageAssetCache.h"

class BigDot : public StaticGameEntity
{
public:
	BigDot(Vector2f aPosition);
	void Init();

	void Draw();

	~BigDot(void);
private:
	ImageAssetCacheSPtr mImageAssetCache;
};

#endif // BIGDOT_H