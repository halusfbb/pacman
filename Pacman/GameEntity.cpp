#include "GameEntity.h"
#include "Drawer.h"
#include "AssetManager.h"

GameEntity::GameEntity(const Vector2f& aPosition, const char* anImage)
:myPosition(aPosition)
,myImage(anImage)
,myIdMarkedForDeleteFlag(false)
,myImageAssetCache(NULL)
{
}

GameEntity::~GameEntity(void)
{
}

void GameEntity::Init()
{
	if ((myImage != NULL) && (myImage[0] != '\0'))
		myImageAssetCache = AssetManager::GetInstance()->GetImageAsset(myImage);
}

void GameEntity::SetAlpha(Uint8 alpha)
{
	myImageAssetCache->SetAlpha(alpha);
}

void GameEntity::SetImage(const char * anImage)
{
	if (myImageAssetCache)
	{
		if (strcmp(anImage, myImageAssetCache->getImageName().c_str()) == 0)
			return; // image has no changes
	}
	
	myImageAssetCache = std::dynamic_pointer_cast<ImageAssetCache>(AssetManager::GetInstance()->GetImageAsset(anImage));
}

bool GameEntity::Intersect(GameEntity* aGameEntity)
{
	return false;	
}

void GameEntity::Draw(Drawer* aDrawer)
{
	aDrawer->Draw(myImageAssetCache, (int)myPosition.myX + 220, (int)myPosition.myY + 60);
}
