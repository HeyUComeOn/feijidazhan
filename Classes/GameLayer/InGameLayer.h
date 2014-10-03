#ifndef InGameLayer_h__
#define InGameLayer_h__

#define GameSceneNodeTagFighter						900
#define GameSceneNodeTagMenuBomb					901
#define GameSceneNodeTagNumLabel						902
#define GameSceneNodeBatchTagBullet					903
#define GameSceneNodeBatchTagEnemy					904
#define GameSceneNodeBatchTagBomb1					905
#define GameSceneNodeBatchTagBomb2					906
#define GameSceneNodeBatchTagLayer					907
#define GameSceneNodeBatchTagBackground		908

#include "cocos2d.h"
#include "GameObjects/FlyPlane.h"
#include "GameObjects/EnemyBase.h"
#include "GameObjects/bullet.h"
#include "GameObjects/DoubleBullets.h"
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
	void onEnter();
	void onExit();
	void onEnterTransitionDidFinish();
	void createEnemys(float delta);
	void createbullets(float delta);
	void moveAllEnemys(float delta);
	void moveAllbullets();
	void bulletAttackEnemy(EnemyBase*);
	void EnemyAttackPlane();
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)override;
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)override;
	//void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)override;
	void cleanAllEnemys();
	void GameOver();
	void update(float delta);
	void MenuBombCallback();
	void createBombs(float delta);
	void BombAttackPlane();
	void updateDoubleBulletsIsOn(float delta);
	void MenuPauseCallback();
	void Resume(cocos2d::Ref*Sender);
	void Restart(cocos2d::Ref*Sender);
public:
	cocos2d::Vector<EnemyBase*> EneVec;
	cocos2d::Vector<bullet*> bulletVec;
	cocos2d::Vector<DoubleBullets*> DoubleBulletVec;
private:
	cocos2d::Vec2 m_Pos;
	FlyPlane*hero;
	int score;
	cocos2d::Label* scoreLabel;
	cocos2d::EventListenerPhysicsContact *contactListener;
	cocos2d::EventListenerTouchOneByOne *listener1;
	int bombNum;
	bool DoubleBulletsIsOn;
};

#endif // InGameLayer_h__
