#include "GamePlay.h"
#include "Bird.h"
#include "Cherry.h"
#include "Tutorial.h"
#include "GameOver.h"
#include "Definitions.h"
#include "Time.h"
#include "external/Box2d/Box2d.h"
#include "SimpleAudioEngine.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "PluginManager.h"
#include "Configs.h"
#include <iOSHelper.h>
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "AdmobHelper.h"
#endif

USING_NS_CC;

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

unsigned int gameStarted;

GamePlay::~GamePlay() {
    CC_SAFE_DELETE(world);
    
    bottom = NULL;
    top = NULL;
    right = NULL;
    left = NULL;
    
    delete _contactListener;
    delete powerups;
    delete bird1;
    delete bird2;
    delete bird3;
    delete cherry;
    
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    _caseItem->release();
//    _posItem->release();
//    
//    delete _flurryAds;
//    delete _admob;
//    delete _listener;
//    delete _ads;
//#endif
    
}

Scene* GamePlay::createScene( unsigned int startGame )
{
    gameStarted = startGame;
    
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
    def->setIntegerForKey("misses", 0);
    def->setIntegerForKey("bird_count", 0);
    def->setIntegerForKey("score", 0);
    def->setIntegerForKey("power3_activated", 0);
    def->setIntegerForKey("blue1", 0);
    def->setIntegerForKey("blue2", 0);
    def->setIntegerForKey("blue3", 0);
    auto freshInstall = def->getIntegerForKey("install", 0);
    if(freshInstall == 0) {
        def->setIntegerForKey("install", 1);
        def->setIntegerForKey("power1",5);
        def->setIntegerForKey("power2",5);
        def->setIntegerForKey("power3",2);
        def->getIntegerForKey("sound", 1);
    }
//    def->setIntegerForKey("power1",0);
//    def->setIntegerForKey("power2",0);
//    def->setIntegerForKey("power3",0);
//    def->setIntegerForKey("cherry_currency", 9999);
    
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
    
    auto calculatedVelocity = (640 / visibleSize.height) * 0.001;
    if((visibleSize.width == 1024 && visibleSize.height == 768) || (visibleSize.width == 2048 && visibleSize.height == 1536)) {
        calculatedVelocity = (500 / visibleSize.height) * 0.005;
    }
    def->setFloatForKey("velocity", calculatedVelocity);
    auto soundOn = def->getIntegerForKey("sound");
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
    bottom = world->CreateBody(&bottomBodyDef);
    long region1 = 1;
    bottom->SetUserData( (void*)region1 );

    b2BodyDef topBodyDef;
    topBodyDef.position.Set(0.0f, 0.0f);
    top = world->CreateBody(&topBodyDef);
    long region2 = 2;
    top->SetUserData( (void*)region2 );

    b2BodyDef rightBodyDef;
    rightBodyDef.position.Set(0.0f, 0.0f);
    right = world->CreateBody(&rightBodyDef);
    long region3 = 3;
    right->SetUserData( (void*)region3 );

    b2BodyDef leftBodyDef;
    leftBodyDef.position.Set(0.0f, 0.0f);
    left = world->CreateBody(&leftBodyDef);
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
    
    if(gameStarted)
        this->StartGame(this);
    
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
    playText->setOpacity(125);
    playText->setScale(1.5);
    __String *tempScore = __String::createWithFormat("%i", score);
    playText->setString(tempScore->getCString());
    
    // Add X's
    redX = Sprite::create("BlackX.png");
    auto xWidth = redX->getContentSize().width;
    auto adjust = (redX->getContentSize().height * 1.5) * 0.75;
    redX->setPosition(Point(visibleSize.width - adjust, visibleSize.height - adjust));
    redX->setOpacity(125);
    redX->retain();
    redX->setScale(1.5);
    this->addChild(redX);
    
    redX2 = Sprite::create("BlackX.png");
    redX2->setPosition(Point( redX->getPositionX() - ((xWidth * 1.5) * 1.25), redX->getPositionY()));
    redX2->setOpacity(125);
    redX2->retain();
    redX2->setScale(1.5);
    this->addChild(redX2);
    
    redX3 = Sprite::create("BlackX.png");
    redX3->setPosition(Point( redX2->getPositionX() - ((xWidth * 1.5) * 1.25), redX2->getPositionY()));
    redX3->setOpacity(125);
    redX3->retain();
    redX3->setScale(1.5);
    this->addChild(redX3);
    
    powerups = new Powerups(this,world);
    
    auto instructions = Label::createWithTTF("Tap birds to score points.", "Arial_Regular.ttf", visibleSize.height * 0.085);
    instructions->setColor(Color3B::BLACK);
    instructions->setOpacity(125);
    instructions->setPosition(Point(visibleSize.width / 2, playText->getPositionY() - ((playText->getContentSize().height * 1.5) / 2) - (instructions->getContentSize().height / 2)));
    
    this->addChild(instructions,1);
    
    auto hide = FadeTo::create(1,0);
    
    Sequence *seq = Sequence::create(DelayTime::create(4), hide, NULL);
    instructions->runAction(seq);
    
    cherrySprite = Sprite::create("cherry.png");
    cherrySprite->setPosition(Vec2((cherrySprite->getContentSize().width * 0.65), visibleSize.height - (cherrySprite->getContentSize().width * 0.65)));
    cherrySprite->setOpacity(175);
    cherrySprite->setScale(0.85);
    this->addChild(cherrySprite);
    
    __String *tmpCurrency = __String::createWithFormat("%i", currency);
    
    cherryScore = Label::createWithTTF( tmpCurrency->getCString(), "Arial_Regular.ttf", visibleSize.height * 0.1);
    cherryScore->setColor(Color3B::RED);
    cherryScore->setOpacity(125);
    cherryScore->setPosition(Vec2(cherrySprite->getPositionX() + (cherrySprite->getContentSize().width * 0.75) + (cherryScore->getContentSize().width / 2), cherrySprite->getPositionY() - (cherryScore->getContentSize().height * 0.1)));
    
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
    auto misses = def->getIntegerForKey("misses", 0);
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
                if( misses >= 3) {
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
                        plus2->setPosition(Vec2(sprite->getPositionX(), sprite->getPositionY() + (sprite->getContentSize().height * 1.5 / 2) + (plus2->getContentSize().height / 2)));
                        
                        if(!power3_activated){
                            plus2->setPosition(Vec2(plus2->getPositionX(), sprite->getPositionY() + (sprite->getContentSize().height / 2) + (plus2->getContentSize().height / 2) ));
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
                        def->setIntegerForKey("newHighScore", 1);
                    } else {
                        def->setIntegerForKey("newHighScore", 0);
                    }
                    
                    // Add to bird count for the round
                    bird_count++;
                    def->setIntegerForKey("bird_count", bird_count);
                }
            }
        }
    }
    if( misses == 1 && redCheck3 == 0) {
        redCheck3 = 1;
        if(soundOn)
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/flyAway.mp3");
        redX3->setTexture("RedX.png");
        redX3->setScale(2.5);
        redX3->setOpacity(255);
        auto scale = ScaleTo::create(0.5, 1.5);
        redX3->runAction(scale);
    }
    if( misses == 2 && redCheck2 == 0) {
        redCheck2 = 1;
        if(soundOn)
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/flyAway.mp3");
        redX2->setTexture("RedX.png");
        redX2->setScale(2.5);
        redX2->setOpacity(255);
        auto scale = ScaleTo::create(0.5, 1.5);
        redX2->runAction(scale);
    }
    if(misses == 3 && redCheck == 0) {
        redCheck = 1;
        if(soundOn)
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/flyAway.mp3");
        redX->setTexture("RedX.png");
        redX->setScale(2.5);
        redX->setOpacity(255);
        auto scale = ScaleTo::create(0.5, 1.5);
        redX->runAction(scale);
    }
    
    if(bird_count >= 3 && misses < 3)
    {
        def->setIntegerForKey("bird_count", 0);
        this->scheduleOnce(schedule_selector(GamePlay::ResetBirds), 0.5);
    }
    
    if(misses >= 3) {
        def->setIntegerForKey("bird1_collisions", 3);
        def->setIntegerForKey("bird2_collisions", 3);
        def->setIntegerForKey("bird3_collisions", 3);
        def->setIntegerForKey("misses", 0);
        def->setIntegerForKey("bird_count", 0);
        this->scheduleOnce(schedule_selector(GamePlay::GameOver), 0.5);
    }

    def->flush();
}

void GamePlay::GameOver( float dt )
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    iOSHelper::showAdmobBanner();
#endif
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    AdmobHelper::showAd();
#endif
    
    playText->setOpacity(0);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    auto scene = GameOver::createScene();
    Director::getInstance()->replaceScene(TransitionCrossFade::create(2, scene));
}

// Removes birds then reset birds with new velocity
void GamePlay::ResetBirds( float dt )
{
    UserDefault *def = UserDefault::getInstance();
    // Failsafe for resetting birds if 3 misses
    auto missed1 = def->getIntegerForKey("bird1_miss", 0);
    auto missed2 = def->getIntegerForKey("bird2_miss", 0);
    auto missed3 = def->getIntegerForKey("bird3_miss", 0);
    
    if((missed1 + missed2 + missed3) >= 3) {
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

void GamePlay::RemoveBird( cocos2d::Sprite *sprite )
{
    this->removeChild(sprite);
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
    
    auto scene = CherryStore::createScene(1);
    Director::getInstance()->replaceScene(scene);
}

void GamePlay::PlayQuack()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/quack.wav");
}

void GamePlay::RemoveText( cocos2d::Label *label) {
    this->removeChild(label);
}




