#include "EnemyBase.h"
#include "VisibleRect.h"
#include "SimpleAudioEngine.h"
#include "Dlog.h"
USING_NS_CC;
using namespace CocosDenshion;
EnemyBase::EnemyBase(void):EnemyType(""),
										 vy(0.0f),
										 hp(0),
										 isBlowUp(false)
{
}


EnemyBase::~EnemyBase(void)
{
}

bool EnemyBase::init()
{
	bool ret=false;
	do 
	{
		if(Sprite::init())
			ret=true;
	} while (0);
	return ret;
}

void EnemyBase::Destory()
{
		this->removeFromParent();
}

void EnemyBase::hurt(int damg)
{
	hp-=damg;
	if(hp<=0)
	{
		BlowupAction();
	}
}

void EnemyBase::Move()
{
	int pos=this->getPositionY()-vy;
	this->setPositionY(pos);
}

void EnemyBase::initEnemy(int Enetype)
{
	std::string enemyName="";
	if(Enetype==2||Enetype==1)
	{
		char str[2];
		sprintf(str,"%d",Enetype);
		EnemyType=str;
		Dlog::showLog(EnemyType.c_str());
		enemyName=std::string("enemy")+str+std::string(".png");
	}
	else
	{
		char str2[2];
		sprintf(str2,"%d",Enetype+3);
		EnemyType=str2;
		Dlog::showLog(EnemyType.c_str());
		enemyName=std::string("enemy3_n1.png");
	}
	initWithSpriteFrameName(enemyName);
	//this->autorelease();²»ÄÜ¼Ó
	if(Enetype==0)
		this->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("enemy3Move")));
	vy=CCRANDOM_0_1()*5+5.0f;
	setHP();

	setPosition(Vec2(CCRANDOM_0_1()*(VisibleRect::right().x-this->getContentSize().width-10)+(getContentSize().width+10)/2,VisibleRect::top().y));
	//BlowupAction();
}

void EnemyBase::setHP()
{
	if(EnemyType==std::string("3"))
		hp=5;
	else if(EnemyType==std::string("2"))
		hp=2;
	else if(EnemyType==std::string("1"))
		hp=1;
}

void EnemyBase::BlowupAction()
{
	isBlowUp=true;
	//SimpleAudioEngine::getInstance()->playEffect("explosion.mp3");

	if(EnemyType==std::string("3"))
	{
		auto animation3=AnimationCache::getInstance()->getAnimation("enemy3Down");
		auto animate3=Animate::create(animation3);
		auto remov3=RemoveSelf::create();
		auto seq3=Sequence::create(animate3,remov3,NULL);
		this->runAction(seq3);
	}
	else if(EnemyType==std::string("2"))
	{
		auto animation2=AnimationCache::getInstance()->getAnimation("enemy2Down");
		auto animate2=Animate::create(animation2);
		auto remov2=RemoveSelf::create();
		auto seq2=Sequence::create(animate2,remov2,NULL);
		this->runAction(seq2);
	}
	else if(EnemyType==std::string("1"))
	{
		auto animation1=AnimationCache::getInstance()->getAnimation("enemy1Down");
		auto animate1=Animate::create(animation1);
		auto remov1=RemoveSelf::create();
		auto seq1=Sequence::create(animate1,remov1,NULL);
		this->runAction(seq1);
	}
}

