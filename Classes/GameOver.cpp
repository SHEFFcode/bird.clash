#include "GameOver.h"
#include "GamePlay.h"
#include "FacebookAgent.h"


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
    
    //Initialize the facebook SDK
    cocos2d::plugin::FacebookAgent::getInstance();
    
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
    
    auto facebookitem = MenuItemImage::create("facebook.png", "facebook.png", CC_CALLBACK_1(GameOver::Share, this));
    facebookitem->setPosition(Vec2(visibleSize.width / 2 - (w * 0.65), visibleSize.height * 0.215));
    
    auto rate = MenuItemImage::create("rate.png", "rate.png", CC_CALLBACK_1(GameOver::Rate, this));
    rate->setPosition(Vec2(visibleSize.width / 2 + (w * 0.65), visibleSize.height * 0.215));
    
    auto replay = MenuItemImage::create("replay.png", "replay.png", CC_CALLBACK_1(GameOver::Replay, this));
    replay->setPosition(Vec2(visibleSize.width / 2 + (w * 2), visibleSize.height * 0.215));
    
    auto menu2 = Menu::create(home,facebookitem,rate,replay, NULL);
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

////Screenshot
//std::string GameOver::sceenshot(std::string& filename)
//{
//    Size visibleSize = Director::getInstance()->getVisibleSize();
//    Point origin = Director::getInstance()->getVisibleOrigin();
//    auto tex = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888);
//    tex->setPosition((origin.x + visibleSize.width) / 2, (origin.y + visibleSize.height) / 2);
//    tex->begin();
//    Director::getInstance()->getRunningScene()->visit();
//    tex->end();
//    
//    std::string imgPath = FileUtils::getInstance()->getWritablePath();
//    if (imgPath.length() == 0) {
//        return "";
//    }
//    
//    bool ret = tex->saveToFile(filename, Image::Format::PNG);
//    if (ret) {
//        imgPath += filename;
//        CCLOG("save image to %s", imgPath.c_str());
//        return imgPath;
//    }
//    return "";
//}


void GameOver::Share( cocos2d::Ref *sender )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//
//    std::string fileName = "CaptureScreenTest.png";
//    
//    std::string imgPath = sceenshot(fileName);
//    
//    auto delay = DelayTime::create(1.0);
//    auto share = CallFunc::create([=](){
//        cocos2d::plugin::FacebookAgent::FBInfo params;
//        params.insert(std::make_pair("dialog", "sharePhoto"));
//        params.insert(std::make_pair("photo", imgPath));
//        
//        if (cocos2d::plugin::FacebookAgent::getInstance()->canPresentDialogWithParams(params))
//        {
//            cocos2d::plugin::FacebookAgent::getInstance()->dialog(params, [=](int ret, std::string& msg){
//                CCLOG("%s", msg.c_str());
//            });
//        }
//        else
//        {
//            cocos2d::plugin::FacebookAgent::getInstance()->dialog(params, [=](int ret, std::string& msg){
//                CCLOG("%s", msg.c_str());
//            });
//            
//        }
//    });
//    
//    auto seq = Sequence::create(delay, share, nullptr);
//    runAction(seq);
//    
//    cocos2d::plugin::FacebookAgent::FBInfo params;
//    params.insert(std::make_pair("dialog", "Link"));
//    params.insert(std::make_pair("name", "Cocos2d-x web site"));
//    params.insert(std::make_pair("caption", "Cocos2d-x caption"));
//    params.insert(std::make_pair("description", "Cocos2d-x description"));
//    params.insert(std::make_pair("to", "100006738453912")); // android only web view support
//    params.insert(std::make_pair("picture", "http://files.cocos2d-x.org/images/orgsite/logo.png"));
//    params.insert(std::make_pair("link", "http://www.cocos2d-x.org"));
//    
//    if (cocos2d::plugin::FacebookAgent::getInstance()->canPresentDialogWithParams(params))
//    {
//        cocos2d::plugin::FacebookAgent::getInstance()->dialog(params, [=](int ret ,std::string& msg){
//            CCLOG("%s", msg.c_str());
//        });
//    }
//    else
//    {
//        cocos2d::plugin::FacebookAgent::getInstance()->share(params, [=](int ret ,std::string& msg){
//            CCLOG("%s", msg.c_str());
//        });
//    }
    
    
    
    //************ WORKS ****************//
    
//    cocos2d::plugin::FacebookAgent::FBInfo params;
//    params.insert(std::make_pair("dialog", "share_link"));
//    params.insert(std::make_pair("name", "Check out Bird Clash!"));
//    params.insert(std::make_pair("caption", "Bird Clash is the best game ever."));
//    params.insert(std::make_pair("description", "Bird Clash is the best game ever."));
//    params.insert(std::make_pair("link", "http://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?id=353372460&pageNumber=0&sortOrdering=2&type=Purple+Software&mt=8"));
//    
//    if (cocos2d::plugin::FacebookAgent::getInstance()->canPresentDialogWithParams(params))
//    {
//        cocos2d::plugin::FacebookAgent::getInstance()->dialog(params, [=](int ret ,std::string& msg){
//            CCLOG("%s", msg.c_str());
//        });
//    }
//    else
//    {
//        cocos2d::plugin::FacebookAgent::getInstance()->share(params, [=](int ret ,std::string& msg)
//                                                             {
//                                                                 CCLOG("%s", msg.c_str());
//                                                             });
//    }
    
    //*****************************************
    cocos2d::plugin::FacebookAgent::FBInfo params;
    params.insert(std::make_pair("dialog", "message_link"));
    params.insert(std::make_pair("description", "Check out Bird Clash! The ONLY Epic bird shooter game! Hit birds, score points, and repeat fun!"));
    params.insert(std::make_pair("title", "Bird Clash"));
    params. insert(std::make_pair("link", "https://itunes.apple.com/us/app/id949282668"));
    params.insert(std::make_pair("imageUrl", "http://www.cheapfreegames.com/wp-content/themes/customizr/inc/assets/img/icon.png"));
    
    if (cocos2d::plugin::FacebookAgent::getInstance()->canPresentDialogWithParams(params))
    {
        cocos2d::plugin::FacebookAgent::getInstance()->dialog(params, [=](int ret, std::string& msg){
            CCLOG("%s", msg.c_str());
        });
    }
    else
    {
        cocos2d::plugin::FacebookAgent::getInstance()->share(params, [=](int ret ,std::string& msg)
                                                             {
                                                                 CCLOG("%s", msg.c_str());
                                                             });
        
    }

    
    
#endif
}

void GameOver::Rate( cocos2d::Ref *sender )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Application::getInstance()->openURL("market://details?id=com.cubeactive.qnotelistfree");
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    Application::getInstance()->openURL("http://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?id=353372460&pageNumber=0&sortOrdering=2&type=Purple+Software&mt=8");
#endif
}





