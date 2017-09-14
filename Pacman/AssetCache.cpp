#include "AssetCache.h"
#include "SDL.h"

AssetCache::AssetCache(std::string assetName, SDL_Renderer * sdlRenderer)
:mAssetName(assetName)
,mRenderer(sdlRenderer)
{
}

AssetCache::~AssetCache()
{
}

SDL_Renderer* AssetCache::GetRenderer() const
{
	return mRenderer;
}

const std::string& AssetCache::getImageName() const
{
	return mAssetName;
}
