#include "GameOver.h"
#include "GamePlay.h"

USING_NS_CC;

Scene* GameOver::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameOver::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameOver::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    UserDefault *def = UserDefault::getInstance();
    auto score = def->getIntegerForKey("score");
    auto highScore = def->getIntegerForKey("highScore");
    auto newHighScore = def->getIntegerForKey("newHighScore");
    def->flush();
    
    __String *gameOverText = __String::create("Game Over");
    __String *tempScore = __String::createWithFormat("%i", score);
    __String *tempHighScore = __String::createWithFormat("%i", highScore);
    __String *tempScoreText = __String::create("Score");
    __String *tempHighScoreText = __String::create("Best");
    
    auto background = Sprite::create("background.png");
    background->setScaleX(visibleSize.width / background->getContentSize().width);
    background->setScaleY(visibleSize.height / background->getContentSize().height);
    background->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(background);
    
    // Add game over popover
    auto popover = Node::create();
    popover->setPosition(Point(0,0));
    
    auto theGameOver = Label::createWithTTF( gameOverText->getCString(), "ShowG.ttf", visibleSize.height * 0.2);
    theGameOver->setColor(Color3B::RED);
    theGameOver->setPosition(Point(visibleSize.width / 2, visibleSize.height * 0.8));
    
    auto scoreText = Label::createWithTTF( tempScoreText->getCString(), "Arial_Regular.ttf", visibleSize.height * 0.05);
    scoreText->setColor(Color3B::BLACK);
    scoreText->setPosition(Point(visibleSize.width / 2, visibleSize.height * 0.675));
    
    auto theScore = Label::createWithTTF( tempScore->getCString(), "Arial_Regular.ttf", visibleSize.height * 0.1);
    theScore->setColor(Color3B::BLACK);
    theScore->setPosition(Point(visibleSize.width / 2, visibleSize.height * 0.575));
    
    auto highScoreText = Label::createWithTTF( tempHighScoreText->getCString(), "Arial_Regular.ttf", visibleSize.height * 0.05);
    highScoreText->setColor(Color3B::BLACK);
    highScoreText->setPosition(Point(visibleSize.width / 2, visibleSize.height * 0.475));
    
    auto theHighScore = Label::createWithTTF( tempHighScore->getCString(), "Arial_Regular.ttf", visibleSize.height * 0.1);
    theHighScore->setColor(Color3B::BLACK);
    theHighScore->setPosition(Point(visibleSize.width / 2, visibleSize.height * 0.375));
    
    popover->addChild(scoreText,2);
    popover->addChild(theScore,2);
    popover->addChild(highScoreText,2);
    popover->addChild(theHighScore,2);
    popover->addChild(theGameOver);
    
    if(newHighScore)
    {
        auto newBest = Sprite::create("new.png");
        newBest->setPosition(Vec2((visibleSize.width / 2) + (theHighScore->getContentSize().width / 2) + (newBest->getContentSize().width / 2) + 15, visibleSize.height * 0.375));
        popover->addChild(newBest,2);
    }
    
    // Add icon menu to bottom
    auto home = MenuItemImage::create("home.png", "home.png", CC_CALLBACK_1(GameOver::GoToGamePlay, this));
    auto w = home->getContentSize().width;
    home->setPosition(Vec2(visibleSize.width / 2 - (w * 2), visibleSize.height * 0.215));
    
    auto facebook = MenuItemImage::create("facebook.png", "facebook.png", CC_CALLBACK_1(GameOver::Share, this));
    facebook->setPosition(Vec2(visibleSize.width / 2 - (w * 0.65), visibleSize.height * 0.215));
    
    auto rate = MenuItemImage::create("rate.png", "rate.png", CC_CALLBACK_1(GameOver::Rate, this));
    rate->setPosition(Vec2(visibleSize.width / 2 + (w * 0.65), visibleSize.height * 0.215));
    
    auto replay = MenuItemImage::create("replay.png", "replay.png", CC_CALLBACK_1(GameOver::Replay, this));
    replay->setPosition(Vec2(visibleSize.width / 2 + (w * 2), visibleSize.height * 0.215));
    
    auto menu2 = Menu::create(home,facebook,rate,replay, NULL);
    menu2->setPosition(Point::ZERO);
    
    popover->addChild(menu2);
    
    this->addChild(popover,10000);
    
    return true;
}

void GameOver::GoToGamePlay( cocos2d::Ref *sender )
{
    auto scene = GamePlay::createScene(0);
    Director::getInstance()->replaceScene(scene);
}

void GameOver::Replay( cocos2d::Ref *sender )
{
    auto scene = GamePlay::createScene(1);
    Director::getInstance()->replaceScene(scene);
}

void GameOver::Share( cocos2d::Ref *sender )
{

}

void GameOver::Rate( cocos2d::Ref *sender )
{

}





