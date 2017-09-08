#include "Avatar.h"

Avatar::Avatar(const Vector2f& aPosition)
: MovableGameEntity(aPosition, "open_32.png")
, mEnumAvatarFacing(AVATAR_FACING_RIGHT) //this corresponds to open_32.png 
{

}

Avatar::~Avatar(void)
{
}

void Avatar::Init()
{
	//the avatar will have textures set according to indices calculated in SetAnimation()
	arrImage[0] = AssetManager::GetInstance()->GetImageAsset("open_32.png");
	arrImage[1] = AssetManager::GetInstance()->GetImageAsset("closed_32.png");
	arrImage[2] = AssetManager::GetInstance()->GetImageAsset("open_down_32.png");
	arrImage[3] = AssetManager::GetInstance()->GetImageAsset("closed_down_32.png");
	arrImage[4] = AssetManager::GetInstance()->GetImageAsset("open_left_32.png");
	arrImage[5] = AssetManager::GetInstance()->GetImageAsset("closed_left_32.png");
	arrImage[6] = AssetManager::GetInstance()->GetImageAsset("open_up_32.png");
	arrImage[7] = AssetManager::GetInstance()->GetImageAsset("closed_up_32.png");

	GameEntity::Init();
}

void Avatar::Update(float aTime)
{
	int tileSize = 22;

	Vector2f destination(myNextTileX * tileSize, myNextTileY * tileSize);
	Vector2f direction = destination - myPosition;

	float distanceToMove = aTime * 30.f;
	
	UpdateFacingDirection(direction);

	if (distanceToMove > direction.Length())
	{
		myPosition = destination;
		myCurrentTileX = myNextTileX;
		myCurrentTileY = myNextTileY;
	}
	else
	{
		direction.Normalize();
		myPosition += direction * distanceToMove;
	}

	SetAnimation();
}

void Avatar::UpdateFacingDirection(Vector2f vec)
{
	if (vec.myX == 0 && vec.myY == 0)
	{
		//do not change facing direction
		return;
	}

	if (vec.myX > 0)
	{
		mEnumAvatarFacing = AVATAR_FACING_RIGHT;
	}
	else if (vec.myX < 0)
	{
		mEnumAvatarFacing = AVATAR_FACING_LEFT;
	}
	else if (vec.myY > 0)
	{
		mEnumAvatarFacing = AVATAR_FACING_DOWN;
	}
	else
	{
		mEnumAvatarFacing = AVATAR_FACING_UP;
	}
}

void Avatar::SetAnimation()
{
	int subIndex = ShouldAvatarBite() ? 0 : 1;

	int majorIndex = mEnumAvatarFacing;

	myImageAssetCache = arrImage[majorIndex * 2 + subIndex];
}

bool Avatar::ShouldAvatarBite() const
{
	if (mEnumAvatarFacing == AVATAR_FACING_RIGHT || mEnumAvatarFacing == AVATAR_FACING_LEFT)
	{
		if ((int)myPosition.myX % 22 < 11)
		{
			return true; 
		}
		else
		{
			return false;
		}
	}
	else
	{
		if ((int)myPosition.myY % 22 < 11)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}