#include "GameEntity.h"
#include "Drawer.h"
#include "AssetManager.h"
#include "globals.h"

GameEntity::GameEntity(const Vector2f& aPosition, const char* anImage)
:myPosition(aPosition)
,myImage(anImage)
,myIdMarkedForDeleteFlag(false)
,myImageAssetCache(NULL)
,myAlpha(255)
{
}

GameEntity::~GameEntity(void)
{
}

void GameEntity::Init()
{
	if ((myImage != NULL) && (myImage[0] != '\0'))
		SetImage(myImage);
}

void GameEntity::SetAlpha(Uint8 alpha)
{
	myAlpha = alpha;
}

void GameEntity::SetImage(const char * anImage)
{
	if (myImageAssetCache)
	{
		if (strcmp(anImage, myImageAssetCache->getImageName().c_str()) == 0)
			return; // image has no changes
	}
	
	myImageAssetCache = std::dynamic_pointer_cast<ImageAssetCache>(AssetManager::GetInstance()->GetImageAsset(anImage));
	myImage = anImage;
}

const char * GameEntity::GetImageName() const
{
	const char* imageTmp = myImageAssetCache->getImageName().c_str();
	return myImageAssetCache->getImageName().c_str();
}

bool GameEntity::Intersect(GameEntity* aGameEntity)
{
	return false;	
}

void GameEntity::Draw(Drawer* aDrawer)
{
	myImageAssetCache->SetAlpha(myAlpha);
	aDrawer->Draw(myImageAssetCache, (int)myPosition.myX + ENTITIY_OFFSET_SIZE_X, (int)myPosition.myY + ENTITIY_OFFSET_SIZE_Y);  //!!@ Hardcoded values
}
