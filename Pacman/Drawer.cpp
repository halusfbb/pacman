#include "Drawer.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "assert.h"

Drawer* Drawer::Create(SDL_Window* aWindow, SDL_Renderer* aRenderer, AssetManager* aAssetManager)
{
	Drawer* drawer = new Drawer(aWindow, aRenderer, aAssetManager);

	if (!drawer->Init())
	{
		delete drawer;
		drawer = NULL;
	}

	return drawer;
}

Drawer::Drawer(SDL_Window* aWindow, SDL_Renderer* aRenderer, AssetManager* aAssetManager)
: myWindow(aWindow)
, myRenderer(aRenderer)
, myAssetManager(aAssetManager)
{
}

Drawer::~Drawer(void)
{
}

bool Drawer::Init()
{
	if (!myWindow)
		return false;

	return true;
}

void Drawer::Draw(const char* anImage, int aCellX, int aCellY)
{
	AssetStructPtr assetStructPtr = myAssetManager->GetAsset(anImage);
	if (!assetStructPtr)
	{
		//this could mean that the asset has not been added to the AssetManager
		assetStructPtr = myAssetManager->AddAsset(anImage);

		if (!assetStructPtr)
		{
			SDL_Log("Getting/Adding asset from AssetManager Failed");
			assert(0 && "Getting/Adding asset from AssetManager Failed");
			return;
		}
	}
	SDL_Texture* optimizedSurface = assetStructPtr->sdlTexture;
	SDL_Surface* surface = assetStructPtr->sdlSurface;
	if (!optimizedSurface || !surface)
	{
		SDL_Log("Unable to Render");
		return;
	}

    SDL_Rect sizeRect;
    sizeRect.x = 0 ;
    sizeRect.y = 0 ;
    sizeRect.w = surface->w ;
    sizeRect.h = surface->h ;

    SDL_Rect posRect ;
    posRect.x = aCellX;
    posRect.y = aCellY;
	posRect.w = sizeRect.w;
	posRect.h = sizeRect.h;

	SDL_RenderCopy(myRenderer, optimizedSurface, &sizeRect, &posRect);	
}

void Drawer::DrawText(const char* aText, const char* aFontFile, int aX, int aY)
{
	TTF_Font* font=TTF_OpenFont(aFontFile, 24);

	SDL_Color fg={255,0,0,255};
	SDL_Surface* surface = TTF_RenderText_Solid(font, aText, fg);

	SDL_Texture* optimizedSurface = SDL_CreateTextureFromSurface(myRenderer, surface);

    SDL_Rect sizeRect;
    sizeRect.x = 0 ;
    sizeRect.y = 0 ;
    sizeRect.w = surface->w ;
    sizeRect.h = surface->h ;

    SDL_Rect posRect ;
    posRect.x = aX;
    posRect.y = aY;
	posRect.w = sizeRect.w;
	posRect.h = sizeRect.h;

	SDL_RenderCopy(myRenderer, optimizedSurface, &sizeRect, &posRect);
	SDL_DestroyTexture(optimizedSurface);
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
}
