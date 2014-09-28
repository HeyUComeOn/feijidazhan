#include "PauseLayer.h"
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
		CC_BREAK_IF(LayerColor::init());
		setViews();
		ret=true;	
	} while (0);
	return ret;
}

void PauseLayer::setViews()
{
	auto label1=Label::createWithTTF("Resume","COOPBL.TTF");
	auto label2=Label::createWithTTF("Restart","COOPBL.TTF");
	auto item1=MenuItemLabel::create(label1,CC_CALLBACK_1(PauseLayer::Resume,this));
	auto item2=MenuItemLabel::create(label2,CC_CALLBACK_1(PauseLayer::Restart,this));
	auto menu=Menu::create(item1,item2,NULL);
	menu->alignItemsVerticallyWithPadding(10);
	this->addChild(menu);
}

void PauseLayer::Resume(Ref*Sender)
{

}

void PauseLayer::Restart(Ref*Sender)
{

}