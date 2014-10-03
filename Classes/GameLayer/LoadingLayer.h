#ifndef LoadingLayer_h__
#define LoadingLayer_h__

#include "cocos2d.h"
USING_NS_CC;
class LoadingLayer:public Layer
{
public:
	LoadingLayer(void);
	~LoadingLayer(void);

public:
	void onEnter();
	void setViews();
	bool init()override;
	CREATE_FUNC(LoadingLayer);
	static Scene*scene();
	void logAction();
	void callback(Ref*pSender);
	void plybtnGm(Ref*pSender);
	void preload();
	void loadingTextureCallBack(Texture2D *texture);
	void delayCall(float dt); 
	void loadingAudio();
private:
	int m_nNumberOfLoaded;
	std::thread* _loadingAudioThread;
};

#endif // LoadingLayer_h__