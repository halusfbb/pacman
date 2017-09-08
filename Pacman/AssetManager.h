#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <unordered_map>
#include <string>
#include "AssetCache.h"
#include "ImageAssetCache.h"
#include "FontAssetCache.h"

struct SDL_Renderer;

//this class manages assets
class AssetManager
{
public:

	//create function
	static AssetManager* Create(SDL_Renderer* aRenderer);
	static AssetManager* assetManagerInstance;
	static AssetManager* GetInstance();
	bool Init();
	~AssetManager();

	//remove item from the asset manager by its filename. 
	//Returns true if successfull, false if the asset doesn't exist in the first place
	bool RemoveImageAsset(const std::string& assetName);
	bool RemoveFontAsset(const std::string& fontAssetName, int fontSize);

	//fetch an AssetStruct from the asset name. Will attempt to add asset to the manager
	//if it is not found
	//Returns  a const pointer to AssetStruct for the asset or NULL if the Asset cannot be added
	ImageAssetCacheSPtr GetImageAsset(const std::string& assetName);
	FontAssetCacheSPtr GetFontAsset(const std::string& fontAssetName, int fontSize);

private:
	AssetManager(SDL_Renderer* aRenderer);

	//add new item to the asset manager by its filename. 
	//Returns a const pointer to AssetStruct for the asset added
	ImageAssetCacheSPtr AddImageAsset(std::string assetName);
	FontAssetCacheSPtr AddFontAsset(std::string fontAssetName, int fontSize);

	std::unordered_map<std::string, AssetCacheSPtr> mMapAsset;
	
	SDL_Renderer* mRenderer;
};

#endif
