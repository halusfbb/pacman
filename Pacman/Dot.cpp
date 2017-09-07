#include "Dot.h"
#include "AssetManager.h"
#include "SDL.h"

Dot::Dot(Vector2f aPosition)
: StaticGameEntity(aPosition, "Small_Dot_32.png")
{
}

void Dot::Init()
{
	mImageAssetCache = std::dynamic_pointer_cast<ImageAssetCache>(AssetManager::GetInstance()->GetImageAsset(myImage));
}

void Dot::Draw()
{

	SDL_Rect sizeRect;
	sizeRect.x = 0 ;
	sizeRect.y = 0 ;
	sizeRect.w = mImageAssetCache->GetWidth();
	sizeRect.h = mImageAssetCache->GetHeight();

	SDL_Rect posRect ;
	posRect.x = myPosition.myX + 220; //!!@ Hardcoded values
	posRect.y = myPosition.myY + 60; //!!@ Hardcoded values
	posRect.w = sizeRect.w;
	posRect.h = sizeRect.h;

	SDL_RenderCopy(mImageAssetCache->GetRenderer(), mImageAssetCache->GetTexture(), &sizeRect, &posRect);
}

Dot::~Dot(void)
{
}
