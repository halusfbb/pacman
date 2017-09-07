#ifndef IMAGEASSETCACHE_H
#define IMAGEASSETCACHE_H

#include "AssetCache.h"

struct SDL_Texture;

class ImageAssetCache : public AssetCache
{
public:
	static ImageAssetCache* CreateImageAssetCacheFromFile(std::string fileName, SDL_Renderer* sdlRenderer);
	SDL_Texture* GetTexture() const;

private:
	ImageAssetCache(std::string fileName, SDL_Renderer* sdlRenderer);
	bool Init();
	void CleanUpAssets() override;
	SDL_Texture* mSDLTexture;
};

#endif