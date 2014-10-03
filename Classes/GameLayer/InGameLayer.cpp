#include "GameLayer/InGameLayer.h"
#include "GameObjects/Background.h"
#include "VisibleRect.h"
#include "GameLayer/welcomeLayer.h"
#include "Dlog.h"
#include "GameObjects/Bomb.h"
#include "GameLayer/PauseLayer.h"
#include "SimpleAudioEngine.h"
#include "MyUtility.h"
using namespace CocosDenshion;
USING_NS_CC;

InGameLayer::InGameLayer(void):EneVec(),bulletVec(),hero(NULL),score(0),bombNum(0),DoubleBulletsIsOn(false),DoubleBulletVec()
{
}

InGameLayer::~InGameLayer(void)
{
}

bool InGameLayer::init()
{
	bool ret=false;
	do 
	{
		if(Layer::init())
		{
			auto back = Background::create();
			back->setAnchorPoint(Vec2(0,0));
			back->setPosition(Vec2(0,0));
			back->setTag(GameSceneNodeBatchTagBackground);
			addChild(back);
			ret=true;
		}
	} while (0);
	return ret;
}

void InGameLayer::onEnter()
{
	Layer::onEnter();
	setViews();
}

void InGameLayer::onExit()
{
	Layer::onExit();
	//停止调用 shootBullet函数.
	this->unschedule(schedule_selector(InGameLayer::moveAllEnemys));
	this->unschedule(schedule_selector(InGameLayer::createEnemys));
	this->unschedule(schedule_selector(InGameLayer::createbullets));
	this->unscheduleUpdate();
	//注销事件监听器.
	Director::getInstance()->getEventDispatcher()->removeEventListener(listener1);
	Director::getInstance()->getEventDispatcher()->removeEventListener(contactListener);

	auto nodes = this->getChildren();
	for (const auto& node : nodes) {
		//如果是背景不进行remove
		if (node->getTag() != GameSceneNodeBatchTagBackground) 
		{
			this->removeChild(node);
		}
	}
}

void InGameLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/game_music.mp3",true);
}

Scene*InGameLayer::scene()
{
	auto sc=Scene::createWithPhysics();
	PhysicsWorld* phyWorld = sc->getPhysicsWorld();
	//phyWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	//0,0不受到重力的影响
	phyWorld->setGravity(Vect(0,0));
	auto lay=InGameLayer::create();
	sc->addChild(lay);
	return sc;
}

