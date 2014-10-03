#include "GameObjects/bullet.h"
#include "VisibleRect.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
bullet::bullet(void):vy(45.0f)
{
}


bullet::~bullet(void)
{
}

bool bullet::init()
{
	bool ret=false;
	do 
	{
		if(Sprite::init())
		{
			initWithSpriteFrameName("bullet1.png");
			auto body = PhysicsBody::createBox(this->getContentSize());

			body->setCategoryBitmask(0x01);			//0001
			body->setCollisionBitmask(0x02);		//0010
			body->setContactTestBitmask(0x01);

			this->setPhysicsBody(body);
			ret=true;
		}
	} while (0);
	return ret;
}


void bullet::move()
{
	float nestPosY=this->getPositionY()+vy;
	//SimpleAudioEngine::getInstance()->playEffect("shoot.mp3");
	this->setPositionY(nestPosY);
	if(nestPosY>=VisibleRect::top().y+this->getContentSize().height/2)
		this->removeFromParent();

}

/*
bool bullet::testhurt(EnemyBase*enemy)
{

	return  enemy->getBoundingBox().intersectsRect(this->getBoundingBox());
}*/