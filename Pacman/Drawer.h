#ifndef DRAWER_H
#define DRAWER_H

#include "ImageAssetCache.h"
#include "SDL.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;

class Drawer
{
public:
	static Drawer* Create(SDL_Window* aWindow, SDL_Renderer* aRenderer);
	~Drawer(void);

	void Draw(ImageAssetCacheSPtr imageAssetCache, int aCellX = 0, int aCellY = 0);
	void DrawText(const char* aText, const char* aFontFile, int aX, int aY, int fontSize, SDL_Color color, bool isCenterAlign=false);

	SDL_Renderer* myRenderer;
private:
	Drawer(SDL_Window* aWindow, SDL_Renderer* aRenderer);
	bool Init();
	
	SDL_Window* myWindow;
	SDL_Surface* world;
};

#endif // DRAWER_H