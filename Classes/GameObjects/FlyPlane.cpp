#include "FlyPlane.h"
#include "VisibleRect.h"
USING_NS_CC;

FlyPlane::FlyPlane(void)
{
}


FlyPlane::~FlyPlane(void)
{
}

bool FlyPlane::init()
{
	do 
	{
		bool ret=false;
		if(Sprite::init())
		{	
			//this->setAnchorPoint(Vec2(0.5f,0.5f));
			FlyAction();
			ret=true;
		}
		return ret;
	} while (0);
}

void FlyPlane::Moveto(cocos2d::Vec2 pos)
{
	
	if(pos.x<this->getContentSize().width/2)
		pos.x=this->getContentSize().width/2;
	else if(pos.x>VisibleRect::right().x-this->getContentSize().width/2)
		pos.x=VisibleRect::right().x-this->getContentSize().width/2;
	else if(pos.y<this->getContentSize().height/2)
		pos.y=this->getContentSize().height/2;
	else if(pos.y>VisibleRect::top().y-this->getContentSize().height/2)
		pos.y=VisibleRect::top().y-this->getContentSize().height/2;
	this->setPosition(pos);
}

void FlyPlane::Destory()
{
	this->removeFromParent();
}
void FlyPlane::FlyAction()
{
	Vector<SpriteFrame*> spfrmVec;
	
	auto spfrm1=SpriteFrame::create("hero_1.png",Rect(0,0,62,75));
	auto spfrm2=SpriteFrame::create("hero_2.png",Rect(0,0,62,75));
	spfrmVec.pushBack(spfrm1);
	spfrmVec.pushBack(spfrm2);
	auto animation=Animation::createWithSpriteFrames(spfrmVec,0.2f,-1);//
	auto animate=Animate::create(animation);
	this->runAction(animate);
}
//void FlyPlane::SetViews()
//{
//
//}