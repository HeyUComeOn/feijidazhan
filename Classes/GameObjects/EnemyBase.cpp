#include "GameObjects/EnemyBase.h"
#include "VisibleRect.h"
#include "SimpleAudioEngine.h"
#include "Dlog.h"
USING_NS_CC;
using namespace CocosDenshion;
EnemyBase::EnemyBase(void):enemyType(-1),
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
	if(enemyType==2||enemyType==1)
	{
		int pos=this->getPositionY()-vy;
		this->setPositionY(pos);
	}
	else if(enemyType==0)
	{
		int pos=this->getPositionY()-vy+10;
		this->setPositionY(pos);
	}
}

void EnemyBase::initEnemy(int Enetype)
{
	std::string enemyName="";
	//0代表敌人3
	auto body = PhysicsBody::create();

	enemyType = Enetype % 3;

	if(enemyType==2||enemyType==1)
	{
		Dlog::showLog("%d", enemyType);
		enemyName = __String::createWithFormat("enemy%d.png" ,enemyType)->getCString();
	}
	else
	{
		enemyName = std::string("enemy3_n1.png");
	}
	initWithSpriteFrameName(enemyName);
	//this->autorelease();不能加
	if  (enemyType == 1) {
		Vec2 verts[] ={  
			Vec2(4.500,-17.500)*5/3,
			Vec2(-2.500,-17.500)*5/3, 
			Vec2(-23.500,3.500)*5/3,
			Vec2(-11.500,16.500)*5/3, 
			Vec2(13.500,17.500)*5/3, 
			Vec2(25.500,3.500)	*5/3};
		body->addShape(PhysicsShapePolygon::create(verts ,6));
	}
	else if(enemyType == 2)
	{
		Vec2 verts[] ={ 
			Vec2(-33.500,16.500)*5/3,
			Vec2(-3.500,45.500)*5/3,
			Vec2(3.500,45.500)*5/3,
			Vec2(33.500,15.500)*5/3, 
			Vec2(33.500,-14.500)*5/3, 
			Vec2(5.500,-40.500)*5/3, 
			Vec2(-5.500,-40.500)*5/3, 
			Vec2(-33.500,-15.500)*5/3	};
		body->addShape(PhysicsShapePolygon::create(verts ,8));
	}
	else
	{
		Vec2 verts[] ={ 
			Vec2(67.500,125.000)*5/3,
			Vec2(83.500,-72.000)*5/3, 
			Vec2(39.500,-114.000)*5/3, 
			Vec2(-34.500,-114.000)*5/3, 
			Vec2(-81.500,-71.000)*5/3, 
			Vec2(-66.500,125.000)*5/3};
		body->addShape(PhysicsShapePolygon::create(verts ,6));
		SimpleAudioEngine::getInstance()->playEffect("sound/big_spaceship_flying.mp3");
	}

	body->setCategoryBitmask(0x01);			//0001
	body->setCollisionBitmask(0x02);		//0010
	body->setContactTestBitmask(0x01);

	this->setPhysicsBody(body);
	if(enemyType==0)
		this->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("enemy3Move")));
	vy=CCRANDOM_0_1()*10+10.0f;
	setHP();

	setPosition(Vec2(CCRANDOM_0_1()*(VisibleRect::right().x-this->getContentSize().width-10)+(getContentSize().width+10)/2,VisibleRect::top().y));
	//BlowupAction();
}

void EnemyBase::setHP()
{
	if(enemyType == 0)
		hp=9;
	else if(enemyType == 2)
		hp=2;
	else if(enemyType == 1)
		hp=1;
}

void EnemyBase::BlowupAction()
{
	isBlowUp=true;
	//SimpleAudioEngine::getInstance()->playEffect("explosion.mp3");

	if(enemyType == 0)
	{
		auto animation3=AnimationCache::getInstance()->getAnimation("enemy3Down");
		auto animate3=Animate::create(animation3);
		auto remov3=RemoveSelf::create();
		auto seq3=Sequence::create(animate3,remov3,NULL);
		this->runAction(seq3);

	}
	else if(enemyType == 2)
	{
		auto animation2=AnimationCache::getInstance()->getAnimation("enemy2Down");
		auto animate2=Animate::create(animation2);
		auto remov2=RemoveSelf::create();
		auto seq2=Sequence::create(animate2,remov2,NULL);
		this->runAction(seq2);

	}
	else if(enemyType == 1)
	{
		auto animation1=AnimationCache::getInstance()->getAnimation("enemy1Down");
		auto animate1=Animate::create(animation1);
		auto remov1=RemoveSelf::create();
		auto seq1=Sequence::create(animate1,remov1,NULL);
		this->runAction(seq1);

	}
}

