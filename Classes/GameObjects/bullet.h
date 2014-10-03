#ifndef bullet_h__
#define bullet_h__

#include "cocos2d.h"
#include "GameObjects/EnemyBase.h"
class bullet :public cocos2d::Sprite
{
public:
	bullet(void);
	~bullet(void);
	bool init()override;
	CREATE_FUNC(bullet);
	void move();
	/*bool testhurt(EnemyBase*);*/
private:
	float vy;
};

#endif // bullet_h__