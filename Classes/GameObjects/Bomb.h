#ifndef __BOMB__H__
#define __BOMB__H__

#include "cocos2d.h"


class Bomb : public cocos2d::Sprite
{
	CC_SYNTHESIZE(cocos2d::Vec2, velocity, Velocity);

public :

	void ShootBomb();

	virtual void BombUpdate(float dt);
	static Bomb* createWithSpriteFrameName(const char* spriteFrameName);
};

#endif //__BOMB__H__
