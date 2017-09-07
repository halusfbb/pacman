#include "AssetCache.h"
#include "SDL.h"

AssetCache::AssetCache(std::string assetName, SDL_Renderer * sdlRenderer)
:mAssetName(assetName)
,mRenderer(sdlRenderer)
,mWidth(0)
,mHeight(0)
{
}

AssetCache::~AssetCache()
{
}

SDL_Renderer* AssetCache::GetRenderer() const
{
	return mRenderer;
}

int AssetCache::GetWidth() const
{
	return mWidth;
}

int AssetCache::GetHeight() const
{
	return mHeight;
}
