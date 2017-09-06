#ifndef DRAWER_H
#define DRAWER_H

#include "AssetManager.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;

class Drawer
{
public:
	static Drawer* Create(SDL_Window* aWindow, SDL_Renderer* aRenderer, AssetManager* aAssetManager);
	~Drawer(void);

	void Draw(const char* anImage, int aCellX = 0, int aCellY = 0);
	void DrawText(const char* aText, const char* aFontFile, int aX, int aY);

private:
	Drawer(SDL_Window* aWindow, SDL_Renderer* aRenderer, AssetManager* aAssetManager);
	bool Init();
	
	SDL_Window* myWindow;
	SDL_Renderer* myRenderer;
	SDL_Surface* world;
	AssetManager* myAssetManager;
};

#endif // DRAWER_H