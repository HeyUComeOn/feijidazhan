#ifndef welcomeLayer_h__
#define welcomeLayer_h__

#include "cocos2d.h"
USING_NS_CC;
class welcomeLayer:public Layer
{
public:
	welcomeLayer(void);
	~welcomeLayer(void);

public:
	void setViews();
	bool init()override;
	CREATE_FUNC(welcomeLayer);
	static Scene*scene();
	void logAction();
	void callback(Ref*pSender);
	void plybtnGm(Ref*pSender);
	void preload();
};

#endif // welcomeLayer_h__