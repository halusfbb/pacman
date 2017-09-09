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

bool GameEntity::Intersect(GameEntity* aGameEntity)
{
	return false;	
}

void GameEntity::Draw(Drawer* aDrawer)
{
	aDrawer->Draw(myImageAssetCache, (int)myPosition.myX + 220, (int)myPosition.myY + 60);
}
