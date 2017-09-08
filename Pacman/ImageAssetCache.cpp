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
, mWidth(0)
, mHeight(0)
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
	
	mWidth = sdlSurface->w;
	mHeight = sdlSurface->h;

	SDL_Texture* sdlTexture = SDL_CreateTextureFromSurface(mRenderer, sdlSurface);
	if (!sdlTexture)
	{
		SDL_Log("Failed to create Texture\n");
		return false;
	}
	
	SDL_FreeSurface(sdlSurface);
	mSDLTexture = sdlTexture;
	
	return true;
}

void ImageAssetCache::CleanUpAssets()
{
	SDL_DestroyTexture(mSDLTexture);
}

int ImageAssetCache::GetWidth() const
{
	return mWidth;
}

int ImageAssetCache::GetHeight() const
{
	return mHeight;
}