//?????????研究放在init和enter中的区别对于像score这样的可变动的量
void InGameLayer::setViews()
{
	
	//////////////////添加UFO、Pause Menu start///////////////
	auto bombSp = Sprite::createWithSpriteFrameName("bomb.png");
	auto itemBomb = MenuItemSprite::create(bombSp,bombSp,CC_CALLBACK_0(InGameLayer::MenuBombCallback,this));
	itemBomb->setPosition(VisibleRect::leftBottom()+Vec2(bombSp->getContentSize().width/2,bombSp->getContentSize().height/2));

	auto menu = Menu::create(itemBomb, nullptr);
	menu->setPosition(VisibleRect::leftBottom());
	menu->setTag(GameSceneNodeTagMenuBomb);
	menu->setVisible(false);
	addChild(menu);

	auto PauseSp1 = Sprite::createWithSpriteFrameName("game_pause_nor.png");
	auto PauseSp2 = Sprite::createWithSpriteFrameName("game_pause_nor-44.png");
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto itemPause = MenuItemSprite::create(PauseSp1,PauseSp2,CC_CALLBACK_0(InGameLayer::MenuPauseCallback,this));
	itemPause->setPosition(VisibleRect::leftTop()+Vec2(PauseSp1->getContentSize().width/2, - PauseSp1->getContentSize().height/2));

	auto menu2 = Menu::create(itemPause,nullptr);
	menu2->setPosition(VisibleRect::leftBottom());
	addChild(menu2);

	auto BombNumLabel = Label::createWithTTF("x0","fonts/hanyi.ttf",36);
	BombNumLabel->setPosition(VisibleRect::leftBottom()+Vec2(bombSp->getContentSize().width/2+60,bombSp->getContentSize().height/2));
	BombNumLabel->setTag(GameSceneNodeTagNumLabel);
	BombNumLabel->setVisible(false);
	addChild(BombNumLabel);

	this->schedule(SEL_SCHEDULE(&InGameLayer::createBombs),20.0f);

	///////////////////添加UFO、Pause Menu end///////////////////////////////

	scoreLabel = Label::createWithTTF("0","fonts/hanyi.ttf",45);
	scoreLabel->setPosition(VisibleRect::leftTop()+Vec2(itemPause->getContentSize().width*2, -scoreLabel->getContentSize().height));
	addChild(scoreLabel,1000);

	/////////////////最高分 start////////////////////////
	UserDefault *defaults = UserDefault::getInstance();
	int highScore = defaults->getIntegerForKey("highscore_key");    
	if (highScore < score) {
		highScore = score;
		defaults->setIntegerForKey("highscore_key", highScore);
	}
	__String *text = __String::createWithFormat("%i", highScore);
	auto lblHighScore = Label::createWithTTF(MyUtility::getUTF8Char("lblHighScore"), "fonts/hanyi.ttf", 45);

	lblHighScore->setAnchorPoint(Vec2(0,0));

	lblHighScore->setPosition(Vec2(VisibleRect::rightTop().x-2*lblHighScore->getContentSize().width ,scoreLabel->getPosition().y-15));

	addChild(lblHighScore);

	auto lblScoreOfHigh = Label::createWithTTF(text->getCString(), "fonts/hanyi.ttf", 45);
	//lblScoreOfHigh->setColor(Color3B(75,255,255));	
	lblScoreOfHigh->setAnchorPoint(Vec2(0,0));
	lblScoreOfHigh->setPosition(lblHighScore->getPosition() + Vec2(200, 0));
	lblScoreOfHigh->setTag(GameSceneNodelblHighScore);
	addChild(lblScoreOfHigh);
	//////////////////最高分 end/////////////////////////////

	hero = FlyPlane::create();
	hero->setPosition(Vec2(VisibleRect::center().x,120));//hero的contentsize为0，为什么
	hero->setIsAlive(true);
	hero->setTag(GameSceneNodeTagFighter);
	addChild(hero);
	m_Pos = hero->getPosition();
	addEvents();

	//注册 碰撞事件监听器
	contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = [this](PhysicsContact& contact)
	{
		auto spriteA = contact.getShapeA()->getBody()->getNode();
		auto spriteB = contact.getShapeB()->getBody()->getNode();
		Node* enemy1 = nullptr;

		////////////////////////////检测 飞机与敌人的碰撞 start//////////////////////////////////
		//Dlog::showLog("A = %d  ------------ B = %d", spriteA->getTag(), spriteB->getTag());		
		if (spriteA&&spriteB&&spriteA->getTag() == GameSceneNodeTagFighter && spriteB->getTag() == GameSceneNodeBatchTagEnemy)
		{
			enemy1 = spriteB;
		}
		if (spriteA&&spriteB&&spriteA->getTag() == GameSceneNodeBatchTagEnemy && spriteB->getTag() == GameSceneNodeTagFighter)
		{
			enemy1 = spriteA;
		}
		if (enemy1 != nullptr) {//发生碰撞
			if(hero&&hero->getIsAlive())
			{
				this->EnemyAttackPlane();
			}
		
			//this->handleFighterCollidingWithEnemy((Enemy*)enemy1);
			return false;
		}
		/////////////////////////检测 飞机与敌人的碰撞 end/////////////////////////////////////

		////////////////////////////检测 炮弹与敌人的碰撞 start////////////////////////////////
		Node* enemy2 = nullptr;
		Node* bullet = nullptr;
		if (spriteA&&spriteB&&spriteA->getTag() == GameSceneNodeBatchTagBullet && 
			spriteB->getTag() == GameSceneNodeBatchTagEnemy)
		{
			//不可见的炮弹不发生碰撞
			//if (!spriteA->isVisible())
			//	return false;
			//使得炮弹消失
			//spriteA->setVisible(false);
			bullet = spriteA/*->removeFromParentAndCleanup(true)*/;
			enemy2 = spriteB;
		}
		
		if (spriteA&&spriteB&&spriteA->getTag() == GameSceneNodeBatchTagEnemy 
			&& spriteB->getTag() == GameSceneNodeBatchTagBullet)
		{
			//不可见的炮弹不发生碰撞
			//if (!spriteB->isVisible())
			//	return false;
			//使得炮弹消失
			//spriteB->setVisible(false);
			bullet = spriteB/*->removeFromParent()*/;
			enemy2 = spriteA;
		}
		if (enemy2 != nullptr) {//发生碰撞
			bullet->removeFromParent();
			this->bulletAttackEnemy((EnemyBase*)enemy2);
			//this->handleBulletCollidingWithEnemy((Enemy*)enemy2);
			return false;
		}
		/////////////////////////检测 炮弹与敌人的碰撞 end/////////////////////////////////////

		////////////////////////////检测 飞机与UFO2的碰撞 start//////////////////////////////////
		Dlog::showLog("A = %d  ------------ B = %d", spriteA->getTag(), spriteB->getTag());		
		Node* enemy3 = nullptr;
		if (spriteA&&spriteB&&spriteA->getTag() == GameSceneNodeTagFighter && spriteB->getTag() == GameSceneNodeBatchTagBomb2)
		{
			enemy3 = spriteB;
		}
		if (spriteA&&spriteB&&spriteA->getTag() == GameSceneNodeBatchTagBomb2 && spriteB->getTag() == GameSceneNodeTagFighter)
		{
			enemy3 = spriteA;
		}
		if (enemy3 != nullptr) {//发生碰撞
			SimpleAudioEngine::getInstance()->playEffect("sound/get_bomb.mp3");
			enemy3->setVisible(false);
			enemy3->removeFromParent();
			this->BombAttackPlane();
			//this->handleFighterCollidingWithEnemy((Enemy*)enemy1);
			return false;
		}
		/////////////////////////检测 飞机与UFO2的碰撞 end/////////////////////////////////////

		////////////////////////////检测 飞机与UFO1的碰撞 start//////////////////////////////////
		Dlog::showLog("A = %d  ------------ B = %d", spriteA->getTag(), spriteB->getTag());		
		Node* enemy4 = nullptr;
		if (spriteA&&spriteB&&spriteA->getTag() == GameSceneNodeTagFighter && spriteB->getTag() == GameSceneNodeBatchTagBomb1)
		{
			enemy4 = spriteB;
		}
		if (spriteA&&spriteB&&spriteA->getTag() == GameSceneNodeBatchTagBomb1 && spriteB->getTag() == GameSceneNodeTagFighter)
		{
			enemy4 = spriteA;
		}
		if (enemy4 != nullptr) {//发生碰撞
			SimpleAudioEngine::getInstance()->playEffect("sound/get_double_laser.mp3");
			enemy4->setVisible(false);
			enemy4->removeFromParent();
			this->DoubleBulletsIsOn = true;
			this->scheduleOnce(schedule_selector(InGameLayer::updateDoubleBulletsIsOn),11.0f);
			//this->handleFighterCollidingWithEnemy((Enemy*)enemy1);
			return false;
		}
		/////////////////////////检测 飞机与UFO1的碰撞 end/////////////////////////////////////
		return false;
	};	

	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithFixedPriority(contactListener,1); 
	this->schedule(SEL_SCHEDULE(&InGameLayer::createEnemys),CCRANDOM_0_1()*0.25+0.36f);
	this->schedule(SEL_SCHEDULE(&InGameLayer::moveAllEnemys),0.08f);
	this->schedule(SEL_SCHEDULE(&InGameLayer::createbullets),0.2f);
	//this->schedule(SEL_SCHEDULE(&InGameLayer::moveAllbullets),0.016f);

	this->scheduleUpdate();	
}

