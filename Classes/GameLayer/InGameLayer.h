#ifndef InGameLayer_h__
#define InGameLayer_h__

#include "cocos2d.h"
#include "GameObjects/FlyPlane.h"
#include "GameObjects/EnemyBase.h"
#include "GameObjects/bullet.h"
class InGameLayer:public cocos2d::Layer
{
public:
	InGameLayer(void);
	~InGameLayer(void);

	virtual bool init()override;
	CREATE_FUNC(InGameLayer);

	static cocos2d::Scene*scene();

	void setViews();

	void addEvents();
	void onEnterTransitionDidFinish();
	void createEnemys(float t);
	void createbullets(float t);
	void moveAllEnemys(float t);
	void moveAllbullets(float t);
	void bulletAttackEnemy();
	void EnemyAttackPlane();
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)override;
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)override;
	//void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)override;
	void cleanAllEnemys();
	void GameOver();
	void update(float t);
public:
	cocos2d::Vector<EnemyBase*> EneVec;
	cocos2d::Vector<bullet*> bulletVec;
private:
	cocos2d::Vec2 m_Pos;
	FlyPlane*hero;
};

#endif // InGameLayer_h__
