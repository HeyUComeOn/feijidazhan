#ifndef FlyPlane_h__
#define FlyPlane_h__

#include "cocos2d.h"

class FlyPlane:public cocos2d::Sprite
{
public:
	FlyPlane(void);
	~FlyPlane(void);

	virtual bool init()override;
	CREATE_FUNC(FlyPlane);
	void Moveto(cocos2d::Vec2);
	void Destory();
	void FlyAction();
	//void SetViews();

};

#endif // FlyPlane_h__
