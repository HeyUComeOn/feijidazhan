#include "Background.h"
USING_NS_CC;

Background::Background(void):m_Background_1(nullptr),m_Background_2(nullptr),vy(6.0f)
{
}


Background::~Background(void)
{
}

bool Background::init()
{
	bool ret=false;
	do 
	{

		if(Layer::init())
		{
			setViews();
			this->schedule(SEL_SCHEDULE(&Background::move),0.020f);
			ret=true;
		}

	} while (0);
	return ret;
}

void Background::setViews()
{
	m_Background_1=Sprite::createWithSpriteFrameName("background.png");
	m_Background_1->getTexture()->setAliasTexParameters();//·ÀºÚÏß
	m_Background_1->setAnchorPoint(Vec2(0,0));
	m_Background_1->setPosition(Vec2(0,0));
	
	addChild(m_Background_1);

	m_Background_2=Sprite::createWithSpriteFrameName("background.png");
	m_Background_2->getTexture()->setAliasTexParameters();
	m_Background_2->setAnchorPoint(Vec2(0,0));
	m_Background_2->setPosition(Vec2(0,m_Background_2->getContentSize().height));
	addChild(m_Background_2);
};

void Background::move(float t)
{
	m_Background_1->setPositionY(m_Background_1->getPositionY()-vy);
	m_Background_2->setPositionY(m_Background_1->getPositionY()+m_Background_1->getContentSize().height-2);
	if(m_Background_2->getPositionY()<=0)
	{
		m_Background_1->setPositionY(0);
	}
}