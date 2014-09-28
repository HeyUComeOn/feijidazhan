#ifndef PauseLayer_h__
#define PauseLayer_h__

#include "cocos2d.h"


class PauseLayer:public cocos2d::LayerColor
{
public:
	PauseLayer(void);
	~PauseLayer(void);
	bool init()override;
	CREATE_FUNC(PauseLayer);
	void setViews();
	void Resume(Ref*);
	void Restart(Ref*);
};

#endif // PauseLayer_h__
