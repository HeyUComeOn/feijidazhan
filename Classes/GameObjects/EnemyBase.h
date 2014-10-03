#ifndef EnemyBase_h__
#define EnemyBase_h__

#include "cocos2d.h"

class EnemyBase:public cocos2d::Sprite
{
public:
	EnemyBase(void);
	~EnemyBase(void);

	virtual bool init()override;
	CREATE_FUNC(EnemyBase);

	void Destory();
	void hurt(int damg);
	void initEnemy(int Enetype);
	void setHP();
	void Move();
	void BlowupAction();
	CC_SYNTHESIZE_READONLY(bool,isBlowUp,IsBlowUp);
	CC_SYNTHESIZE_READONLY(int,hp,HP);
	CC_SYNTHESIZE(int, enemyType,EnemyType);
	
private:
		float vy;
		
};

#endif // EnemyBase_h__
