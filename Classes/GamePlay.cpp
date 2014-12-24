#include "GamePlay.h"
#include "Bird.h"
#include "Cherry.h"
#include "Tutorial.h"
#include "Definitions.h"
#include "Time.h"
#include "external/Box2d/Box2d.h"
#include "SimpleAudioEngine.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "external/plugin/protocols/include/PluginManager.h"
#include "external/plugin/samples/HelloPlugins/Classes/Configs.h"
#include <iOSHelper.h>
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "AdmobHelper.h"
#endif

USING_NS_CC;
bool newHighScore = false;

//AdMob Implementation
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    //AdMob Implementation
    using namespace cocos2d::plugin;

    const std::string s_aTestCases[] = {
        "Admob",
        "Flurry",
    };

    const std::string s_aTestPoses[] = {
        "Pos: Center",
        "Pos: Top",
        "Pos: TopLeft",
        "Pos: TopRight",
        "Pos: Bottom",
        "Pos: BottomLeft",
        "Pos: BottomRight",
    };
#endif

Scene* GamePlay::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GamePlay::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GamePlay::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
//AdMob Implementation
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
    _listener = new MyAdsListener();
    _admob = dynamic_cast<ProtocolAds*>(PluginManager::getInstance()->loadPlugin("AdsAdmob"));
    _flurryAds = dynamic_cast<ProtocolAds*>(PluginManager::getInstance()->loadPlugin("AdsFlurry"));
    TAdsDeveloperInfo devInfo;
#endif
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    devInfo["AdmobID"] = ADMOB_ID_IOS;
    devInfo["FlurryAppKey"] = FLURRY_KEY_IOS;
#endif
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    _admob->configDeveloperInfo(devInfo);
    _admob->setAdsListener(_listener);
    _admob->setDebugMode(true);
    
    _flurryAds->configDeveloperInfo(devInfo);
    _flurryAds->setAdsListener(_listener);
    _flurryAds->setDebugMode(true);
    
    _ads = _admob;
    _pos = ProtocolAds::kPosBottom;
    
    // init the AdsInfo
    adInfo["AdmobType"] = "1";
    adInfo["AdmobSizeEnum"] = "3";
    adInfo["FlurryAdsID"] = "BANNER_MAIN_VC";
    adInfo["FlurryAdsSize"] = "2";
    
//    _ads->showAds(adInfo, _pos);
#endif
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    iOSHelper::showAdmobBanner();
#endif
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    AdmobHelper::showAd();
#endif

    //End of AdMob Implementation
    
    srand((unsigned)time(NULL));
    
    redCheck = 0;
    redCheck2 = 0;
    redCheck3 = 0;
    
    UserDefault *def = UserDefault::getInstance();
    def->setIntegerForKey("bird1_collisions", 0);
    def->setIntegerForKey("bird2_collisions", 0);
    def->setIntegerForKey("bird3_collisions", 0);
    def->setIntegerForKey("bird1_region", 0);
    def->setIntegerForKey("bird2_region", 0);
    def->setIntegerForKey("missed_birds", 0);
    def->setIntegerForKey("bird_count", 0);
    def->setIntegerForKey("score", 0);
    def->setIntegerForKey("power3_activated", 0);
    def->setIntegerForKey("blue1", 0);
    def->setIntegerForKey("blue2", 0);
    def->setIntegerForKey("blue3", 0);
