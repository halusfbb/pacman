#include "ImageAssetCache.h"
#include "SDL_image.h"
#include "SDL.h"

ImageAssetCacheSPtr ImageAssetCache::CreateImageAssetCacheFromFile(std::string& fileName, SDL_Renderer* sdlRenderer)
{
	ImageAssetCache* imageAssetCache = new ImageAssetCache(fileName, sdlRenderer);

	if (!imageAssetCache->Init())
	{
		delete imageAssetCache;
		return NULL;
	}

	return std::make_shared<ImageAssetCache>(*imageAssetCache);
}

ImageAssetCache::ImageAssetCache(std::string& fileName, SDL_Renderer* sdlRenderer)
:AssetCache(fileName, sdlRenderer)
{
}

SDL_Texture * ImageAssetCache::GetTexture() const
{
	return mSDLTexture;
}

bool ImageAssetCache::Init()
{
	SDL_Surface* sdlSurface = IMG_Load(mAssetName.c_str());
	if (!sdlSurface)
	{
		SDL_Log("IMG_Load Error: %s\n", IMG_GetError());
		return false;
	}
	
	SDL_Texture* sdlTexture = SDL_CreateTextureFromSurface(mRenderer, sdlSurface);
	if (!sdlTexture)
	{
		SDL_Log("Failed to create Texture\n");
		return false;
	}
	
	mSDLTexture = sdlTexture;
	
	return true;
}

void ImageAssetCache::CleanUpAssets()
{
	delete mSDLTexture;
}
