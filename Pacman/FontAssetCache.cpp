#include "FontAssetCache.h"

FontAssetCacheSPtr FontAssetCache::CreateFontAssetCacheFromFile(std::string & fileName, int fontSize, SDL_Renderer * sdlRenderer)
{
	FontAssetCache* fontAssetCache = new FontAssetCache(fileName, fontSize, sdlRenderer);

	if (!fontAssetCache->Init())
	{
		delete fontAssetCache;
		return NULL;
	}

	return std::make_shared<FontAssetCache>(*fontAssetCache);
}

TTF_Font * FontAssetCache::GetFont() const
{
	return mFont;
}

FontAssetCache::FontAssetCache(std::string& fileName, int fontSize, SDL_Renderer* sdlRenderer)
:AssetCache(fileName, sdlRenderer)
,mFontSize(fontSize)
,mFont(NULL)
{
}

bool FontAssetCache::Init()
{
	mFont = TTF_OpenFont(mAssetName.c_str(), mFontSize);

	return true;
}

void FontAssetCache::CleanUpAssets()
{
	TTF_CloseFont(mFont);
}
