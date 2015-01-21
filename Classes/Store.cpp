#include "Store.h"
#include "GamePlay.h"
#include "CCStoreInventory.h"
#include "CCStoreService.h"
#include "CCSoomlaUtils.h"
#include "CCError.h"
#include "cherryAssets.h"
#include "ExampleEventHandler.h"

USING_NS_CC;

Store::~Store(){
//    background->release();
//    cherryText->release();
}

Scene* Store::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Store::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Store::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    UserDefault *def = UserDefault::getInstance();
    cherries = def->getIntegerForKey("cherry_currency", 0);
    __String *temp = __String::createWithFormat("%i", cherries);
    def->flush();
    
    auto backgroundOrig = Sprite::create("background.png");
    backgroundOrig->setScaleX(visibleSize.width / backgroundOrig->getContentSize().width);
    backgroundOrig->setScaleY(visibleSize.height / backgroundOrig->getContentSize().height);
    backgroundOrig->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(backgroundOrig);
    
    auto black = Sprite::create("menu_purchase/black.jpg");
    black->setScaleX(visibleSize.width / black->getContentSize().width);
    black->setScaleY(visibleSize.height / black->getContentSize().height);
    black->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    black->setOpacity(100);
    
    background = Sprite::create("menu_purchase/menu.png");
    background->setScaleX((visibleSize.width / background->getContentSize().width) * 0.85);
    background->setScaleY((visibleSize.height / background->getContentSize().height) * 0.85);
    background->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    
    // Menu Positioning
    auto startWidth = (visibleSize.width - (background->getContentSize().width * background->getScaleX())) / 2;
    auto startHeight = (visibleSize.height - (background->getContentSize().height * background->getScaleY())) / 2;
    auto backgroundWidth = background->getContentSize().width * background->getScaleX();
    auto backgroundHeight = background->getContentSize().height * background->getScaleY();
    
    auto storeText = Label::createWithTTF("Cherry Store", "Arial_Regular.ttf", visibleSize.height * 0.075);
    storeText->setColor(Color3B::BLACK);
    storeText->setPosition(Point(startWidth + (backgroundWidth * 0.015), startHeight + (backgroundHeight * 0.935)));
    storeText->setAnchorPoint(Vec2(0,0.5));
    
    auto cherry = Sprite::create("cherry.png");
    cherry->setPosition(Point(startWidth + (backgroundWidth * 0.95), startHeight + (backgroundHeight * 0.935)));
    cherry->setScale(0.75);
    
    cherryText = Label::createWithTTF(temp->getCString(), "Arial_Regular.ttf", visibleSize.height * 0.075);
    cherryText->setColor(Color3B::BLACK);
    cherryText->setPosition(Vec2(cherry->getPositionX() - (cherry->getContentSize().width / 2), storeText->getPositionY()));
    cherryText->setAnchorPoint(Vec2(1.0, 0.5));  // Align right
    
    auto cherrySale1 = Sprite::create("cherry.png");
    cherrySale1->setPosition(Point(startWidth + (backgroundWidth * 0.17), startHeight + (backgroundHeight * 0.68)));
    cherrySale1->setScale(0.7);
    
    auto cherrySale2 = Sprite::create("menu_purchase/cherry2.png");
    cherrySale2->setPosition(Point(startWidth + (backgroundWidth * 0.615), startHeight + (backgroundHeight * 0.68)));
    cherrySale2->setScale(0.7);
    
    auto cherrySale3 = Sprite::create("menu_purchase/cherry3.png");
    cherrySale3->setPosition(Point(startWidth + (backgroundWidth * 0.155), startHeight + (backgroundHeight * 0.3)));
    cherrySale3->setScale(0.7);
    
    auto cherrySale4 = Sprite::create("menu_purchase/cherry4.png");
    cherrySale4->setPosition(Point(startWidth + (backgroundWidth * 0.61), startHeight + (backgroundHeight * 0.3)));
    cherrySale4->setScale(0.7);
    
    // Purchase Buttons
    auto item1 = MenuItemImage::create("menu_purchase/sale.png", "menu_purchase/sale.png", CC_CALLBACK_1(Store::Sale1, this));
    item1->setPosition(Point(startWidth + (backgroundWidth * 0.27), startHeight + (backgroundHeight * 0.68)));
    
    auto item2 = MenuItemImage::create("menu_purchase/sale.png", "menu_purchase/sale.png", CC_CALLBACK_1(Store::Sale2, this));
    item2->setPosition(Point(startWidth + (backgroundWidth * 0.73), startHeight + (backgroundHeight * 0.68)));
    
    auto item3 = MenuItemImage::create("menu_purchase/sale.png", "menu_purchase/sale.png", CC_CALLBACK_1(Store::Sale3, this));
    item3->setPosition(Point(startWidth + (backgroundWidth * 0.27), startHeight + (backgroundHeight * 0.3)));
    
    auto item4 = MenuItemImage::create("menu_purchase/sale2.png", "menu_purchase/sale2.png", CC_CALLBACK_1(Store::Sale4, this));
    item4->setPosition(Point(startWidth + (backgroundWidth * 0.73), startHeight + (backgroundHeight * 0.3)));
    
