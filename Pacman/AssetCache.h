#ifndef ASSETCACHE_H
#define ASSETCACHE_H

#include <string>
#include <memory>

struct SDL_Renderer;

//base class for asset caching. The AssetManager will hold a map 
//of pointers of classes derived from AssetCache. The role of
//AssetCache and its derivatives is to hold game entity specific data
class AssetCache
{
public: 
	SDL_Renderer* GetRenderer() const;
	virtual void CleanUpAssets() = 0;
	std::string getImageName() const;

protected:
	AssetCache(std::string assetName, SDL_Renderer* sdlRenderer);
	virtual ~AssetCache();

	std::string mAssetName;
	SDL_Renderer* mRenderer;
};

typedef std::shared_ptr<AssetCache> AssetCacheSPtr;

#endif ASSETCACHE_H