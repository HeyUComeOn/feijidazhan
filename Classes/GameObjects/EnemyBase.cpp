#include "EnemyBase.h"
#include "VisibleRect.h"
#include "SimpleAudioEngine.h"
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
	do 
	{
		bool ret=false;
		if(Sprite::init())

			ret=true;
		return ret;
	} while (0);
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
	EnemyType=Enetype;
	char str[2];
	sprintf(str,"%d",Enetype);
	std::string enemyName=std::string("enemy_")+str+std::string(".png");
	initWithFile(enemyName);
	//this->autorelease();²»ÄÜ¼Ó
	vy=CCRANDOM_0_1()*5+3.0f;
	setHP();

	setPosition(Vec2(CCRANDOM_0_1()*(VisibleRect::right().x-this->getContentSize().width)+getContentSize().width/2,VisibleRect::top().y));
	//BlowupAction();
}

void EnemyBase::setHP()
{
	if(EnemyType==std::string("b"))
		hp=30;
	else if(EnemyType==std::string("m"))
		hp=5;
	else if(EnemyType==std::string("s"))
		hp=2;
}

void EnemyBase::BlowupAction()
{
	isBlowUp=true;
	SimpleAudioEngine::getInstance()->playEffect("explosion.mp3");
	Vector<SpriteFrame*> frmVec;
	auto spf1=SpriteFrame::create("explosion_01.png",Rect(0,0,26,26));
	auto spf2=SpriteFrame::create("explosion_02.png",Rect(0,0,38,39));
	auto spf3=SpriteFrame::create("explosion_03.png",Rect(0,0,40,42));
	frmVec.pushBack(spf1);
	frmVec.pushBack(spf2);
	frmVec.pushBack(spf3);
	auto animation=Animation::createWithSpriteFrames(frmVec,0.1f,1);
	auto animate=Animate::create(animation);
	auto remov=RemoveSelf::create();
	auto seq=Sequence::create(animate,remov,NULL);
	this->runAction(seq);
}