void InGameLayer::update(float delta)
{
	//bulletAttackEnemy();
	//EnemyAttackPlane();
	moveAllbullets();
}

void InGameLayer::createEnemys(float delta)
{
	auto newEny = EnemyBase::create();
	float ran = CCRANDOM_0_1();
	int x;
	if(ran < 0.02)
	{
		x = 0;
	}
	else if(ran >0.9)
	{
		x = 2;
	}
	else
	{
		x = 1; 
	}
	newEny->initEnemy(x);
	newEny->setTag(GameSceneNodeBatchTagEnemy);
	addChild(newEny);
	EneVec.pushBack(newEny);

}

void InGameLayer::createbullets(float delta)
{
	SimpleAudioEngine::getInstance()->playEffect("sound/bullet.mp3");
	auto visibleSize = Director::getInstance()->getVisibleSize();

	if(hero&&hero->getIsAlive())
	{
		if(!DoubleBulletsIsOn)
		{
			for (int i = DoubleBulletVec.size()-1; i>=0;i--)
			{
				auto douBullet = DoubleBulletVec.at(i);
				douBullet->setPositionY(douBullet->getPositionY()+390);
				if(douBullet->getPositionY()>=VisibleRect::top().y+douBullet->getContentSize().height/2)
					DoubleBulletVec.erase(i);
				//DoubleBulletVec.at(i)->removeFromParent();
				//DoubleBulletVec.erase(i);
			}

			auto mbullet=bullet::create();
			mbullet->setPosition(hero->getPosition());
			mbullet->setTag(GameSceneNodeBatchTagBullet);
			addChild(mbullet);
			bulletVec.pushBack(mbullet);
		}
		else
		{
			for (int i = bulletVec.size()-1; i>=0;i--)
			{
				auto Bullet = bulletVec.at(i);
				Bullet->setPositionY(Bullet->getPositionY()+390);
				if(Bullet->getPositionY()>=VisibleRect::top().y+Bullet->getContentSize().height/2)
					bulletVec.erase(i);
				//bulletVec.at(i)->removeFromParent();
				//bulletVec.erase(i);
			}

			auto mbullet1=DoubleBullets::create();
			auto mbullet2=DoubleBullets::create();

			mbullet1->setPosition(hero->getPosition()+Vec2(-1*hero->getContentSize().width/6,0));
			mbullet2->setPosition(hero->getPosition()+Vec2(1*hero->getContentSize().width/6,0));

			mbullet1->setTag(GameSceneNodeBatchTagBullet);
			mbullet2->setTag(GameSceneNodeBatchTagBullet);
			addChild(mbullet1);
			addChild(mbullet2);
			DoubleBulletVec.pushBack(mbullet1);
			DoubleBulletVec.pushBack(mbullet2);


		}

	}
}