//    def->setIntegerForKey("power1",1000);
//    def->setIntegerForKey("power2",1000);
//    def->setIntegerForKey("power3",1000);
    
    auto blue_bird = rand() % 4 + 1;
    auto whichBird = rand() % 3 + 1;
    if( blue_bird == 1 )
    {
        if(whichBird == 1)
            def->setIntegerForKey("blue1", 1);
        if(whichBird == 2)
            def->setIntegerForKey("blue2", 1);
        if(whichBird == 3)
            def->setIntegerForKey("blue3", 1);
    }
    auto calculatedVelocity = (640 / visibleSize.height) * 0.002;
    def->setFloatForKey("velocity", calculatedVelocity);
    auto soundOn = def->getIntegerForKey("sound", 1);
    def->flush();
    
    // Setup Physics
    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);

    world = new b2World(gravity);
    
    world->SetAllowSleeping(false);
    
    world->SetContinuousPhysics(true);
    
    // Physics Bodies
    b2BodyDef bottomBodyDef;
    bottomBodyDef.position.Set(0.0f, 0.0f);
    b2Body* bottom = world->CreateBody(&bottomBodyDef);
    long region1 = 1;
    bottom->SetUserData( (void*)region1 );

    b2BodyDef topBodyDef;
    topBodyDef.position.Set(0.0f, 0.0f);
    b2Body* top = world->CreateBody(&topBodyDef);
    long region2 = 2;
    top->SetUserData( (void*)region2 );

    b2BodyDef rightBodyDef;
    rightBodyDef.position.Set(0.0f, 0.0f);
    b2Body* right = world->CreateBody(&rightBodyDef);
    long region3 = 3;
    right->SetUserData( (void*)region3 );

    b2BodyDef leftBodyDef;
    leftBodyDef.position.Set(0.0f, 0.0f);
    b2Body* left = world->CreateBody(&leftBodyDef);
    long region4 = 4;
    left->SetUserData( (void*)region4 );

    // Define the ground box shape.
    b2EdgeShape groundBox;
    
    // bottom
    groundBox.Set(b2Vec2(0,0), b2Vec2(visibleSize.width/PTM_RATIO,0));
    bottom->CreateFixture(&groundBox,0);
    
    // top
    groundBox.Set(b2Vec2(0,visibleSize.height/PTM_RATIO), b2Vec2(visibleSize.width/PTM_RATIO,visibleSize.height/PTM_RATIO));
    top->CreateFixture(&groundBox,0);
    
    // left
    groundBox.Set(b2Vec2(0,0), b2Vec2(0,visibleSize.height/PTM_RATIO));
    left->CreateFixture(&groundBox,0);
    
    // right
    groundBox.Set(b2Vec2(visibleSize.width/PTM_RATIO,0), b2Vec2(visibleSize.width/PTM_RATIO,visibleSize.height/PTM_RATIO));
    right->CreateFixture(&groundBox,0);
    
    // Add Background
    auto background = Sprite::create("background.png");
    background->setScaleX(visibleSize.width / background->getContentSize().width);
    background->setScaleY(visibleSize.height / background->getContentSize().height);
    background->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(background);
    
    // Play Button
    auto playItem = MenuItemImage::create("play_button_background.png", "play_button_background.png", CC_CALLBACK_1(GamePlay::StartGame, this));
    auto height = (visibleSize.height * 0.45);
    auto ratio = height / playItem->getContentSize().width;
    playItem->setScale(ratio);
    playItem->setPosition(Point(visibleSize.width / 2, visibleSize.height * 0.415));
    
    // Tutorial
    auto tutorial = MenuItemImage::create("question.png", "question.png", CC_CALLBACK_1(GamePlay::Tutorial, this));
    tutorial->setPosition(Point(((visibleSize.width / 2 - playItem->getContentSize().width) / 2), visibleSize.height * 0.55));
    
    // Sound
    soundSprite = Sprite::create("sound.png");
    auto sound = MenuItemSprite::create(soundSprite, soundSprite, CC_CALLBACK_1(GamePlay::Sound, this));
    sound->setPosition(Point(((visibleSize.width / 2 - playItem->getContentSize().width) / 2), visibleSize.height * 0.28));
    
    if(!soundOn)
    {
        soundSprite->setTexture("no_sound.png");
    }
    
    // Store
    auto storeItem = MenuItemImage::create("store.png", "store.png", CC_CALLBACK_1(GamePlay::GoToStore, this));
    storeItem->setPosition(Point(visibleSize.width - tutorial->getPositionX(), visibleSize.height * 0.55));
    
    // Cherry
    auto cherryItem = MenuItemImage::create("cherry_front.png", "cherry_front.png", CC_CALLBACK_1(GamePlay::UseCherries, this));
    cherryItem->setPosition(Point(visibleSize.width - tutorial->getPositionX(), visibleSize.height * 0.28));
    
    // Menu
    menu = Menu::create(playItem, tutorial, sound, storeItem, cherryItem, NULL);
    menu->setPosition(Point::ZERO);
    
    this->addChild(menu);

    playText = Label::createWithTTF("Play", "Arial_Regular.ttf", visibleSize.height * 0.1);
    playText->setColor(Color3B::WHITE);
    playText->setPosition(Point(visibleSize.width / 2, visibleSize.height * 0.415));

    this->addChild(playText,1);
    
    Color3B color = {0, 173, 239};
    
    gameText = Label::createWithTTF( "Bird Clash", "ShowG.ttf", visibleSize.height * 0.2);
    gameText->setColor(color);
    gameText->setPosition(Point(visibleSize.width / 2, visibleSize.height * 0.8));
    
    this->addChild(gameText,1);
    
    // Create contact listener
    _contactListener = new MyContactListener();
    world->SetContactListener(_contactListener);
    
    
    scheduleUpdate();
    
    return true;
}

