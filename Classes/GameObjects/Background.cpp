#include "Background.h"
USING_NS_CC;

Background::Background(void):m_Background_1(nullptr),m_Background_2(nullptr),vy(5.5f)
{
}


Background::~Background(void)
{
}

bool Background::init()
{
	do 
	{
		bool ret=false;
		if(Layer::init())
		{
			setViews();
			this->schedule(SEL_SCHEDULE(&Background::move),0.020f);
			ret=true;
		}
		return ret;
	} while (0);
}

void Background::setViews()
{
	m_Background_1=Sprite::create("bg_01.jpg");
	m_Background_1->setAnchorPoint(Vec2(0,0));
	m_Background_1->setPosition(Vec2(0,0));
	addChild(m_Background_1);

	m_Background_2=Sprite::create("bg_02.jpg");
	m_Background_2->setAnchorPoint(Vec2(0,0));
	m_Background_2->setPosition(Vec2(0,m_Background_2->getPositionY()+m_Background_2->getContentSize().height));
	addChild(m_Background_2);
};

void Background::move(float t)
{
	m_Background_1->setPositionY(m_Background_1->getPositionY()-vy);
	m_Background_2->setPositionY(m_Background_2->getPositionY()-vy);
	if(m_Background_2->getPositionY()<=-m_Background_2->getContentSize().height)
	{
		m_Background_2->setPositionY(m_Background_1->getPositionY()+m_Background_1->getContentSize().height);
	}
	else if(m_Background_1->getPositionY()<=-m_Background_1->getContentSize().height)
	{
		m_Background_1->setPositionY(m_Background_2->getPositionY()+m_Background_2->getContentSize().height);
	}
}