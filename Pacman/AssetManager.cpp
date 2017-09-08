#include "AssetManager.h"
#include "SDL_image.h"
#include "ImageAssetCache.h"
#include "FontAssetCache.h"
#include "assert.h"

AssetManager* AssetManager::assetManagerInstance = NULL;

AssetManager* AssetManager::Create(SDL_Renderer* aRenderer)
{
	if (assetManagerInstance)
		return assetManagerInstance;

	assetManagerInstance = new AssetManager(aRenderer);

	if (!assetManagerInstance->Init())
	{
		delete assetManagerInstance;
		return NULL;
	}

	return assetManagerInstance;
}

AssetManager* AssetManager::GetInstance()
{
	return assetManagerInstance;
}

bool AssetManager::Init()
{
	if (!mRenderer)
	{
		return false;
	}

	return true;
}

AssetManager::AssetManager(SDL_Renderer* aRenderer)
:mRenderer(aRenderer)
{
}

ImageAssetCacheSPtr AssetManager::AddImageAsset(std::string assetName)
{
	//search if the asset already exists
	auto search = mMapAsset.find(assetName);
	if (search != mMapAsset.end())
	{
		return std::dynamic_pointer_cast<ImageAssetCache>(search->second); //asset already added
	}
	
	ImageAssetCacheSPtr imageAssetCacheSPtr = ImageAssetCache::CreateImageAssetCacheFromFile(assetName, mRenderer);
	if (!imageAssetCacheSPtr)
		return NULL;

	AssetCacheSPtr assetCacheSPtr = imageAssetCacheSPtr;
	mMapAsset.insert(std::make_pair(assetName, assetCacheSPtr));
	
	return std::dynamic_pointer_cast<ImageAssetCache>(assetCacheSPtr);
}

FontAssetCacheSPtr AssetManager::AddFontAsset(std::string fontAssetName, int fontSize)
{
	//search if the asset already exists
	std::string concatenatedFontName = fontAssetName + ":" + std::to_string(fontSize);
	auto search = mMapAsset.find(concatenatedFontName);
	if (search != mMapAsset.end())
	{
		return std::dynamic_pointer_cast<FontAssetCache>(search->second); //asset already added
	}

	FontAssetCacheSPtr fontAssetCacheSPtr = FontAssetCache::CreateFontAssetCacheFromFile(fontAssetName, fontSize, mRenderer);
	if (!fontAssetCacheSPtr)
		return NULL;

	AssetCacheSPtr assetCacheSPtr = fontAssetCacheSPtr;
	mMapAsset.insert(std::make_pair(concatenatedFontName, assetCacheSPtr));


	return std::dynamic_pointer_cast<FontAssetCache>(assetCacheSPtr);
}

ImageAssetCacheSPtr AssetManager::GetImageAsset(const std::string& assetName)
{
	//search if the asset already exists
	auto search = mMapAsset.find(assetName);
	if (search != mMapAsset.end())
	{
		return std::dynamic_pointer_cast<ImageAssetCache>(search->second); //asset already added
	}
	else
	{
		return AddImageAsset(assetName);
	}
}

FontAssetCacheSPtr AssetManager::GetFontAsset(const std::string& fontAssetName, int fontSize)
{
	//search if the asset already exists
	std::string concatenatedFontName = fontAssetName + ":" + std::to_string(fontSize);

	//search if the asset already exists
	auto search = mMapAsset.find(concatenatedFontName);
	if (search != mMapAsset.end())
	{
		return std::dynamic_pointer_cast<FontAssetCache>(search->second);
	}
	else
	{
		return AddFontAsset(fontAssetName, fontSize);
	}
}

AssetManager::~AssetManager()
{
	for (auto pair : mMapAsset)
	{
		pair.second->CleanUpAssets();
	}
}

bool AssetManager::RemoveImageAsset(const std::string& assetName)
{
	//search if the asset already exists
	auto search = mMapAsset.find(assetName);
	if (search != mMapAsset.end())
	{
		search->second->CleanUpAssets();
		mMapAsset.erase(search);
		return true; //asset exists and have been removed
	}
	else
	{
		return false; //asset doesn't exist
	}
}

bool AssetManager::RemoveFontAsset(const std::string & fontAssetName, int fontSize)
{
	std::string concatenatedFontName = fontAssetName + ":" + std::to_string(fontSize);

	auto search = mMapAsset.find(concatenatedFontName);
	if (search != mMapAsset.end())
	{
		search->second->CleanUpAssets();
		mMapAsset.erase(search);
		return true; //asset exists and have been removed
	}
	else
	{
		return false; //asset doesn't exist
	}
}

