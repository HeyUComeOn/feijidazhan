#include "GameObjects/Bomb.h"

USING_NS_CC;

Bomb* Bomb::createWithSpriteFrameName(const char* spriteFrameName)
{
	Bomb *bomb = new Bomb();
	if (bomb && bomb->initWithSpriteFrameName(spriteFrameName)) {
		bomb->autorelease();
		bomb->setVisible(false);

		auto body = PhysicsBody::createBox(bomb->getContentSize());

		body->setCategoryBitmask(0x01);			//0001
		body->setCollisionBitmask(0x02);		//0010
		body->setContactTestBitmask(0x01);

		bomb->setPhysicsBody(body);

		return bomb;
	}
	CC_SAFE_DELETE(bomb);
	return nullptr;
}

void Bomb::ShootBomb()
{

	auto visibleSize = Director::getInstance()->getVisibleSize();
	
	srand ((unsigned)time(nullptr));//利用时间改变随机种子
	int n = rand()%10;

	if (n>5)
	{
		this->setPosition(Vec2(visibleSize.width/4,visibleSize.height+this->getContentSize().height/2));
	}
	else
	{
		this->setPosition(Vec2(3*visibleSize.width/4,visibleSize.height+this->getContentSize().height/2));
	}
	this->setVisible(true);
	
	this->unscheduleUpdate();
	this->scheduleOnce(schedule_selector(Bomb::BombUpdate),3);

}

void Bomb::BombUpdate(float dt)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto starPos = this->getPosition();
	auto ac1 = MoveTo::create(1 ,starPos+Vec2(0,-visibleSize.height/5));
	auto ac2 = MoveTo::create(1,starPos);
	auto ac3 = MoveTo::create(1 ,Vec2(starPos.x,-this->getContentSize().height/2));
	//auto ac4 = RemoveSelf::create();
	
	this->runAction(Sequence::create(EaseBackOut::create(ac1),EaseBackIn::create(ac2),ac3,nullptr));

	if (this->getPosition().y - this->getContentSize().height/2< 0) {
		//log("isVisible = %d",this->isVisible());
		this->setVisible(false);
		//this->unscheduleUpdate();
		this->removeFromParent();
	}    
}