void GamePlay::StartGame(cocos2d::Ref *sender)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    iOSHelper::hideAdmobBanner();
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    AdmobHelper::hideAd();
#endif
    UserDefault *def = UserDefault::getInstance();
    auto soundOn = def->getIntegerForKey("sound");
    auto currency = def->getIntegerForKey("cherry_currency", 0);
    def->flush();
    
    if(soundOn)
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/ambience.mp3", true);
    
    srand((unsigned)time(NULL));
    
    auto createCherry = rand() % 2 + 1;
    if(createCherry == 1) {
        auto cherryTime = rand() % 3 + 1;
        this->scheduleOnce(schedule_selector(GamePlay::AddCherry), cherryTime);
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    this->removeChild(menu);
    this->removeChild(gameText);
    
    // Move and change PlayText to be the score
    playText->setPositionY(visibleSize.height * 0.8);
    playText->setColor(Color3B::RED);
    playText->setOpacity(95);
    __String *tempScore = __String::createWithFormat("%i", score);
    playText->setString(tempScore->getCString());
    
    // Add X's
    redX = Sprite::create("BlackX.png");
    auto xWidth = redX->getContentSize().width;
    auto adjust = visibleSize.height - (visibleSize.height - (redX->getContentSize().height * 0.75));
    redX->setPosition(Point(visibleSize.width - adjust, visibleSize.height - (redX->getContentSize().height * 0.75)));
    redX->setOpacity(95);
    redX->retain();
    this->addChild(redX);
    
    redX2 = Sprite::create("BlackX.png");
    redX2->setPosition(Point( redX->getPositionX() - (xWidth * 1.25), redX->getPositionY()));
    redX2->setOpacity(95);
    redX2->retain();
    this->addChild(redX2);
    
    redX3 = Sprite::create("BlackX.png");
    redX3->setPosition(Point( redX2->getPositionX() - (xWidth * 1.25), redX2->getPositionY()));
    redX3->setOpacity(95);
    redX3->retain();
    this->addChild(redX3);
    
    powerups = new Powerups(this,world);
    
    auto instructions = Label::createWithTTF("Tap birds to score points.", "Arial_Regular.ttf", visibleSize.height * 0.05);
    instructions->setColor(Color3B::BLACK);
    instructions->setOpacity(125);
    instructions->setPosition(Point(visibleSize.width / 2, playText->getPositionY() - (playText->getContentSize().height / 2) - (instructions->getContentSize().height / 2)));
    
    this->addChild(instructions,1);
    
    auto hide = FadeTo::create(1,0);
    
    Sequence *seq = Sequence::create(DelayTime::create(4), hide, NULL);
    instructions->runAction(seq);
    
    cherrySprite = Sprite::create("cherry.png");
    cherrySprite->setPosition(Vec2((cherrySprite->getContentSize().width * 0.4), redX->getPositionY()));
    cherrySprite->setOpacity(125);
    cherrySprite->setScale(0.5);
    this->addChild(cherrySprite);
    
    __String *tmpCurrency = __String::createWithFormat("%i", currency);
    
    cherryScore = Label::createWithTTF( tmpCurrency->getCString(), "Arial_Regular.ttf", visibleSize.height * 0.05);
    cherryScore->setColor(Color3B::RED);
    cherryScore->setOpacity(95);
    cherryScore->setPosition(Vec2(cherrySprite->getPositionX() + (cherrySprite->getContentSize().width / 2) + (cherryScore->getContentSize().width / 2), cherrySprite->getPositionY() - (cherryScore->getContentSize().height * 0.15)));
    
    this->addChild(cherryScore,1);
    
    this->scheduleOnce(schedule_selector(GamePlay::AddBird), 0.5);
    this->scheduleOnce(schedule_selector(GamePlay::AddBird2), 1);
    this->scheduleOnce(schedule_selector(GamePlay::AddBird3), 1.5);
}

