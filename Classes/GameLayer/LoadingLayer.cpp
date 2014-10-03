#include "GameLayer/welcomeLayer.h"
#include "VisibleRect.h"
#include "GameLayer/LoadingLayer.h"
#include "SimpleAudioEngine.h"
#include "Dlog.h"
using namespace CocosDenshion;

LoadingLayer::LoadingLayer(void)
{
}


LoadingLayer::~LoadingLayer(void)
{
}

void LoadingLayer::setViews()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/shoot_background.plist");
	auto bg = Sprite::createWithSpriteFrameName("background.png");
	bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
	this->addChild(bg);

	auto logo =  Sprite::createWithSpriteFrameName("mylog.png");
	this->addChild(logo);
	logo->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
	auto ac1 = JumpBy::create(2.0f,Vec2(0,0),50,1);
	auto ac2 = ScaleBy::create(1.0f,1.0f,0.625f);
	auto ac3 = ScaleBy::create(1.0f,1.0f,1.6f);
	
	logo->runAction(RepeatForever::create(Spawn::create(ac1,Sequence::create(ac2,ac3,nullptr),nullptr)));
	
	m_nNumberOfLoaded = 0;

	Director::getInstance()->getTextureCache()->addImageAsync("texture/shoot.png",
		CC_CALLBACK_1(LoadingLayer::loadingTextureCallBack, this));

	_loadingAudioThread = new std::thread(&LoadingLayer::loadingAudio,this);
	/*preload();
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
	addChild(menu);*/
}

void LoadingLayer::loadingTextureCallBack(Texture2D *texture)
{
	switch (m_nNumberOfLoaded++)
	{
	case 0:
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/shoot.plist",texture);
		Dlog::showLog("home textrue ok.");
		preload();
		/*break;
	case 1:
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/setting_textrue.plist",texture);
		Dlog::showLog("setting textrue ok.");
		break;
	case 2:
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/gameplay_textrue.plist",texture);
		Dlog::showLog("gamepla textrue ok.");*/
		this->schedule(schedule_selector(LoadingLayer::delayCall),1,1,3);
		//float interval, unsigned int repeat, float delay
		break;
	}
}

void LoadingLayer::delayCall(float dt)  
{
	auto sc = welcomeLayer::scene();
	Director::getInstance()->replaceScene(sc);
}

void LoadingLayer::loadingAudio()
{
	log("loadAudio");

#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID) //android平台不进行预处理

	//初始化 音乐
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/game_music.mp3");
	
	//初始化音效  TODO  预处理后没有声音，移植的时候需要测试。
	SimpleAudioEngine::getInstance()->preloadEffect("sound/achievement.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/big_spaceship_flying.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/bullet.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/button.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/enemy1_down.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/enemy2_down.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/enemy3_down.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/game_over.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/get_bomb.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/get_double_laser.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/use_bomb.mp3");

#endif

}

bool LoadingLayer::init()
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

void LoadingLayer::onEnter()
{
	Layer::onEnter();

}

Scene*LoadingLayer::scene()
{
	auto s = Scene::create();
	auto l	 = LoadingLayer::create();
	s->addChild(l);
	return s;
}

/*
void LoadingLayer::logAction()
{
	auto sp2=Sprite::createWithSpriteFrameName("shoot_copyright_02.png");
	sp2->setAnchorPoint(Vec2(0.5,1));
	sp2->setPosition(VisibleRect().top()-Vec2(0,300));
	sp2->setOpacity(0);
	sp2->setScale(0.8f);
	this->addChild(sp2);
	auto ac=FadeIn::create(2.0f);
	sp2->runAction(Sequence::create(ac,
		CallFuncN::create( CC_CALLBACK_1(LoadingLayer::callback, this)),
		NULL));

}*/

/*
void LoadingLayer::callback(Ref*pSender)
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
void LoadingLayer::plybtnGm(Ref*pSender)
{
	Director::getInstance()->replaceScene(/ *TransitionJumpZoom::create(1.0f,* /welcomeLayer::scene()/ *)* /);
}*/

void LoadingLayer::preload()
{
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