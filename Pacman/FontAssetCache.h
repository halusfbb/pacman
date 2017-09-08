#ifndef FONTASSETCACHE_H
#define FONTASSETCACHE_H

#include "AssetCache.h"
#include "sdl_ttf.h"

class FontAssetCache;
typedef std::shared_ptr<FontAssetCache> FontAssetCacheSPtr;

class FontAssetCache : public AssetCache
{
public:
	static FontAssetCacheSPtr CreateFontAssetCacheFromFile(std::string& fileName, int fontSize, SDL_Renderer* sdlRenderer);
	TTF_Font* GetFont() const;

private:
	FontAssetCache(std::string& fileName, int fontSize, SDL_Renderer* sdlRenderer);
	bool Init();
	void CleanUpAssets() override;
	TTF_Font* mFont;
	int mFontSize;
};


#endif
