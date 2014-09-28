#include "bullet.h"
#include "VisibleRect.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
bullet::bullet(void):vy(10.0f)
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

bool bullet::testhurt(EnemyBase*enemy)
{

	return  enemy->getBoundingBox().intersectsRect(this->getBoundingBox());
}