//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    auto itemScale = (backgroundWidth * 0.44) / item1->getContentSize().width;
    item1->setScaleX(itemScale);
    item2->setScaleX(itemScale);
    item3->setScaleX(itemScale);
    item4->setScaleX(itemScale);
//#endif
    
    auto buy1 = Sprite::create("menu_purchase/buy.png");
    buy1->setPosition(Point(startWidth + (backgroundWidth * 0.35), startHeight + (backgroundHeight * 0.56)));
    
    auto buy2 = Sprite::create("menu_purchase/buy.png");
    buy2->setPosition(Point(startWidth + (backgroundWidth * 0.8), startHeight + (backgroundHeight * 0.56)));
    
    auto buy3 = Sprite::create("menu_purchase/buy.png");
    buy3->setPosition(Point(startWidth + (backgroundWidth * 0.35), startHeight + (backgroundHeight * 0.18)));
    
    auto buy4 = Sprite::create("menu_purchase/buy.png");
    buy4->setPosition(Point(startWidth + (backgroundWidth * 0.8), startHeight + (backgroundHeight * 0.18)));
    
    auto banner1 = Sprite::create("menu_purchase/banner.png");
    banner1->setPosition(Point(buy2->getPositionX(), startHeight + (backgroundHeight * 0.8)));
    
    auto banner2 = Sprite::create("menu_purchase/banner.png");
    banner2->setPosition(Point(buy3->getPositionX(), startHeight + (backgroundHeight * 0.42)));
    
    auto banner3 = Sprite::create("menu_purchase/banner.png");
    banner3->setPosition(Point(buy4->getPositionX(), startHeight + (backgroundHeight * 0.42)));
    
    auto bannerText1 = Label::createWithTTF("Save 20%", "Arial_Regular.ttf", visibleSize.height * 0.036);
    bannerText1->setColor(Color3B::WHITE);
    bannerText1->setPosition(Point(banner1->getPositionX(), banner1->getPositionY() + (backgroundHeight * 0.008)));
    
    auto bannerText2 = Label::createWithTTF("Save 33%", "Arial_Regular.ttf", visibleSize.height * 0.036);
    bannerText2->setColor(Color3B::WHITE);
    bannerText2->setPosition(Point(banner2->getPositionX(), banner2->getPositionY() + (backgroundHeight * 0.008)));
    
    auto bannerText3 = Label::createWithTTF("Save 50%", "Arial_Regular.ttf", visibleSize.height * 0.036);
    bannerText3->setColor(Color3B::WHITE);
    bannerText3->setPosition(Point(banner3->getPositionX(), banner3->getPositionY() + (backgroundHeight * 0.008)));
    
    auto creditsText1 = Label::createWithTTF("+400", "Arial_Regular.ttf", visibleSize.height * 0.08);
    creditsText1->setColor(Color3B::RED);
    creditsText1->setPosition(Point(buy1->getPositionX() - (backgroundWidth * 0.008), startHeight + (backgroundHeight * 0.71)));
    
    auto creditsText2 = Label::createWithTTF("+1000", "Arial_Regular.ttf", visibleSize.height * 0.08);
    creditsText2->setColor(Color3B::RED);
    creditsText2->setPosition(Point(buy2->getPositionX() - (backgroundWidth * 0.008), startHeight + (backgroundHeight * 0.71)));
    
    auto creditsText3 = Label::createWithTTF("+3000", "Arial_Regular.ttf", visibleSize.height * 0.08);
    creditsText3->setColor(Color3B::RED);
    creditsText3->setPosition(Point(buy3->getPositionX() - (backgroundWidth * 0.008), startHeight + (backgroundHeight * 0.33)));
    
    auto creditsText4 = Label::createWithTTF("+8000", "Arial_Regular.ttf", visibleSize.height * 0.08);
    creditsText4->setColor(Color3B::RED);
    creditsText4->setPosition(Point(buy4->getPositionX() - (backgroundWidth * 0.008), startHeight + (backgroundHeight * 0.33)));
    
    auto priceText1 = Label::createWithTTF("$0.99", "Arial_Regular.ttf", visibleSize.height * 0.04);
    priceText1->setColor(Color3B::BLACK);
    priceText1->setPosition(Point(buy1->getPositionX(), startHeight + (backgroundHeight * 0.635)));
    
    auto priceText2 = Label::createWithTTF("$1.99", "Arial_Regular.ttf", visibleSize.height * 0.04);
    priceText2->setColor(Color3B::BLACK);
    priceText2->setPosition(Point(buy2->getPositionX(), startHeight + (backgroundHeight * 0.635)));
    
    auto priceText3 = Label::createWithTTF("$4.99", "Arial_Regular.ttf", visibleSize.height * 0.04);
    priceText3->setColor(Color3B::BLACK);
    priceText3->setPosition(Point(buy3->getPositionX(), startHeight + (backgroundHeight * 0.255)));
    
    auto priceText4 = Label::createWithTTF("$9.99", "Arial_Regular.ttf", visibleSize.height * 0.04);
    priceText4->setColor(Color3B::BLACK);
    priceText4->setPosition(Point(buy4->getPositionX(), startHeight + (backgroundHeight * 0.255)));
    
    auto buyText1 = Label::createWithTTF("Buy", "Arial_Regular.ttf", visibleSize.height * 0.04);
    buyText1->setColor(Color3B::WHITE);
    buyText1->setPosition(Point(buy1->getPositionX(), buy1->getPositionY()));
    
    auto buyText2 = Label::createWithTTF("Buy", "Arial_Regular.ttf", visibleSize.height * 0.04);
    buyText2->setColor(Color3B::WHITE);
    buyText2->setPosition(Point(buy2->getPositionX(), buy2->getPositionY()));
    
    auto buyText3 = Label::createWithTTF("Buy", "Arial_Regular.ttf", visibleSize.height * 0.04);
    buyText3->setColor(Color3B::WHITE);
    buyText3->setPosition(Point(buy3->getPositionX(), buy3->getPositionY()));
    
    auto buyText4 = Label::createWithTTF("Buy", "Arial_Regular.ttf", visibleSize.height * 0.04);
    buyText4->setColor(Color3B::WHITE);
    buyText4->setPosition(Point(buy4->getPositionX(), buy4->getPositionY()));
    
    // Done Button
    auto done = MenuItemImage::create("menu_purchase/done2.png", "menu_purchase/done2.png", CC_CALLBACK_1(Store::GoToGamePlay, this));
    done->setPosition(Point(startWidth + (backgroundWidth * 0.5), startHeight + (backgroundHeight * 0.06)));
    
    auto doneText = Label::createWithTTF("Done", "Arial_Regular.ttf", visibleSize.height * 0.04);
    doneText->setColor(Color3B::BLACK);
    doneText->setPosition(Point(done->getPositionX(), done->getPositionY()));
    
    // Menu
    auto menu = Menu::create(item1, item2, item3, item4, done, NULL);
    menu->setPosition(Point::ZERO);
    
    this->addChild(black,1);
    this->addChild(background,2);
    this->addChild(storeText,3);
    this->addChild(cherry,3);
    this->addChild(cherryText,3);
    this->addChild(menu,3);
    this->addChild(doneText,4);
    this->addChild(cherrySale1,4);
    this->addChild(cherrySale2,4);
    this->addChild(cherrySale3,4);
    this->addChild(cherrySale4,4);
    this->addChild(creditsText1,4);
    this->addChild(creditsText2,4);
    this->addChild(creditsText3,4);
    this->addChild(creditsText4,4);
    this->addChild(priceText1,4);
    this->addChild(priceText2,4);
    this->addChild(priceText3,4);
    this->addChild(priceText4,4);
    this->addChild(banner1,4);
    this->addChild(banner2,4);
    this->addChild(banner3,4);
    this->addChild(bannerText1,5);
    this->addChild(bannerText2,5);
    this->addChild(bannerText3,5);
    this->addChild(buy1,4);
    this->addChild(buy2,4);
    this->addChild(buy3,4);
    this->addChild(buy4,4);
    this->addChild(buyText1,5);
    this->addChild(buyText2,5);
    this->addChild(buyText3,5);
    this->addChild(buyText4,5);
    
    //setup touch listener
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches( true );
    touchListener->onTouchBegan = CC_CALLBACK_2( Store::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, black);
    
    return true;
}

