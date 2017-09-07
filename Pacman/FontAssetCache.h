#ifndef FONTASSETCACHE_H
#define FONTASSETCACHE_H

#include "AssetCache.h"

struct TTF_Font;
class FontAssetCache;
typedef std::shared_ptr<FontAssetCache> FontAssetCacheSPtr;

class FontAssetCache : public AssetCache
{
private:
	TTF_Font* mFont;
};


#endif
