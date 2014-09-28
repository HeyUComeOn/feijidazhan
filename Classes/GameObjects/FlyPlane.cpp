#include "FlyPlane.h"
#include "VisibleRect.h"
USING_NS_CC;

FlyPlane::FlyPlane(void):isAlive(false)
{
}


FlyPlane::~FlyPlane(void)
{
}

bool FlyPlane::init()
{
	bool ret=false;
	do 
	{
		if(Sprite::init())
		{	
			initWithSpriteFrameName("hero1.png");
			this->setAnchorPoint(Vec2(0.5,0.5));
			FlyAction();
			ret=true;
		}
	} while (0);
	return ret;
}

void FlyPlane::Moveto(cocos2d::Vec2 pos)
{
	if(isAlive&&!Director::getInstance()->isPaused())
	{
		auto FlySize=this->getContentSize();
		if(pos.x<FlySize.width/2-52)
			pos.x=FlySize.width/2-52;
		if(pos.x>VisibleRect::right().x-FlySize.width/2+55)
			pos.x=VisibleRect::right().x-FlySize.width/2+55;
		if(pos.y<FlySize.height/2)
			pos.y=FlySize.height/2;
		if(pos.y>VisibleRect::top().y-FlySize.height/2+50)
			pos.y=VisibleRect::top().y-FlySize.height/2+50;
		this->setPosition(pos);
	}
}

void FlyPlane::Destory()
{
	auto animation=AnimationCache::getInstance()->getAnimation("heroBlowup");
	auto remo=RemoveSelf::create();
	auto animate=Animate::create(animation);
	this->runAction(Sequence::create(animate,remo,NULL));
	//this->removeFromParent();
}
void FlyPlane::FlyAction()
{
	auto animation=Animation::create();
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1.png"));
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2.png"));
	animation->setLoops(-1);
	animation->setDelayPerUnit(0.2f);
	auto animate=Animate::create(animation);
	this->runAction(animate);
}