void InGameLayer::addEvents()
{
	listener1 = EventListenerTouchOneByOne::create();
	listener1->onTouchBegan=CC_CALLBACK_2(InGameLayer::onTouchBegan,this);
	listener1->onTouchMoved=CC_CALLBACK_2(InGameLayer::onTouchMoved,this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener1,this);
}
bool InGameLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if(hero&&hero->getIsAlive())
	{
		Dlog::showLog("touch began");
		return true;
	}
	return false;
}
void InGameLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	Dlog::showLog("touch move");
	m_Pos=touch->getLocation();

	if(hero&&hero->getIsAlive()&&hero->getBoundingBox().containsPoint(m_Pos))
		hero->Moveto(m_Pos);
}

void InGameLayer::moveAllEnemys(float delta)
{
	Vector<EnemyBase*>::iterator it=EneVec.begin();
	while(it!=EneVec.end())
	{
		(*it)->Move();
		if((*it)->getPositionY()<=-(*it)->getContentSize().height/2)
		{
			(*it)->removeFromParent();
			it=EneVec.erase(it);
		}
		else
			++it;
	}
}

void InGameLayer::moveAllbullets()
{
	if (!DoubleBulletsIsOn)
	{
		Vector<bullet*>::iterator it=bulletVec.begin();
		while(it!=bulletVec.end())
		{
			(*it)->move();
			if((*it)->getPositionY()>=VisibleRect::top().y+(*it)->getContentSize().height/2)
				it=bulletVec.erase(it);
			else
				++it;
		}
	}
	else
	{
		Vector<DoubleBullets*>::iterator it=DoubleBulletVec.begin();
		while(it!=DoubleBulletVec.end())
		{
			(*it)->move();
			if((*it)->getPositionY()>=VisibleRect::top().y+(*it)->getContentSize().height/2)
				it=DoubleBulletVec.erase(it);
			else
				++it;
		}
	}

}

void InGameLayer::bulletAttackEnemy(EnemyBase*Ene)
{

	/*Vector<bullet*>::iterator iter2=bulletVec.begin();
	for(int i=EneVec.size()-1;i>=0;--i)
	{
	auto Ene=EneVec.at(i);
	for(int j=bulletVec.size()-1;j>=0;--j)//如果就j!=0会出现越界
	{
	auto bult=bulletVec.at(j);
	if(bult->testhurt(Ene))
	{

	bult->removeFromParent();
	bulletVec.erase(j);*/
	Ene->hurt(1);
	if(Ene->getIsBlowUp())
	{
		
		switch (Ene->getEnemyType())
		{
		case 0:
			{
				SimpleAudioEngine::getInstance()->playEffect("sound/enemy3_down.mp3");
				score += 50;
				break;}
		case 1:
			{
				SimpleAudioEngine::getInstance()->playEffect("sound/enemy1_down.mp3");
				score += 10;
				break;
			}
		case 2:
			{
				SimpleAudioEngine::getInstance()->playEffect("sound/enemy2_down.mp3");
				score += 20;
				break;
			}
		default:
			break;
		}
		scoreLabel->setString(__String::createWithFormat("%d",score)->getCString());
		EneVec.eraseObject(Ene);

		UserDefault *defaults = UserDefault::getInstance();
		int highScore = defaults->getIntegerForKey("highscore_key");    
		if (highScore < score) {
			highScore = score;
			defaults->setIntegerForKey("highscore_key", highScore);
		}
		__String *text = __String::createWithFormat("%i", highScore);

		auto lblScoreOfHigh = static_cast<Label*>(this->getChildByTag(GameSceneNodelblHighScore));
		lblScoreOfHigh->setString(text->getCString());
	
	}
	/*
	break;
	}
	}
	//正序删与倒序删1  2  3  4
	}
	}*/

}

