#ifndef Background_h__
#define Background_h__

#include "cocos2d.h"

class Background:public cocos2d::Layer
{
public:
	Background(void);
	~Background(void);

	bool init()override;
	CREATE_FUNC(Background);
private:
	cocos2d::Sprite*m_Background_1;
	cocos2d::Sprite*m_Background_2;
	float vy;
public:
	void setViews();
	void move(float);
};

#endif // Background_h__