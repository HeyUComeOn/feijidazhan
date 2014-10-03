#include "GameObjects/DoubleBullets.h"
#include "VisibleRect.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
DoubleBullets::DoubleBullets(void):vy(45.0f)
{
}


DoubleBullets::~DoubleBullets(void)
{
}

bool DoubleBullets::init()
{
	bool ret=false;
	do 
	{
		if(Sprite::init())
		{
			initWithSpriteFrameName("bullet2.png");
			auto body = PhysicsBody::createBox(this->getContentSize()*5/3);

			body->setCategoryBitmask(0x01);			//0001
			body->setCollisionBitmask(0x02);		//0010
			body->setContactTestBitmask(0x01);

			this->setPhysicsBody(body);
			ret=true;
		}
	} while (0);
	return ret;
}

void DoubleBullets::move()
{
	float nestPosY=this->getPositionY()+vy;
	//SimpleAudioEngine::getInstance()->playEffect("shoot.mp3");
	this->setPositionY(nestPosY);
	if(nestPosY>=VisibleRect::top().y+this->getContentSize().height/2)
		this->removeFromParent();

}

/*
bool DoubleBullets::testhurt(EnemyBase*enemy)
{

	return  enemy->getBoundingBox().intersectsRect(this->getBoundingBox());
}*/