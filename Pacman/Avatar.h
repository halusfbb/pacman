#ifndef AVATAR_H
#define AVATAR_H

#include "MovableGameEntity.h"
#include "Vector2f.h"
#include "AssetManager.h"

typedef enum
{
	AVATAR_FACING_RIGHT = 0,
	AVATAR_FACING_DOWN,
	AVATAR_FACING_LEFT,
	AVATAR_FACING_UP
} eAvatarFacing;

class Avatar : public MovableGameEntity
{
public:
	Avatar(const Vector2f& aPosition);
	~Avatar(void);

	void Init() override;

	void Update(float aTime);

private:
	//this determines if avatar should bite down depending on its position within a cell
	bool ShouldAvatarBite() const;
	 
	//determinds which avatar asset to be used and assigns it to the active image to be drawn
	void SetAnimation();

	//takes in a direction vector of the avatars movement and stores where
	//avatar is currently facing
	void UpdateFacingDirection(Vector2f vec);
	eAvatarFacing mEnumAvatarFacing;
	ImageAssetCacheSPtr arrImageCache[8];
};

#endif //AVATAR_H