void InGameLayer::EnemyAttackPlane()
{
	/*if(hero&&hero->getIsAlive())
	{
	Vector<EnemyBase*>::iterator it=EneVec.begin();
	auto heroRect=hero->getBoundingBox();
	heroRect.origin.x+=60;
	heroRect.origin.y+=25;//origin为图左下角
	heroRect.size.width=80;
	heroRect.size.height=90;
	while(it!=EneVec.end())
	{
	if(heroRect.intersectsRect((*it)->getBoundingBox()))
	{*/
	hero->setIsAlive(false);
	hero->Destory();//hero被销毁了
	hero=NULL;
	GameOver();
	/*
	//GameOver();
	break;
	}
	++it;
	}
	}*/

}

void InGameLayer::GameOver()
{
	/*换场景时必须删除监听器*/

	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1.5f,welcomeLayer::scene()));
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}

void InGameLayer::cleanAllEnemys()
{
	for(int i=EneVec.size()-1;i>=0;--i)
	{
		auto Ene=EneVec.at(i);
		Ene->hurt(12);
		if(Ene->getIsBlowUp())
		{
			EneVec.erase(i);
		}
	}
	//正序删与倒序删1  2  3  4
}

void InGameLayer::MenuBombCallback()
{
	SimpleAudioEngine::getInstance()->playEffect("sound/use_bomb.mp3");
	cleanAllEnemys();
	if(bombNum>0)
		bombNum--;

	auto label = static_cast<Label*>(this->getChildByTag(GameSceneNodeTagNumLabel));
	auto menu = static_cast<Menu*>(this->getChildByTag(GameSceneNodeTagMenuBomb));

	if (bombNum == 0)
	{
		label->setVisible(false);
		menu->setVisible(false);
	}
	else
	{
		label->setString(__String::createWithFormat("x%d",bombNum)->getCString());
	}
	score += 100;
	scoreLabel->setString(__String::createWithFormat("%d",score)->getCString());
	UserDefault *defaults = UserDefault::getInstance();
	int highScore = defaults->getIntegerForKey("highscore_key");    
	if (highScore < score) {
		highScore = score;
		defaults->setIntegerForKey("highscore_key", highScore);
	}
	__String *text = __String::createWithFormat("%i", highScore);

	auto lblScoreOfHigh = static_cast<Label*>(this->getChildByTag(GameSceneNodelblHighScore));
	lblScoreOfHigh->setString(text->getCString());
}

void InGameLayer::createBombs(float delta)
{
	srand ((unsigned)time(nullptr));//利用时间改变随机种子
	int y = rand()%10;
	//int y = CCRANDOM_0_1();
	if(y>5)
	{
		auto bomb = Bomb::createWithSpriteFrameName("ufo2.png");
		bomb->ShootBomb();
		bomb->setTag(GameSceneNodeBatchTagBomb2);
		addChild(bomb,1000);
	}
	else
	{
		auto bomb = Bomb::createWithSpriteFrameName("ufo1.png");
		bomb->ShootBomb();
		bomb->setTag(GameSceneNodeBatchTagBomb1);
		addChild(bomb,1000);
	}
}

void InGameLayer::BombAttackPlane()
{
	bombNum++;

	auto label = static_cast<Label*>(this->getChildByTag(GameSceneNodeTagNumLabel));
	auto menu = static_cast<Menu*>(this->getChildByTag(GameSceneNodeTagMenuBomb));
	label->setVisible(true);
	menu->setVisible(true);
	label->setString(__String::createWithFormat("x%d",bombNum)->getCString());

}

void InGameLayer::updateDoubleBulletsIsOn(float delta)
{
	DoubleBulletsIsOn = false;
}

void InGameLayer::MenuPauseCallback()
{
	SimpleAudioEngine::getInstance()->playEffect("sound/button.mp3");
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	//暂停当前层中的node
	this->pause();

	for(const auto& node:this->getChildren())
	{
		node->pause();
	}
	auto layer = PauseLayer::create();

	this->addChild(layer);
	//this->addChild(layer,3000,GameSceneNodeBatchTagLayer);
}

/*
void InGameLayer::Resume(Ref*Sender)
{
Director::getInstance()->resume();
this->getChildByTag(GameSceneNodeBatchTagLayer)->removeFromParent();
}

void InGameLayer::Restart(Ref*Sender)
{
Director::getInstance()->replaceScene(/ *TransitionFade::create(2.0f,* /welcomeLayer::scene()/ *)* /);
}*/