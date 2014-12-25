#include "SplashScreen.h"
#include "GamePlay.h"

USING_NS_CC;

Scene* SplashScreen::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SplashScreen::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScreen::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto background = Sprite::create("splash_screen.png");
    background->setScaleX(visibleSize.width / background->getContentSize().width);
    background->setScaleY(visibleSize.height / background->getContentSize().height);
    background->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(background);
    
    this->scheduleOnce(schedule_selector(SplashScreen::GoToGamePlay), 2);
    
    return true;
}

void SplashScreen::GoToGamePlay(float dt)
{
    auto scene = GamePlay::createScene(0);
    Director::getInstance()->replaceScene(scene);
}





