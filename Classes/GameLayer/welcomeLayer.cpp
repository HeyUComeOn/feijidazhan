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
	preload();
	auto bg=Sprite::createWithSpriteFrameName("background.png");
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
	Director::getInstance()->replaceScene(TransitionJumpZoom::create(1.0f,InGameLayer::scene()));
}

void welcomeLayer::preload()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot_background.plist");
	auto animation1=Animation::create();
	animation1->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero_blowup_n1.png"));
	animation1->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero_blowup_n2.png"));
	animation1->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero_blowup_n3.png"));
	animation1->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero_blowup_n4.png"));
	animation1->setLoops(1);
	animation1->setDelayPerUnit(0.1f);
	AnimationCache::getInstance()->addAnimation(animation1,"heroBlowup");

	auto animation2=Animation::create();
	animation2->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1_down1.png"));
	animation2->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1_down2.png"));
	animation2->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1_down3.png"));
	animation2->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1_down4.png"));
	animation2->setLoops(1);
	animation2->setDelayPerUnit(0.1f);
	AnimationCache::getInstance()->addAnimation(animation2,"enemy1Down");

	auto animation3=Animation::create();
	animation3->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_down1.png"));
	animation3->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_down3.png"));
	animation3->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_down3.png"));
	animation3->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_down4.png"));
	animation3->setLoops(1);
	animation3->setDelayPerUnit(0.2f);
	AnimationCache::getInstance()->addAnimation(animation3,"enemy2Down");

	auto animation4=Animation::create();
	animation4->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down1.png"));
	animation4->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down2.png"));
	animation4->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down3.png"));
	animation4->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down4.png"));
	animation4->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down5.png"));
	animation4->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down6.png"));
	animation4->setLoops(1);
	animation4->setDelayPerUnit(0.2f);
	AnimationCache::getInstance()->addAnimation(animation4,"enemy3Down");

	auto animation5=Animation::create();
	animation5->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n1.png"));
	animation5->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n2.png"));
	animation5->setDelayPerUnit(0.2f);
	animation5->setLoops(-1);
	AnimationCache::getInstance()->addAnimation(animation5,"enemy3Move");

}