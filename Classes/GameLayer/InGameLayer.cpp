#include "InGameLayer.h"
#include "GameObjects/Background.h"
#include "VisibleRect.h"
#include "GameLayer/welcomeLayer.h"
#include "Dlog.h"
USING_NS_CC;

InGameLayer::InGameLayer(void):EneVec(),bulletVec(),hero(NULL)
{
}


InGameLayer::~InGameLayer(void)
{
}

bool InGameLayer::init()
{
	bool ret=false;
	do 
	{
		if(Layer::init())
		{
			ret=true;
		}
	} while (0);
	return ret;
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
	
	auto bombLabel=Sprite::createWithSpriteFrameName("bomb.png");
	bombLabel->setPosition(VisibleRect::leftBottom()+Vec2(bombLabel->getContentSize().width/2,bombLabel->getContentSize().height/2));
	addChild(bombLabel);
	
	hero=FlyPlane::create();
	hero->setPosition(Vec2(VisibleRect::center().x,120));//hero的contentsize为0，为什么
	hero->setIsAlive(true);
	addChild(hero);
	m_Pos=hero->getPosition();
	addEvents();
	//
	this->schedule(SEL_SCHEDULE(&InGameLayer::createEnemys),CCRANDOM_0_1()+1.3f);
	this->schedule(SEL_SCHEDULE(&InGameLayer::moveAllEnemys),0.08f);
	this->schedule(SEL_SCHEDULE(&InGameLayer::createbullets),0.2f);
	this->schedule(SEL_SCHEDULE(&InGameLayer::moveAllbullets),0.016f);
	
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
	int x=CCRANDOM_0_1()*1.4+0.9;
	newEny->initEnemy(x);
	addChild(newEny);
	EneVec.pushBack(newEny);
	
}

void InGameLayer::createbullets(float t)
{
	if(hero&&hero->getIsAlive())
	{
		auto mbullet=bullet::create();
		mbullet->setPosition(hero->getPosition());
		addChild(mbullet);
		bulletVec.pushBack(mbullet);
	}

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
	if(hero&&hero->getIsAlive())
	{
		Dlog::showLog("touch began");
		return true;
	}
	return false;
}
void InGameLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	Dlog::showLog("touch move");
	m_Pos=touch->getLocation();
	
	if(hero&&hero->getIsAlive()&&hero->getBoundingBox().containsPoint(m_Pos))
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
				Ene->hurt(1);
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
	if(hero&&hero->getIsAlive())
	{
		Vector<EnemyBase*>::iterator it=EneVec.begin();
		auto heroRect=hero->getBoundingBox();
		heroRect.origin.x+=60;
		heroRect.origin.y+=25;//origin为图左下角
		heroRect.size.width=80;
		heroRect.size.height=90;
		while(it!=EneVec.end())
		{
			if(heroRect.intersectsRect((*it)->getBoundingBox()))
			{
				hero->Destory();//hero被销毁了
				hero->setIsAlive(false);
				hero=NULL;
				//GameOver();
				break;
			}
			++it;
		}
	}

}

void InGameLayer::GameOver()
{
	/*换场景时必须删除监听器*/

	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1.5f,welcomeLayer::scene()));
}

void InGameLayer::cleanAllEnemys()
{
	for(int i=EneVec.size()-1;i>=0;--i)
	{
		auto Ene=EneVec.at(i);
		Ene->hurt(5);
		if(Ene->getIsBlowUp())
		{
			EneVec.erase(i);
			break;
		}
	}
			//正序删与倒序删1  2  3  4
}