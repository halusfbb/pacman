#include "AssetManager.h"
#include "SDL_image.h"
#include "assert.h"

AssetManager* AssetManager::Create(SDL_Renderer* aRenderer)
{
	AssetManager* assetManager = new AssetManager(aRenderer);

	if (!assetManager->Init())
	{
		delete assetManager;
		return NULL;
	}

	return assetManager;
}

bool AssetManager::Init()
{
	if (!mRenderer)
	{
		return false;
	}

	return true;
}

AssetManager::~AssetManager()
{
	for (auto pair : mMapAsset)
	{
		CleanupAssetStruct(pair.second);
	}
}

const AssetStructPtr AssetManager::AddAsset(std::string assetName)
{
	//search if the asset already exists
	auto search = mMapAsset.find(assetName);
	if (search != mMapAsset.end())
	{
		return search->second; //asset already added
	}

	AssetStructPtr assetStructPtr = new AssetStruct;
	assetStructPtr->assetName = assetName;
	assetStructPtr->sdlSurface = IMG_Load(assetName.c_str());
	if (!assetStructPtr->sdlSurface)
	{
		SDL_Log("IMG_Load Error: %s\n", IMG_GetError());
	}

	assetStructPtr->sdlTexture = SDL_CreateTextureFromSurface(mRenderer, assetStructPtr->sdlSurface);
	if (!assetStructPtr->sdlTexture)
	{
		SDL_Log("Failed to create Texture\n");
	}

	mMapAsset.insert(std::make_pair(assetName, assetStructPtr));

	return assetStructPtr;
}

bool AssetManager::RemoveAsset(const std::string& assetName)
{
	//search if the asset already exists
	auto search = mMapAsset.find(assetName);
	if (search != mMapAsset.end())
	{
		CleanupAssetStruct(search->second);
		mMapAsset.erase(search);
		return true; //asset exists and have been removed
	}
	else
	{
		return false; //asset doesn't exist
	}
}

const AssetStructPtr AssetManager::GetAsset(const std::string& assetName) const
{
	//search if the asset already exists
	auto search = mMapAsset.find(assetName);
	if (search != mMapAsset.end())
	{
		return search->second;
	}
	else
	{
		return NULL;
	}
}

void AssetManager::CleanupAssetStruct(AssetStructPtr assetStructPtr)
{
	SDL_DestroyTexture(assetStructPtr->sdlTexture);
	SDL_FreeSurface(assetStructPtr->sdlSurface);
}

