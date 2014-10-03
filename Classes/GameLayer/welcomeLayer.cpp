#include "GameLayer/welcomeLayer.h"
#include "VisibleRect.h"
#include "GameLayer/InGameLayer.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

welcomeLayer::welcomeLayer(void)
{
}


welcomeLayer::~welcomeLayer(void)
{
}

void welcomeLayer::setViews()
{
	
	auto bg=Sprite::createWithSpriteFrameName("background.png");
	bg->setPosition(VisibleRect::center());
	bg->setAnchorPoint(Vec2(0.5,0.5));
	addChild(bg);
	
	//
	logAction();

	//
	auto plylb=Label::createWithTTF("Play game","fonts/COOPBL.TTF",68);
	auto item1=MenuItemLabel::create(plylb,CC_CALLBACK_1(welcomeLayer::plybtnGm,this));
	auto menu=Menu::create(item1,NULL);
	addChild(menu);
}

bool welcomeLayer::init()
{
	bool ret=false;
	do 
	{
		if(Layer::init())
		{
			setViews();
			ret= true;
		}

	} while (0);
	return ret;
}

void welcomeLayer::onEnter()
{
	Layer::onEnter();
	
}

Scene*welcomeLayer::scene()
{
	auto s=Scene::create();
	auto l=welcomeLayer::create();
	s->addChild(l);
	return s;
}

void welcomeLayer::logAction()
{
	auto sp2=Sprite::createWithSpriteFrameName("shoot_copyright_02.png");
	sp2->setAnchorPoint(Vec2(0.5,1));
	sp2->setPosition(VisibleRect().top()-Vec2(0,300));
	sp2->setOpacity(0);
	sp2->setScale(0.8f);
	this->addChild(sp2);
	auto ac=FadeIn::create(2.0f);
	sp2->runAction(Sequence::create(ac,
		CallFuncN::create( CC_CALLBACK_1(welcomeLayer::callback, this)),
		NULL));
	
}

void welcomeLayer::callback(Ref*pSender)
{
	auto sp=Sprite::createWithSpriteFrameName("mylog.png");
	sp->setAnchorPoint(Vec2(1,1));
	sp->setPosition(VisibleRect().leftTop());
	sp->setOpacity(150);
	sp->setScale(0.6f);
	this->addChild(sp);
	auto ac1=JumpTo::create(1.0f,VisibleRect::rightTop(),50,3);
	sp->runAction(ac1);
}
void welcomeLayer::plybtnGm(Ref*pSender)
{
	Director::getInstance()->replaceScene(/*TransitionJumpZoom::create(1.0f,*/InGameLayer::scene()/*)*/);
}

