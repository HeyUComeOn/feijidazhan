#include "bullet.h"
#include "VisibleRect.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
bullet::bullet(void):vy(35.0f)
{
}


bullet::~bullet(void)
{
}

bool bullet::init()
{
	do 
	{
		bool ret=false;
		if(Sprite::init())
		{
			initWithFile("bullet_1.png");
			ret=true;
		}
		return ret;
	} while (0);
}


void bullet::move()
{
	float nestPosY=this->getPositionY()+vy;
	SimpleAudioEngine::getInstance()->playEffect("shoot.mp3");
	this->setPositionY(nestPosY);
	if(nestPosY>=VisibleRect::top().y+this->getContentSize().height/2)
		this->removeFromParent();

}

bool bullet::testhurt(EnemyBase*enemy)
{
	return  enemy->getBoundingBox().containsPoint(this->getPosition());
}