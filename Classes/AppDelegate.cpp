#include "AppDelegate.h"
#include "GameLayer/LoadingLayer.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::createWithRect("War of Planes",Rect(0,0,800,1280));//注意不要让屏幕在windows中无法显示，会出现奇葩错误，可以利用ZoomFactor调节
		//glview->setFrameZoomFactor(0.65f);
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);


	//屏幕大小
	auto screenSize = glview->getFrameSize();

	//设计分辨率大小
	auto designSize = Size(400, 640);
	//资源大小
	auto resourceSize = Size(480, 800);

	std::vector<std::string> searchPaths;

	if (screenSize.height > 960) {	//640x1136
		designSize = Size(800, 1280);
		searchPaths.push_back("texture");
	} else {
		searchPaths.push_back("texture");
	}

	director->setContentScaleFactor(resourceSize.width/designSize.width); 

	FileUtils::getInstance()->setSearchPaths(searchPaths);

	glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::FIXED_WIDTH);

    // create a scene. it's an autorelease object
    auto scene = LoadingLayer::scene();
	
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
