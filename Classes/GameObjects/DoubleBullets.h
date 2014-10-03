#ifndef __DOUBLE_BULLETS_h__
#define __DOUBLE_BULLETS_h__

#include "cocos2d.h"
#include "GameObjects/EnemyBase.h"
class DoubleBullets :public cocos2d::Sprite
{
public:
	DoubleBullets(void);
	~DoubleBullets(void);
	bool init()override;
	CREATE_FUNC(DoubleBullets);
	void move();
	/*bool testhurt(EnemyBase*);*/
private:
	float vy;
};

#endif // __DOUBLE_BULLETS_h__