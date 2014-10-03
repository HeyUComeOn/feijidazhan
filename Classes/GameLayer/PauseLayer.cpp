#include "GameLayer/PauseLayer.h"
#include "GameLayer/InGameLayer.h"
#include "SimpleAudioEngine.h"
#include "MyUtility.h"

using namespace CocosDenshion;
USING_NS_CC;

PauseLayer::PauseLayer(void)
{
}


PauseLayer::~PauseLayer(void)
{
}

bool PauseLayer::init()
{
	bool ret=false;
	do 
	{
		CC_BREAK_IF(!LayerColor::init());
		setViews();
		ret=true;	
	} while (0);
	return ret;
}

void PauseLayer::setViews()
{
	auto label1 = Label::createWithTTF(MyUtility::getUTF8Char("lblGoOn"), "fonts/hanyi.ttf", 80);
	//auto label1=Label::createWithTTF("Resume","fonts/COOPBL.TTF",36);
	auto label2 = Label::createWithTTF(MyUtility::getUTF8Char("lblRestart"), "fonts/hanyi.ttf", 80);
	//auto label2=Label::createWithTTF("Restart","fonts/COOPBL.TTF",36);
	auto item1=MenuItemLabel::create(label1,CC_CALLBACK_1(PauseLayer::Resume,this));
	auto item2=MenuItemLabel::create(label2,CC_CALLBACK_1(PauseLayer::Restart,this));
	auto menu=Menu::create(item1,item2,NULL);
	menu->alignItemsVerticallyWithPadding(100);
	this->addChild(menu);
}

void PauseLayer::Resume(Ref*Sender)
{
	this->getParent()->resume();
	for(const auto& node : this->getParent()->getChildren())
	{
		node->resume();
	}
	this->removeFromParent();
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void PauseLayer::Restart(Ref*Sender)
{
	Director::getInstance()->replaceScene(/*TransitionFade::create(2.0f,*/InGameLayer::scene()/*)*/);
}