void GamePlay::AddBird( float dt )
{
    bird1 = new Bird(this,world);
    srand((unsigned)time(NULL));
}
void GamePlay::AddBird2 ( float dt )
{
    bird2 = new Bird(this,world);
    srand(1234);
}
void GamePlay::AddBird3 ( float dt )
{
    bird3 = new Bird(this,world);
    srand(5678);
}

void GamePlay::AddCherry( float dt )
{
    cherryCreated = 1;
    cherry = new Cherry(this, cherryScore);
    this->scheduleOnce(schedule_selector(GamePlay::RemoveCherry), 2);
}

void GamePlay::RemoveCherry( float dt )
{
    if(this->getChildByTag(999)){
        this->removeChildByTag(999);
    }
    cherryCreated = 0;
}

void GamePlay::update( float dt )
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    UserDefault *def = UserDefault::getInstance();
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 8;
    int32 positionIterations = 1;
    auto bird_count = def->getIntegerForKey("bird_count", 0);
    auto missed_birds = def->getIntegerForKey("missed_birds", 0);
    auto blue1 = def->getIntegerForKey("blue1");
    auto blue2 = def->getIntegerForKey("blue2");
    auto blue3 = def->getIntegerForKey("blue3");
    auto soundOn = def->getIntegerForKey("sound");
    auto power3_activated = def->getIntegerForKey("power3_activated");
    long whichBird;
    
    for (int32 i = 0; i < 60; i++)
    {
        world->Step(timeStep, velocityIterations, positionIterations);

        for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
        {
            if(b->GetType() == b2_dynamicBody)
            {
                Sprite* sprite = (Sprite*)b->GetUserData();
                sprite->setPosition( Vec2( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
                
                // Remove touchlisteners from all birdSprites if gameover
                if(missed_birds >= 3) {
                    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(sprite);
                }
                if(sprite->getTag() == 0) {
                    if(soundOn)
                        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/quack.wav");

                    for (b2Fixture *f = b->GetFixtureList(); f; f=f->GetNext()) {
                        if (f->GetUserData() != NULL) {
                            whichBird = (long)f->GetUserData();
                        }
                    }
                    if((whichBird == 1 && !blue1) || (whichBird == 2 && !blue2) || (whichBird == 3 && !blue3)) {
                        sprite->setTexture("bird_front.png");
                    } else {
                        sprite->setTexture("blue_front.png");
     
                        if(soundOn) {
                            Sequence *seq = Sequence::create(DelayTime::create(0.25), CallFunc::create(std::bind(&GamePlay::PlayQuack, this)), NULL);
                            this->runAction(seq);
                        }
                        // Add +2 here
                        auto plus2 = Label::createWithTTF("+2", "Arial_Regular.ttf", visibleSize.height * 0.08);
                        Color3B color = {0, 173, 239};
                        plus2->setColor(color);
                        plus2->setPosition(Vec2(sprite->getPositionX(), sprite->getPositionY() + (sprite->getContentSize().height / 2) + (plus2->getContentSize().height / 2)));
                        
                        if(!power3_activated){
                            plus2->setPosition(Vec2(plus2->getPositionX(), sprite->getPositionY() + (sprite->getContentSize().height / 1.5 / 2) + (plus2->getContentSize().height / 2) ));
                        }
                        
                        this->addChild(plus2,1);
                        Sequence *seq2 = Sequence::create(DelayTime::create(1), CallFunc::create(std::bind(&GamePlay::RemoveText, this, plus2)), NULL);
                        this->runAction(seq2);
                    }
                    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(sprite);
                    sprite->setRotation(0);
                    world->DestroyBody(b);
                    // Setting tag to something different so it doesn't reschedule
                    sprite->setTag(001);
                    Sequence *seq = Sequence::create(DelayTime::create(1), CallFunc::create(std::bind(&GamePlay::RemoveBird, this, sprite)), NULL);
                    this->runAction(seq);
                    
                    // Adjust score
                    auto score = def->getIntegerForKey("score", 0);
                    auto highScore = def->getIntegerForKey("highScore", 0);
                    score++;
                    if((whichBird == 1 && blue1) || (whichBird == 2 && blue2) || (whichBird == 3 && blue3)) {
                        score++;
                    }
                    def->setIntegerForKey("score", score);
                    
                    __String *tempScore = __String::createWithFormat("%i", score);
                    
                    playText->setString(tempScore->getCString());
                    
                    if(score > highScore)
                    {
                        highScore = score;
                        def->setIntegerForKey("highScore", highScore);
                        newHighScore = true;
                    }
                    
                    // Add to bird count for the round
                    bird_count++;
                    def->setIntegerForKey("bird_count", bird_count);
                }
            }
        }
    }
    if(missed_birds == 1 && redCheck3 == 0) {
        redCheck3 = 1;
        if(soundOn)
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/flyAway.mp3");
        redX3->setTexture("RedX.png");
        redX3->setScale(1.5);
        redX3->setOpacity(255);
        auto scale = ScaleTo::create(0.5, 1);
        redX3->runAction(scale);
    }
    if(missed_birds == 2 && redCheck2 == 0) {
        redCheck2 = 1;
        if(soundOn)
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/flyAway.mp3");
        redX2->setTexture("RedX.png");
        redX2->setScale(1.5);
        redX2->setOpacity(255);
        auto scale = ScaleTo::create(0.5, 1);
        redX2->runAction(scale);
    }
    if(missed_birds == 3 && redCheck == 0) {
        redCheck = 1;
        if(soundOn)
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/flyAway.mp3");
        redX->setTexture("RedX.png");
        redX->setScale(1.5);
        redX->setOpacity(255);
        auto scale = ScaleTo::create(0.5, 1);
        redX->runAction(scale);
    }
    
    if(bird_count >= 3 && missed_birds < 3)
    {
        def->setIntegerForKey("bird_count", 0);
        this->scheduleOnce(schedule_selector(GamePlay::ResetBirds), 0.5);
    }
    
    if(missed_birds >= 3) {
        def->setIntegerForKey("bird1_collisions", 3);
        def->setIntegerForKey("bird2_collisions", 3);
        def->setIntegerForKey("bird3_collisions", 3);
        def->setIntegerForKey("missed_birds", 0);
        def->setIntegerForKey("bird_count", 0);
        this->scheduleOnce(schedule_selector(GamePlay::RemoveBirds), 0.5);
        this->GameOver();
    }

    def->flush();
}

void GamePlay::GameOver()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    iOSHelper::showAdmobBanner();
#endif
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    AdmobHelper::showAd();
#endif
    
    this->RemoveCherry(0);
    powerups->HideAll();
    
    auto hide = FadeTo::create(1,0);
    auto hide2 = FadeTo::create(1,0);
    auto hide3 = FadeTo::create(1,0);
    auto hide4 = FadeTo::create(1,0);
    auto hide5 = FadeTo::create(1,0);

    redX->runAction(hide);
    redX2->runAction(hide2);
    redX3->runAction(hide3);
    cherrySprite->runAction(hide4);
    cherryScore->runAction(hide5);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    playText->setOpacity(0);
    
    UserDefault *def = UserDefault::getInstance();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto score = def->getIntegerForKey("score", 0);
    auto highScore = def->getIntegerForKey("highScore", 0);
    def->flush();
    
    __String *gameOverText = __String::create("Game Over");
    __String *tempScore = __String::createWithFormat("%i", score);
    __String *tempHighScore = __String::createWithFormat("%i", highScore);
    __String *tempScoreText = __String::create("Score");
    __String *tempHighScoreText = __String::create("Best");
    
    // Add game over popover
    popover = Node::create();
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
    auto home = MenuItemImage::create("home.png", "home.png", CC_CALLBACK_1(GamePlay::Home, this));
    auto w = home->getContentSize().width;
    home->setPosition(Vec2(visibleSize.width / 2 - (w * 2), visibleSize.height * 0.215));
    
    auto facebook = MenuItemImage::create("facebook.png", "facebook.png", CC_CALLBACK_1(GamePlay::Share, this));
    facebook->setPosition(Vec2(visibleSize.width / 2 - (w * 0.65), visibleSize.height * 0.215));
    
    auto rate = MenuItemImage::create("rate.png", "rate.png", CC_CALLBACK_1(GamePlay::Rate, this));
    rate->setPosition(Vec2(visibleSize.width / 2 + (w * 0.65), visibleSize.height * 0.215));
    
    auto replay = MenuItemImage::create("replay.png", "replay.png", CC_CALLBACK_1(GamePlay::Replay, this));
    replay->setPosition(Vec2(visibleSize.width / 2 + (w * 2), visibleSize.height * 0.215));
    
    auto menu2 = Menu::create(home,facebook,rate,replay, NULL);
    menu2->setPosition(Point::ZERO);
    
    popover->addChild(menu2);
    
    this->addChild(popover,10000);
}