bool Store::onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event )
{
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Point locationInNode2 = background->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Size b = background->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    Rect rect2 = Rect(0, 0, b.width, b.height);
    
    if (rect.containsPoint(locationInNode) && !(rect2.containsPoint(locationInNode2)))
    {
        this->GoToGamePlay(this);
        return true;
    }
    return false;
}

void Store::GoToGamePlay( cocos2d::Ref *sender )
{
    auto scene = GamePlay::createScene(0);
    Director::getInstance()->replaceScene(scene);
}

void Store::Sale1( cocos2d::Ref *sender )
{
    soomla::CCError *soomlaError = NULL;
    soomla::CCStoreInventory::sharedStoreInventory()->buyItem("001", &soomlaError);
    if (soomlaError) {
        soomla::CCSoomlaUtils::logException("StoreScreen::Onclicked", soomlaError);
    }
}

void Store::Sale2( cocos2d::Ref *sender )
{
    soomla::CCError *soomlaError = NULL;
    soomla::CCStoreInventory::sharedStoreInventory()->buyItem("002", &soomlaError);
    if (soomlaError) {
        soomla::CCSoomlaUtils::logException("StoreScreen::Onclicked", soomlaError);
    }
}

void Store::Sale3( cocos2d::Ref *sender )
{
    soomla::CCError *soomlaError = NULL;
    soomla::CCStoreInventory::sharedStoreInventory()->buyItem("003", &soomlaError);
    if (soomlaError) {
        soomla::CCSoomlaUtils::logException("StoreScreen::Onclicked", soomlaError);
    }
}

void Store::Sale4( cocos2d::Ref *sender )
{
    soomla::CCError *soomlaError = NULL;
    soomla::CCStoreInventory::sharedStoreInventory()->buyItem("003", &soomlaError);
    if (soomlaError) {
        soomla::CCSoomlaUtils::logException("StoreScreen::Onclicked", soomlaError);
    }
}

void Store::onEnter() {
    CCLayer::onEnter();
    __NotificationCenter::getInstance()->addObserver(this,
                                                     callfuncO_selector(Store::updateCurrencyBalance),
                                                     EVENT_ON_CURRENCY_BALANCE_CHANGED, NULL);
}

void Store::onExit() {
    __NotificationCenter::getInstance()->removeObserver(this, EVENT_ON_CURRENCY_BALANCE_CHANGED);
    Layer::onExit();
}

void Store::updateCurrencyBalance(Ref *pBalance) {
    UserDefault *def = UserDefault::getInstance();
    cherries = cherries + ((__Integer*)pBalance)->getValue();
    __String *temp = __String::createWithFormat("%i", cherries);
    cherryText->setString(temp->getCString());
    def->setIntegerForKey("cherry_currency", cherries);
    def->flush();
}










