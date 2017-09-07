#include "AssetManager.h"
#include "SDL_image.h"
#include "ImageAssetCache.h"
#include "assert.h"

AssetManager* AssetManager::assetManagerInstance = NULL;

AssetManager* AssetManager::Create(SDL_Renderer* aRenderer)
{
	assetManagerInstance = new AssetManager(aRenderer);

	if (!assetManagerInstance->Init())
	{
		delete assetManagerInstance;
		return NULL;
	}

	return assetManagerInstance;
}

AssetManager * AssetManager::GetInstance()
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

AssetCacheSPtr AssetManager::AddImageAsset(std::string assetName)
{
	//search if the asset already exists
	auto search = mMapAsset.find(assetName);
	if (search != mMapAsset.end())
	{
		return search->second; //asset already added
	}
	
	ImageAssetCacheSPtr imageAssetCacheSPtr = ImageAssetCache::CreateImageAssetCacheFromFile(assetName, mRenderer);
	if (!imageAssetCacheSPtr)
		return NULL;

	AssetCacheSPtr assetCacheSPtr = imageAssetCacheSPtr;
	mMapAsset.insert(std::make_pair(assetName, assetCacheSPtr));
	
	return imageAssetCacheSPtr;
}

AssetCacheSPtr AssetManager::GetImageAsset(const std::string& assetName)
{
	//search if the asset already exists
	auto search = mMapAsset.find(assetName);
	if (search != mMapAsset.end())
	{
		return search->second;
	}
	else
	{
		return AddImageAsset(assetName);
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

