#include "welcomeLayer.h"
#include "VisibleRect.h"
#include "GameLayer/InGameLayer.h"
welcomeLayer::welcomeLayer(void)
{
}


welcomeLayer::~welcomeLayer(void)
{
}

void welcomeLayer::setViews()
{
	auto bg=Sprite::create("bg_01.jpg");
	bg->setPosition(VisibleRect::center());
	bg->setAnchorPoint(Vec2(0.5,0.5));
	addChild(bg);
	
	//
	logAction();

	//
	auto plylb=Label::createWithTTF("Play game","COOPBL.TTF",68);
	auto item1=MenuItemLabel::create(plylb,CC_CALLBACK_1(welcomeLayer::plybtnGm,this));
	auto menu=Menu::create(item1,NULL);
	addChild(menu);
}

bool welcomeLayer::init()
{
	do 
	{
		bool ret=false;
		if(Layer::init())
		{
			setViews();
			ret= true;
		}
		return ret;
	} while (0);
	
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
	auto sp=Sprite::create("Log.jpg");
	sp->setAnchorPoint(Vec2(0.5,1));
	sp->setPosition(VisibleRect().top()-Vec2(0,100));
	sp->setOpacity(0);
	sp->setScale(0.8f);
	this->addChild(sp);

	auto ac=FadeIn::create(2.0f);
	sp->runAction(ac);
	
}

void welcomeLayer::plybtnGm(Ref*pSender)
{
	Director::getInstance()->replaceScene(TransitionJumpZoom::create(1.0f,InGameLayer::scene()));
}