// Removes birds then reset birds with new velocity
void GamePlay::ResetBirds( float dt )
{
    UserDefault *def = UserDefault::getInstance();
    // Failsafe for resetting birds if 3 misses
    auto missed = def->getIntegerForKey("missed_birds", 0);
    if(missed >= 3) {
        return;
    }
    
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        if(b->GetType() == b2_dynamicBody)
        {
            Sprite* sprite = (Sprite*)b->GetUserData();
            this->removeChild(sprite);
            world->DestroyBody(b);
        }
    }
    
    srand((unsigned)time(NULL));
    
    if(cherryCreated == 0)
    {
        auto createCherry = rand() % 2 + 1;
        if(createCherry == 1) {
            auto cherryTime = rand() % 3 + 1;
            this->scheduleOnce(schedule_selector(GamePlay::AddCherry), cherryTime);
        }
    }
    
    auto blue_bird = rand() % 4 + 1;
    auto whichBird = rand() % 3 + 1;
    
    def->setIntegerForKey("blue1", 0);
    def->setIntegerForKey("blue2", 0);
    def->setIntegerForKey("blue3", 0);
    if( blue_bird == 1 )
    {
        if(whichBird == 1)
            def->setIntegerForKey("blue1", 1);
        if(whichBird == 2)
            def->setIntegerForKey("blue2", 1);
        if(whichBird == 3)
            def->setIntegerForKey("blue3", 1);
    }
    def->setIntegerForKey("bird1_collisions", 0);
    def->setIntegerForKey("bird2_collisions", 0);
    def->setIntegerForKey("bird3_collisions", 0);
    def->setIntegerForKey("bird1_region", 0);
    def->setIntegerForKey("bird2_region", 0);
    def->setIntegerForKey("redCheck", 0);
    def->setIntegerForKey("redCheck2", 0);
    def->setIntegerForKey("redCheck3", 0);
    auto velocity = def->getFloatForKey("velocity");
    velocity = velocity + 0.0005;
    def->setFloatForKey("velocity", velocity);
    def->flush();
   
    this->scheduleOnce(schedule_selector(GamePlay::AddBird), 0.5);
    this->scheduleOnce(schedule_selector(GamePlay::AddBird2), 1);
    this->scheduleOnce(schedule_selector(GamePlay::AddBird3), 1.5);
}

