#include "InGameLayer.h"
#include "GameObjects/Background.h"
#include "VisibleRect.h"
#include "GameLayer/welcomeLayer.h"
USING_NS_CC;

InGameLayer::InGameLayer(void):EneVec(),bulletVec(),hero(nullptr)

{
}


InGameLayer::~InGameLayer(void)
{
}

bool InGameLayer::init()
{
	do 
	{
		bool ret=false;
		if(Layer::init())
		{
			
			ret=true;
		}
		return ret;
	} while (0);
}

void InGameLayer::onEnterTransitionDidFinish()
{
	setViews();
}
Scene*InGameLayer::scene()
{
	auto se=Scene::create();
	auto lay=InGameLayer::create();
	se->addChild(lay);
	return se;
}

void InGameLayer::setViews()
{
	auto back=Background::create();
	back->setAnchorPoint(Vec2(0,0));
	back->setPosition(Vec2(0,0));
	addChild(back);
	
	hero=FlyPlane::create();
	hero->setAnchorPoint(Vec2(0.5f,0.5f));
	hero->setPosition(VisibleRect::top());
	addChild(hero);
	m_Pos=hero->getPosition();
	addEvents();
	//
	this->schedule(SEL_SCHEDULE(&InGameLayer::createEnemys),CCRANDOM_0_1()+1.3f);
	this->schedule(SEL_SCHEDULE(&InGameLayer::moveAllEnemys),0.08f);
	this->schedule(SEL_SCHEDULE(&InGameLayer::createbullets),CCRANDOM_0_1()+0.3f);
	this->schedule(SEL_SCHEDULE(&InGameLayer::moveAllbullets),0.03f);
	
	this->scheduleUpdate();	
}

void InGameLayer::update(float t)
{
	bulletAttackEnemy();
	EnemyAttackPlane();
}
void InGameLayer::createEnemys(float t)
{
	auto newEny=EnemyBase::create();
	int x=CCRANDOM_0_1()+1.3;
	newEny->initEnemy(x);
	addChild(newEny);
	EneVec.pushBack(newEny);
	
}

void InGameLayer::createbullets(float t)
{
	auto mbullet=bullet::create();
	
	mbullet->setPosition(hero->getPosition());
	addChild(mbullet);
	bulletVec.pushBack(mbullet);
}
void InGameLayer::addEvents()
{
	auto listener=EventListenerTouchOneByOne::create();
	listener->onTouchBegan=CC_CALLBACK_2(InGameLayer::onTouchBegan,this);
	listener->onTouchMoved=CC_CALLBACK_2(InGameLayer::onTouchMoved,this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
}
bool InGameLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	log("touch began");

	return true;
}
void InGameLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	log("touch move");
	m_Pos=touch->getLocation();
	
	if(hero->getBoundingBox().containsPoint(m_Pos))
		hero->Moveto(m_Pos);
}

void InGameLayer::moveAllEnemys(float t)
{
	Vector<EnemyBase*>::iterator it=EneVec.begin();
	while(it!=EneVec.end())
	{
		(*it)->Move();
		if((*it)->getPositionY()<=-(*it)->getContentSize().height/2)
			it=EneVec.erase(it);
		else
			++it;
	}
}

void InGameLayer::moveAllbullets(float t)
{
	Vector<bullet*>::iterator it=bulletVec.begin();
	while(it!=bulletVec.end())
	{
		(*it)->move();
		if((*it)->getPositionY()>=VisibleRect::top().y+(*it)->getContentSize().height/2)
			it=bulletVec.erase(it);
		else
			++it;
	}
}

void InGameLayer::bulletAttackEnemy()
{
	
	Vector<bullet*>::iterator iter2=bulletVec.begin();
	for(int i=EneVec.size()-1;i>=0;--i)
	{
		auto Ene=EneVec.at(i);
		for(int j=bulletVec.size()-1;j>=0;--j)//如果就j!=0会出现越界
		{
			auto bult=bulletVec.at(j);
			if(bult->testhurt(Ene))
			{

				bult->removeFromParent();
				bulletVec.erase(j);
				Ene->hurt(10);
				if(Ene->getIsBlowUp())
				{
					EneVec.erase(i);
					break;
				}
			}
			//正序删与倒序删1  2  3  4
		}
	}

}

void InGameLayer::EnemyAttackPlane()
{
	Vector<EnemyBase*>::iterator it=EneVec.begin();
	while(it!=EneVec.end())
	{
		//Size size=(*it)->getContentSize();
		if(hero->getBoundingBox().containsPoint((*it)->getPosition()))
		{
			GameOver();
			break;
		}
		++it;
	}
}

void InGameLayer::GameOver()
{
	/*换场景时必须删除监听器*/
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1.5f,welcomeLayer::scene()));
	//log("GameOver");
}