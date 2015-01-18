#include "AppDelegate.h"
#include "GamePlay.h"
#include "SplashScreen.h"
#include "CCServiceManager.h"
#include "CCStoreService.h"
#include "Cocos2dxStore.h"
#include "cherryAssets.h"
#include "ExampleEventHandler.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
    handler = new ExampleEventHandler();
}

AppDelegate::~AppDelegate()
{
    soomla::CCStoreEventDispatcher::getInstance()->removeEventHandler(handler);
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    //Initialize SoomLa
    cherryAssets *assets = cherryAssets::create();
    __Dictionary *commonParams = __Dictionary::create();
    commonParams->setObject(__String::create("ExampleCustomSecret"), "customSecret");
    soomla::CCServiceManager::getInstance()->setCommonParams(commonParams);
    __Dictionary *storeParams = __Dictionary::create();
    storeParams->setObject(__String::create("ExamplePublicKey"), "androidPublicKey");
    
    soomla::CCStoreService::initShared(assets, storeParams);
    
    soomla::CCStoreEventDispatcher::getInstance()->addEventHandler(handler);
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Bird Clash");
        director->setOpenGLView(glview);
    }
    
    // turn on display FPS
    director->setDisplayStats(false);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    
    auto fileUtils = FileUtils::getInstance();
    auto screenSize = glview->getFrameSize();
    std::vector<std::string> resDirOrders;
    
    resDirOrders.push_back("iphone");
    glview->setDesignResolutionSize(screenSize.width, screenSize.height, ResolutionPolicy::NO_BORDER);
    director->setContentScaleFactor(500 / screenSize.height);
    
    fileUtils->setSearchPaths(resDirOrders);
    
    // create a scene. it's an autorelease object
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    auto scene = SplashScreen::createScene();
#else
    auto scene = GamePlay::createScene(0);
#endif
    
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
