#include "BigDot.h"
#include "AssetManager.h"
#include "SDL.h"

BigDot::BigDot(Vector2f aPosition)
: StaticGameEntity(aPosition, "Big_Dot_32.png")
{
}

void BigDot::Init()
{
	mImageAssetCache = std::dynamic_pointer_cast<ImageAssetCache>(AssetManager::GetInstance()->GetImageAsset(myImage));
}

void BigDot::Draw()
{

	SDL_Rect sizeRect;
	sizeRect.x = 0;
	sizeRect.y = 0;
	sizeRect.w = mImageAssetCache->GetWidth();
	sizeRect.h = mImageAssetCache->GetHeight();

	SDL_Rect posRect;
	posRect.x = myPosition.myX + 220; //!!@ Hardcoded values
	posRect.y = myPosition.myY + 60; //!!@ Hardcoded values
	posRect.w = sizeRect.w;
	posRect.h = sizeRect.h;

	SDL_RenderCopy(mImageAssetCache->GetRenderer(), mImageAssetCache->GetTexture(), &sizeRect, &posRect);
}

BigDot::~BigDot(void)
{
}
