#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>
#include "SDL.h"

struct SDL_Renderer;

//struct for keeping track of an asset's surface and texture
typedef struct
{
	std::string assetName;
	SDL_Surface* sdlSurface;
	SDL_Texture* sdlTexture;
} AssetStruct;
typedef AssetStruct* AssetStructPtr;

//this class manages assets
class AssetManager
{
public:

	//create function
	static AssetManager* Create(SDL_Renderer* aRenderer);
	bool Init();
	~AssetManager();

	//add new item to the asset manager by its filename. 
	//Returns a const pointer to AssetStruct for the asset added
	const AssetStructPtr AddAsset(std::string assetName);
	//remove item from the asset manager by its filename. 
	//Returns true if successfull, false if the asset doesn't exist in the first place
	bool RemoveAsset(const std::string& assetName);
	//fetch an AssetStruct from the asset name
	//Returns  a const pointer to AssetStruct for the asset or NULL if the Asset cannot be found
	const AssetStructPtr GetAsset(const std::string& assetName) const;

private:
	AssetManager(SDL_Renderer* aRenderer):mRenderer(aRenderer) {};

	//this cleans up dynamically allocated resources for the asset struct
	void CleanupAssetStruct(AssetStructPtr assetStructPtr);

	std::map<std::string, AssetStructPtr> mMapAsset;
	
	SDL_Renderer* mRenderer;
};

#endif