// Removes all the birds
void GamePlay::RemoveBirds( float dt )
{
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        if(b->GetType() == b2_dynamicBody)
        {
            Sprite* sprite = (Sprite*)b->GetUserData();
            this->removeChild(sprite);
            world->DestroyBody(b);
        }
    }
}

void GamePlay::Home(cocos2d::Ref *sender)
{
    auto scene = GamePlay::createScene();
    Director::getInstance()->replaceScene(scene);
}

void GamePlay::Share(cocos2d::Ref *sender)
{
    
}

void GamePlay::Rate(cocos2d::Ref *sender)
{
    
}

void GamePlay::Replay(cocos2d::Ref *sender)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    this->removeChild(popover);
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    iOSHelper::hideAdmobBanner();
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    AdmobHelper::hideAd();
#endif
    
    powerups->ShowAll();
    
    cherrySprite->setOpacity(255);
    cherryScore->setOpacity(255);
    
    redX->setTexture("BlackX.png");
    redX->setOpacity(95);
    redX2->setTexture("BlackX.png");
    redX2->setOpacity(95);
    redX3->setTexture("BlackX.png");
    redX3->setOpacity(95);
    
    playText->setOpacity(95);
    playText->setString("0");
    
    cherrySprite->setOpacity(255);
    cherryScore->setOpacity(255);
    
    redCheck = 0;
    redCheck2 = 0;
    redCheck3 = 0;
    
    srand((unsigned)time(NULL));
    auto blue_bird = rand() % 4 + 1;
    auto whichBird = rand() % 3 + 1;
    
    UserDefault *def = UserDefault::getInstance();
    def->setIntegerForKey("blue1", 0);
    def->setIntegerForKey("blue2", 0);
    def->setIntegerForKey("blue3", 0);
    auto soundOn = def->getIntegerForKey("sound", 1);
    if(soundOn)
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/ambience.mp3", true);
    
    if( blue_bird == 1 )
    {
        if(whichBird == 1)
            def->setIntegerForKey("blue1", 1);
        if(whichBird == 2)
            def->setIntegerForKey("blue2", 1);
        if(whichBird == 3)
            def->setIntegerForKey("blue3", 1);
    }

    auto createCherry = rand() % 2 + 1;
    if(createCherry == 1) {
        auto cherryTime = rand() % 3 + 1;
        this->scheduleOnce(schedule_selector(GamePlay::AddCherry), cherryTime);
    }
    
    def->setIntegerForKey("bird1_collisions", 0);
    def->setIntegerForKey("bird2_collisions", 0);
    def->setIntegerForKey("bird3_collisions", 0);
    def->setIntegerForKey("bird1_region", 0);
    def->setIntegerForKey("bird2_region", 0);
    def->setIntegerForKey("missed_birds", 0);
    def->setIntegerForKey("bird_count", 0);
    def->setIntegerForKey("score", 0);
    def->setIntegerForKey("power3_activated", 0);
    auto calculatedVelocity = (640 / visibleSize.height) * 0.002;
    def->setFloatForKey("velocity", calculatedVelocity);
    def->flush();
    
    srand((unsigned)time(NULL));
    
    this->scheduleOnce(schedule_selector(GamePlay::AddBird), 0.5);
    this->scheduleOnce(schedule_selector(GamePlay::AddBird2), 1);
    this->scheduleOnce(schedule_selector(GamePlay::AddBird3), 1.5);
}

