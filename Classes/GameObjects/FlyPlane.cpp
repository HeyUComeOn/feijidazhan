#include "GameObjects/FlyPlane.h"
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
			Vec2 verts[] ={ 
				Vec2(35.000,-8.000),
				Vec2(50.000,-48.000),
				Vec2(32.000,-59.000),
				Vec2(-36.000,-59.000), 
				Vec2(-52.000,-49.000), 
				Vec2(-37.000,-10.000), 
				Vec2(0.000,39.000)
			};

			auto body = PhysicsBody::createPolygon(verts, 7);

			body->setCategoryBitmask(0x01);			//0001
			body->setCollisionBitmask(0x02);		//0010
			body->setContactTestBitmask(0x01);

			this->setPhysicsBody(body);
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