void GamePlay::RemoveBird( cocos2d::Sprite *sprite )
{
    this->removeChild(sprite);
}

void GamePlay::FacebookShare(cocos2d::Ref *sender)
{
    
}

void GamePlay::Tutorial(cocos2d::Ref *sender)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    iOSHelper::hideAdmobBanner();
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    AdmobHelper::hideAd();
#endif
    
    auto scene = Tutorial::createScene();
    Director::getInstance()->replaceScene(scene);
}

void GamePlay::Sound(cocos2d::Ref *sender)
{
    UserDefault *def = UserDefault::getInstance();
    auto soundOn = def->getIntegerForKey("sound");
    if(soundOn) {
        soundSprite->setTexture("no_sound.png");
        def->setIntegerForKey("sound",0);
    } else {
        soundSprite->setTexture("sound.png");
        def->setIntegerForKey("sound",1);
    }
    def->flush();
}

void GamePlay::GoToStore(cocos2d::Ref *sender)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    iOSHelper::hideAdmobBanner();
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    AdmobHelper::hideAd();
#endif
    
    auto scene = Store::createScene();
    Director::getInstance()->replaceScene(scene);
}

void GamePlay::UseCherries(cocos2d::Ref *sender)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    iOSHelper::hideAdmobBanner();
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    AdmobHelper::hideAd();
#endif
    
    auto scene = CherryStore::createScene();
    Director::getInstance()->replaceScene(scene);
}

void GamePlay::PlayQuack()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/quack.wav");
}

void GamePlay::RemoveText( cocos2d::Label *label) {
    this->removeChild(label);
}




