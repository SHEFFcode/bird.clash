#include "CherryStore.h"
#include "GamePlay.h"

USING_NS_CC;

unsigned int powerup;

CherryStore::~CherryStore(){
//    cover->release();
//    notice->release();
//    background->release();
//    item1->release();
//    item2->release();
//    item3->release();
//    item4->release();
//    cherry1->release();
//    cherry2->release();
//    cherry3->release();
//    cherry4->release();
//    power1_infinity->release();
//    power2_infinity->release();
//    power3_infinity->release();
//    power1Text->release();
//    power2Text->release();
//    power3Text->release();
//    cherryText->release();
//    noticeText->release();
}

Scene* CherryStore::createScene( unsigned int power )
{
    powerup = power;
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = CherryStore::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CherryStore::init()
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
    powerNum1 = def->getIntegerForKey("power1",0);
    powerNum2 = def->getIntegerForKey("power2",0);
    powerNum3 = def->getIntegerForKey("power3",0);
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
    startWidth = (visibleSize.width - (background->getContentSize().width * background->getScaleX())) / 2;
    startHeight = (visibleSize.height - (background->getContentSize().height * background->getScaleY())) / 2;
    backgroundWidth = background->getContentSize().width * background->getScaleX();
    backgroundHeight = background->getContentSize().height * background->getScaleY();
    
    auto storeText = Label::createWithTTF("Powerups", "Arial_Regular.ttf", visibleSize.height * 0.075);
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
    
    // Done Button
    auto done = MenuItemImage::create("menu_purchase/done2.png", "menu_purchase/done2.png", CC_CALLBACK_1(CherryStore::GoToGamePlay, this));
    done->setPosition(Point(startWidth + (backgroundWidth * 0.5), startHeight + (backgroundHeight * 0.06)));
    
    auto doneText = Label::createWithTTF("Done", "Arial_Regular.ttf", visibleSize.height * 0.04);
    doneText->setColor(Color3B::BLACK);
    doneText->setPosition(Point(done->getPositionX(), done->getPositionY()));
    
    item1 = Sprite::create("menu_purchase/sale_cherry_special.png");
    item1->setPosition(Point(startWidth + (backgroundWidth * 0.2), startHeight + (backgroundHeight * 0.42)));
    
    item2 = Sprite::create("menu_purchase/sale_cherry.png");
    item2->setPosition(Point(startWidth + (backgroundWidth * 0.4), item1->getPositionY()));
    
    item3 = Sprite::create("menu_purchase/sale_cherry.png");
    item3->setPosition(Point(startWidth + (backgroundWidth * 0.6), item1->getPositionY()));
    
    item4 = Sprite::create("menu_purchase/sale_cherry.png");
    item4->setPosition(Point(startWidth + (backgroundWidth * 0.8), item1->getPositionY()));
    
    auto buy1 = MenuItemImage::create("menu_purchase/buy2.png", "menu_purchase/buy2.png", CC_CALLBACK_1(CherryStore::Sale, this, 1));
    buy1->setPosition(Point(item1->getPositionX(), item1->getPositionY() - (item1->getContentSize().height * 0.39)));
    
    auto buy2 = MenuItemImage::create("menu_purchase/buy2.png", "menu_purchase/buy2.png", CC_CALLBACK_1(CherryStore::Sale, this, 2));
    buy2->setPosition(Point(item2->getPositionX(), item1->getPositionY() - (item1->getContentSize().height * 0.39)));
    
    auto buy3 = MenuItemImage::create("menu_purchase/buy2.png", "menu_purchase/buy2.png", CC_CALLBACK_1(CherryStore::Sale, this, 3));
    buy3->setPosition(Point(item3->getPositionX(), item1->getPositionY() - (item1->getContentSize().height * 0.39)));
    
    auto buy4 = MenuItemImage::create("menu_purchase/buy2.png", "menu_purchase/buy2.png", CC_CALLBACK_1(CherryStore::Sale, this, 4));
    buy4->setPosition(Point(item4->getPositionX(), item1->getPositionY() - (item1->getContentSize().height * 0.39)));
    
    auto buyText1 = Label::createWithTTF("Buy", "Arial_Regular.ttf", visibleSize.height * 0.04);
    buyText1->setColor(Color3B::WHITE);
    buyText1->setPosition(Point(buy1->getPositionX(), buy1->getPositionY()));
    
    auto buyText2 = Label::createWithTTF("Buy", "Arial_Regular.ttf", visibleSize.height * 0.04);
    buyText2->setColor(Color3B::WHITE);
    buyText2->setPosition(Point(buy2->getPositionX(), buy1->getPositionY()));
    
    auto buyText3 = Label::createWithTTF("Buy", "Arial_Regular.ttf", visibleSize.height * 0.04);
    buyText3->setColor(Color3B::WHITE);
    buyText3->setPosition(Point(buy3->getPositionX(), buy1->getPositionY()));
    
    auto buyText4 = Label::createWithTTF("Buy", "Arial_Regular.ttf", visibleSize.height * 0.04);
    buyText4->setColor(Color3B::WHITE);
    buyText4->setPosition(Point(buy4->getPositionX(), buy1->getPositionY()));
    
    auto amount1 = Label::createWithTTF("x200", "Arial_Regular.ttf", visibleSize.height * 0.08);
    amount1->setColor(Color3B::BLACK);
    amount1->setPosition(Point(item1->getPositionX(), item1->getPositionY() + (amount1->getContentSize().height / 2)));
    
    auto amount2 = Label::createWithTTF("x100", "Arial_Regular.ttf", visibleSize.height * 0.08);
    amount2->setColor(Color3B::BLACK);
    amount2->setPosition(Point(item2->getPositionX(), amount1->getPositionY()));
    
    auto amount3 = Label::createWithTTF("x50", "Arial_Regular.ttf", visibleSize.height * 0.08);
    amount3->setColor(Color3B::BLACK);
    amount3->setPosition(Point(item3->getPositionX(), amount1->getPositionY()));
    
    auto amount4 = Label::createWithTTF("x10", "Arial_Regular.ttf", visibleSize.height * 0.08);
    amount4->setColor(Color3B::BLACK);
    amount4->setPosition(Point(item4->getPositionX(), amount1->getPositionY()));
    
    auto plus1 = Label::createWithTTF("+50 FREE", "Arial_Regular.ttf", visibleSize.height * 0.0375);
    plus1->setColor(Color3B::BLACK);
    plus1->setPosition(Point(item1->getPositionX(), amount1->getPositionY() - (amount1->getContentSize().height * 0.65)));
    
    auto plus2 = Label::createWithTTF("+25 FREE", "Arial_Regular.ttf", visibleSize.height * 0.0375);
    plus2->setColor(Color3B::BLACK);
    plus2->setPosition(Point(item2->getPositionX(), plus1->getPositionY()));
    
    auto plus3 = Label::createWithTTF("+5 FREE", "Arial_Regular.ttf", visibleSize.height * 0.0375);
    plus3->setColor(Color3B::BLACK);
    plus3->setPosition(Point(item3->getPositionX(), plus1->getPositionY()));
    
    cherry1 = Sprite::create("cherry.png");
    cherry1->setScale(0.55);
    cherry1->setPosition(Point(item1->getPositionX() - (item1->getContentSize().width / 2) + (cherry1->getContentSize().width / 2), item1->getPositionY() - (item1->getContentSize().height * 0.185)));
    
    cherry2 = Sprite::create("cherry.png");
    cherry2->setScale(0.55);
    cherry2->setPosition(Point(item2->getPositionX() - (item2->getContentSize().width / 2) + (cherry2->getContentSize().width / 2), cherry1->getPositionY()));
    
    cherry3 = Sprite::create("cherry.png");
    cherry3->setScale(0.55);
    cherry3->setPosition(Point(item3->getPositionX() - (item3->getContentSize().width / 2) + (cherry3->getContentSize().width / 2), cherry1->getPositionY()));
    
    cherry4 = Sprite::create("cherry.png");
    cherry4->setScale(0.55);
    cherry4->setPosition(Point(item4->getPositionX() - (item4->getContentSize().width / 2) + (cherry4->getContentSize().width / 2), cherry1->getPositionY()));
    
    auto cherry_total_background = Sprite::create("menu_purchase/cherry_total_background.png");
    cherry_total_background->setPosition(Vec2(item1->getPositionX(), cherry1->getPositionY()));
    
    auto cherry_total_background2 = Sprite::create("menu_purchase/cherry_total_background.png");
    cherry_total_background2->setPosition(Vec2(item2->getPositionX(), cherry1->getPositionY()));
    
    auto cherry_total_background3 = Sprite::create("menu_purchase/cherry_total_background.png");
    cherry_total_background3->setPosition(Vec2(item3->getPositionX(), cherry1->getPositionY()));
    
    auto cherry_total_background4 = Sprite::create("menu_purchase/cherry_total_background.png");
    cherry_total_background4->setPosition(Vec2(item4->getPositionX(), cherry1->getPositionY()));
    
    // Menu
    auto menu = Menu::create(buy1, buy2, buy3, buy4, done, NULL);
    menu->setPosition(Point::ZERO);
    
    this->addChild(black,1);
    this->addChild(background,2);
    this->addChild(storeText, 3);
    this->addChild(cherryText, 3);
    this->addChild(cherry, 3);
    this->addChild(menu, 4);
    this->addChild(doneText, 4);
    this->addChild(item1, 3);
    this->addChild(item2, 3);
    this->addChild(item3, 3);
    this->addChild(item4, 3);
    this->addChild(buyText1, 5);
    this->addChild(buyText2, 5);
    this->addChild(buyText3, 5);
    this->addChild(buyText4, 5);
    this->addChild(amount1, 4);
    this->addChild(amount2, 4);
    this->addChild(amount3, 4);
    this->addChild(amount4, 4);
    this->addChild(plus1, 4);
    this->addChild(plus2, 4);
    this->addChild(plus3, 4);
    this->addChild(cherry_total_background, 4);
    this->addChild(cherry_total_background2, 4);
    this->addChild(cherry_total_background3, 4);
    this->addChild(cherry_total_background4, 4);
    this->addChild(cherry1, 5);
    this->addChild(cherry2, 5);
    this->addChild(cherry3, 5);
    this->addChild(cherry4, 5);
    
    if(powerup == 1) {
        this->Blue();
    }
    if(powerup == 2) {
        this->Light();
    }
    if(powerup == 3) {
        this->Grow();
    }
    
    //setup touch listener
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches( true );
    touchListener->onTouchBegan = CC_CALLBACK_2( CherryStore::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, black);
    
    return true;
}

bool CherryStore::onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event )
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

void CherryStore::Blue() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Color3B color = {0, 173, 239};
    
    auto blueText1 = Label::createWithTTF("Blue Birds", "Arial_Regular.ttf", visibleSize.height * 0.06);
    blueText1->setColor(color);
    blueText1->setPosition(Point(visibleSize.width / 2, visibleSize.height - startHeight - (backgroundHeight * 0.185)));
    
    auto blueText2 = Label::createWithTTF("Changes all Birds to Blue and worth +2 points.", "Arial_Regular.ttf", visibleSize.height * 0.04);
    blueText2->setColor(Color3B::BLACK);
    blueText2->setPosition(Point(visibleSize.width / 2, blueText1->getPositionY() - (blueText1->getContentSize().height * 0.8)));
    
    auto blueBird1 = Sprite::create("blue_left.png");
    blueBird1->setScale(0.5);
    blueBird1->setPosition(Point(item1->getPositionX(), item1->getPositionY() + (item1->getContentSize().height * 0.335)));
    
    auto blueBird2 = Sprite::create("blue_left.png");
    blueBird2->setScale(0.5);
    blueBird2->setPosition(Point(item2->getPositionX(), blueBird1->getPositionY()));
    
    auto blueBird3 = Sprite::create("blue_left.png");
    blueBird3->setScale(0.5);
    blueBird3->setPosition(Point(item3->getPositionX(), blueBird1->getPositionY()));
    
    auto blueBird4 = Sprite::create("blue_left.png");
    blueBird4->setScale(0.5);
    blueBird4->setPosition(Point(item4->getPositionX(), blueBird1->getPositionY()));
    
    auto blue = Sprite::create("powerups/blue_menu.png");
    auto width = blue->getContentSize().width;
    blue->setPosition(Point(startWidth + (backgroundWidth * 0.5) - (width), visibleSize.height - startHeight - (blue->getContentSize().height * 0.24)));
    
    auto light = MenuItemImage::create("powerups/lightning_disabled_menu.png", "powerups/lightning_disabled_menu.png", CC_CALLBACK_1(CherryStore::GoToCherryStore, this, 2));
    light->setPosition(Point(startWidth + (backgroundWidth * 0.5), blue->getPositionY()));
    
    auto grow = MenuItemImage::create("powerups/grow_disabled_menu.png", "powerups/grow_disabled_menu.png", CC_CALLBACK_1(CherryStore::GoToCherryStore, this, 3));
    grow->setPosition(Point(startWidth + (backgroundWidth * 0.5) + (width), blue->getPositionY()));
    
    __String *pText1 = __String::createWithFormat("%i",powerNum1);
    __String *pText2 = __String::createWithFormat("%i",powerNum2);
    __String *pText3 = __String::createWithFormat("%i",powerNum3);
    
    power1Text = Label::createWithTTF( pText1->getCString(), "Arial_Regular.ttf", blue->getContentSize().width * 0.4);
    power1Text->setColor(Color3B::BLACK);
    power1Text->setPosition(Point(blue->getPositionX(), visibleSize.height - startHeight + (power1Text->getContentSize().height * 0.125)));
    
    power2Text = Label::createWithTTF( pText2->getCString(), "Arial_Regular.ttf", blue->getContentSize().width * 0.4);
    power2Text->setColor(Color3B::BLACK);
    power2Text->setPosition(Point(light->getPositionX(), power1Text->getPositionY()));
    
    power3Text = Label::createWithTTF( pText3->getCString(), "Arial_Regular.ttf", blue->getContentSize().width * 0.4);
    power3Text->setColor(Color3B::BLACK);
    power3Text->setPosition(Point(grow->getPositionX(), power1Text->getPositionY()));
    
    if(powerNum1 > 9999) {
        power1Text->setOpacity(0);
        power1_infinity = Sprite::create("powerups/infinity.png");
        power1_infinity->setPosition(Vec2(blue->getPositionX(), power1Text->getPositionY()));
        this->addChild(power1_infinity,5);
    }
    if(powerNum2 > 9999) {
        power2Text->setOpacity(0);
        power2_infinity = Sprite::create("powerups/infinity.png");
        power2_infinity->setPosition(Vec2(light->getPositionX(), power2Text->getPositionY()));
        this->addChild(power2_infinity,5);
    }
    if(powerNum3 > 9999) {
        power3Text->setOpacity(0);
        power3_infinity = Sprite::create("powerups/infinity.png");
        power3_infinity->setPosition(Vec2(grow->getPositionX(), power3Text->getPositionY()));
        this->addChild(power3_infinity,5);
    }
    
    auto get1 = Label::createWithTTF("1000", "Arial_Regular.ttf", visibleSize.height * 0.0575);
    get1->setColor(Color3B::BLACK);
    get1->setPosition(Point(cherry1->getPositionX() + (cherry1->getContentSize().width * 1.05), cherry1->getPositionY() - (cherry1->getContentSize().height * 0.025)));
    
    auto get2 = Label::createWithTTF("500", "Arial_Regular.ttf", visibleSize.height * 0.0575);
    get2->setColor(Color3B::BLACK);
    get2->setPosition(Point(cherry2->getPositionX() + (cherry2->getContentSize().width * 1.05), get1->getPositionY()));
    
    auto get3 = Label::createWithTTF("250", "Arial_Regular.ttf", visibleSize.height * 0.0575);
    get3->setColor(Color3B::BLACK);
    get3->setPosition(Point(cherry3->getPositionX() + (cherry3->getContentSize().width * 1.05), get1->getPositionY()));
    
    auto get4 = Label::createWithTTF("50", "Arial_Regular.ttf", visibleSize.height * 0.0575);
    get4->setColor(Color3B::BLACK);
    get4->setPosition(Point(cherry4->getPositionX() + (cherry4->getContentSize().width * 1.05), get1->getPositionY()));
    
    auto arrowRight = MenuItemImage::create("menu_purchase/arrow_right.png", "menu_purchase/arrow_right.png", CC_CALLBACK_1(CherryStore::GoToCherryStore, this, 2));
    arrowRight->setPosition(Vec2(visibleSize.width - startWidth - (backgroundWidth * 0.05), item1->getPositionY()));
    
    // Menu
    auto menu2 = Menu::create(arrowRight, light, grow, NULL);
    menu2->setPosition(Point::ZERO);
    
    this->addChild(menu2, 3);
    this->addChild(blue, 3);
    this->addChild(blueText1, 3);
    this->addChild(blueText2, 3);
    this->addChild(blueBird1, 4);
    this->addChild(blueBird2, 4);
    this->addChild(blueBird3, 4);
    this->addChild(blueBird4, 4);
    this->addChild(power1Text, 5);
    this->addChild(power2Text, 5);
    this->addChild(power3Text, 5);
    this->addChild(get1, 5);
    this->addChild(get2, 5);
    this->addChild(get3, 5);
    this->addChild(get4, 5);
}

void CherryStore::Light() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto blueText1 = Label::createWithTTF("Lightning", "Arial_Regular.ttf", visibleSize.height * 0.06);
    blueText1->setColor(Color3B::BLUE);
    blueText1->setPosition(Point(visibleSize.width / 2, visibleSize.height - startHeight - (backgroundHeight * 0.185)));
    
    auto blueText2 = Label::createWithTTF("Hits all the Birds instantly.", "Arial_Regular.ttf", visibleSize.height * 0.04);
    blueText2->setColor(Color3B::BLACK);
    blueText2->setPosition(Point(visibleSize.width / 2, blueText1->getPositionY() - (blueText1->getContentSize().height * 0.8)));
    
    auto power1 = Sprite::create("powerups/lightning_big_small.png");
    power1->setPosition(Point(item1->getPositionX(), item1->getPositionY() + (item1->getContentSize().height * 0.335)));
    
    auto power2 = Sprite::create("powerups/lightning_big_small.png");
    power2->setPosition(Point(item2->getPositionX(), power1->getPositionY()));
    
    auto power3 = Sprite::create("powerups/lightning_big_small.png");
    power3->setPosition(Point(item3->getPositionX(), power1->getPositionY()));
    
    auto power4 = Sprite::create("powerups/lightning_big_small.png");
    power4->setPosition(Point(item4->getPositionX(), power1->getPositionY()));
    
    auto blue = MenuItemImage::create("powerups/blue_disabled_menu.png", "powerups/blue_disabled_menu.png", CC_CALLBACK_1(CherryStore::GoToCherryStore, this, 1));
    auto width = blue->getContentSize().width;
    blue->setPosition(Point(startWidth + (backgroundWidth * 0.5) - (width), visibleSize.height - startHeight - (blue->getContentSize().height * 0.24)));
    
    auto light = Sprite::create("powerups/lightning_menu.png");
    light->setPosition(Point(startWidth + (backgroundWidth * 0.5), blue->getPositionY()));
    
    auto grow = MenuItemImage::create("powerups/grow_disabled_menu.png", "powerups/grow_disabled_menu.png", CC_CALLBACK_1(CherryStore::GoToCherryStore, this, 3));
    grow->setPosition(Point(startWidth + (backgroundWidth * 0.5) + (width), blue->getPositionY()));
    
    __String *pText1 = __String::createWithFormat("%i",powerNum1);
    __String *pText2 = __String::createWithFormat("%i",powerNum2);
    __String *pText3 = __String::createWithFormat("%i",powerNum3);
    
    power1Text = Label::createWithTTF( pText1->getCString(), "Arial_Regular.ttf", blue->getContentSize().width * 0.4);
    power1Text->setColor(Color3B::BLACK);
    power1Text->setPosition(Point(blue->getPositionX(), visibleSize.height - startHeight + (power1Text->getContentSize().height * 0.125)));
    
    power2Text = Label::createWithTTF( pText2->getCString(), "Arial_Regular.ttf", blue->getContentSize().width * 0.4);
    power2Text->setColor(Color3B::BLACK);
    power2Text->setPosition(Point(light->getPositionX(), power1Text->getPositionY()));
    
    power3Text = Label::createWithTTF( pText3->getCString(), "Arial_Regular.ttf", blue->getContentSize().width * 0.4);
    power3Text->setColor(Color3B::BLACK);
    power3Text->setPosition(Point(grow->getPositionX(), power1Text->getPositionY()));
    
    if(powerNum1 > 9999) {
        power1Text->setOpacity(0);
        power1_infinity = Sprite::create("powerups/infinity.png");
        power1_infinity->setPosition(Vec2(blue->getPositionX(), power1Text->getPositionY()));
        this->addChild(power1_infinity,5);
    }
    if(powerNum2 > 9999) {
        power2Text->setOpacity(0);
        power2_infinity = Sprite::create("powerups/infinity.png");
        power2_infinity->setPosition(Vec2(light->getPositionX(), power2Text->getPositionY()));
        this->addChild(power2_infinity,5);
    }
    if(powerNum3 > 9999) {
        power3Text->setOpacity(0);
        power3_infinity = Sprite::create("powerups/infinity.png");
        power3_infinity->setPosition(Vec2(grow->getPositionX(), power3Text->getPositionY()));
        this->addChild(power3_infinity,5);
    }
    
    auto get1 = Label::createWithTTF("2000", "Arial_Regular.ttf", visibleSize.height * 0.0575);
    get1->setColor(Color3B::BLACK);
    get1->setPosition(Point(cherry1->getPositionX() + (cherry1->getContentSize().width * 1.05), cherry1->getPositionY() - (cherry1->getContentSize().height * 0.025)));
    
    auto get2 = Label::createWithTTF("1000", "Arial_Regular.ttf", visibleSize.height * 0.0575);
    get2->setColor(Color3B::BLACK);
    get2->setPosition(Point(cherry2->getPositionX() + (cherry2->getContentSize().width * 1.05), get1->getPositionY()));
    
    auto get3 = Label::createWithTTF("500", "Arial_Regular.ttf", visibleSize.height * 0.0575);
    get3->setColor(Color3B::BLACK);
    get3->setPosition(Point(cherry3->getPositionX() + (cherry3->getContentSize().width * 1.05), get1->getPositionY()));
    
    auto get4 = Label::createWithTTF("100", "Arial_Regular.ttf", visibleSize.height * 0.0575);
    get4->setColor(Color3B::BLACK);
    get4->setPosition(Point(cherry4->getPositionX() + (cherry4->getContentSize().width * 1.05), get1->getPositionY()));
    
    auto arrowRight = MenuItemImage::create("menu_purchase/arrow_right.png", "menu_purchase/arrow_right.png", CC_CALLBACK_1(CherryStore::GoToCherryStore, this, 3));
    arrowRight->setPosition(Vec2(visibleSize.width - startWidth - (backgroundWidth * 0.05), item1->getPositionY()));
    
    auto arrowLeft = MenuItemImage::create("menu_purchase/arrow_left.png", "menu_purchase/arrow_left.png", CC_CALLBACK_1(CherryStore::GoToCherryStore, this, 1));
    arrowLeft->setPosition(Vec2(startWidth + (backgroundWidth * 0.05), item1->getPositionY()));
    
    // Menu
    auto menu2 = Menu::create(arrowRight, arrowLeft, blue, grow, NULL);
    menu2->setPosition(Point::ZERO);
    
    this->addChild(menu2, 3);
    this->addChild(light, 3);
    this->addChild(blueText1, 3);
    this->addChild(blueText2, 3);
    this->addChild(power1, 4);
    this->addChild(power2, 4);
    this->addChild(power3, 4);
    this->addChild(power4, 4);
    this->addChild(power1Text, 5);
    this->addChild(power2Text, 5);
    this->addChild(power3Text, 5);
    this->addChild(get1, 5);
    this->addChild(get2, 5);
    this->addChild(get3, 5);
    this->addChild(get4, 5);
}

void CherryStore::Grow() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto blueText1 = Label::createWithTTF("Grow", "Arial_Regular.ttf", visibleSize.height * 0.06);
    blueText1->setColor(Color3B::RED);
    blueText1->setPosition(Point(visibleSize.width / 2, visibleSize.height - startHeight - (backgroundHeight * 0.185)));
    
    auto blueText2 = Label::createWithTTF("Grows Birds for an entire game.", "Arial_Regular.ttf", visibleSize.height * 0.04);
    blueText2->setColor(Color3B::BLACK);
    blueText2->setPosition(Point(visibleSize.width / 2, blueText1->getPositionY() - (blueText1->getContentSize().height * 0.8)));
    
    auto power1 = Sprite::create("powerups/grow_big_small.png");
    power1->setPosition(Point(item1->getPositionX(), item1->getPositionY() + (item1->getContentSize().height * 0.335)));
    
    auto power2 = Sprite::create("powerups/grow_big_small.png");
    power2->setPosition(Point(item2->getPositionX(), power1->getPositionY()));
    
    auto power3 = Sprite::create("powerups/grow_big_small.png");
    power3->setPosition(Point(item3->getPositionX(), power1->getPositionY()));
    
    auto power4 = Sprite::create("powerups/grow_big_small.png");
    power4->setPosition(Point(item4->getPositionX(), power1->getPositionY()));
    
    auto blue = MenuItemImage::create("powerups/blue_disabled_menu.png", "powerups/blue_disabled_menu.png", CC_CALLBACK_1(CherryStore::GoToCherryStore, this, 1));
    auto width = blue->getContentSize().width;
    blue->setPosition(Point(startWidth + (backgroundWidth * 0.5) - (width), visibleSize.height - startHeight - (blue->getContentSize().height * 0.24)));
    
    auto light = MenuItemImage::create("powerups/lightning_disabled_menu.png", "powerups/lightning_disabled_menu.png", CC_CALLBACK_1(CherryStore::GoToCherryStore, this, 2));
    light->setPosition(Point(startWidth + (backgroundWidth * 0.5), blue->getPositionY()));
    
    auto grow = Sprite::create("powerups/grow_menu.png");
    grow->setPosition(Point(startWidth + (backgroundWidth * 0.5) + (width), blue->getPositionY()));
    
    __String *pText1 = __String::createWithFormat("%i",powerNum1);
    __String *pText2 = __String::createWithFormat("%i",powerNum2);
    __String *pText3 = __String::createWithFormat("%i",powerNum3);
    
    power1Text = Label::createWithTTF( pText1->getCString(), "Arial_Regular.ttf", blue->getContentSize().width * 0.4);
    power1Text->setColor(Color3B::BLACK);
    power1Text->setPosition(Point(blue->getPositionX(), visibleSize.height - startHeight + (power1Text->getContentSize().height * 0.125)));
    
    power2Text = Label::createWithTTF( pText2->getCString(), "Arial_Regular.ttf", blue->getContentSize().width * 0.4);
    power2Text->setColor(Color3B::BLACK);
    power2Text->setPosition(Point(light->getPositionX(), power1Text->getPositionY()));
    
    power3Text = Label::createWithTTF( pText3->getCString(), "Arial_Regular.ttf", blue->getContentSize().width * 0.4);
    power3Text->setColor(Color3B::BLACK);
    power3Text->setPosition(Point(grow->getPositionX(), power1Text->getPositionY()));
    
    if(powerNum1 > 9999) {
        power1Text->setOpacity(0);
        power1_infinity = Sprite::create("powerups/infinity.png");
        power1_infinity->setPosition(Vec2(blue->getPositionX(), power1Text->getPositionY()));
        this->addChild(power1_infinity,5);
    }
    if(powerNum2 > 9999) {
        power2Text->setOpacity(0);
        power2_infinity = Sprite::create("powerups/infinity.png");
        power2_infinity->setPosition(Vec2(light->getPositionX(), power2Text->getPositionY()));
        this->addChild(power2_infinity,5);
    }
    if(powerNum3 > 9999) {
        power3Text->setOpacity(0);
        power3_infinity = Sprite::create("powerups/infinity.png");
        power3_infinity->setPosition(Vec2(grow->getPositionX(), power3Text->getPositionY()));
        this->addChild(power3_infinity,5);
    }
    
    auto get1 = Label::createWithTTF("5000", "Arial_Regular.ttf", visibleSize.height * 0.0575);
    get1->setColor(Color3B::BLACK);
    get1->setPosition(Point(cherry1->getPositionX() + (cherry1->getContentSize().width * 1.05), cherry1->getPositionY() - (cherry1->getContentSize().height * 0.025)));
    
    auto get2 = Label::createWithTTF("2500", "Arial_Regular.ttf", visibleSize.height * 0.0575);
    get2->setColor(Color3B::BLACK);
    get2->setPosition(Point(cherry2->getPositionX() + (cherry2->getContentSize().width * 1.05), get1->getPositionY()));
    
    auto get3 = Label::createWithTTF("1250", "Arial_Regular.ttf", visibleSize.height * 0.0575);
    get3->setColor(Color3B::BLACK);
    get3->setPosition(Point(cherry3->getPositionX() + (cherry3->getContentSize().width * 1.05), get1->getPositionY()));
    
    auto get4 = Label::createWithTTF("250", "Arial_Regular.ttf", visibleSize.height * 0.0575);
    get4->setColor(Color3B::BLACK);
    get4->setPosition(Point(cherry4->getPositionX() + (cherry4->getContentSize().width * 1.05), get1->getPositionY()));
    
    auto arrowLeft = MenuItemImage::create("menu_purchase/arrow_left.png", "menu_purchase/arrow_left.png", CC_CALLBACK_1(CherryStore::GoToCherryStore, this, 2));
    arrowLeft->setPosition(Vec2(startWidth + (backgroundWidth * 0.05), item1->getPositionY()));
    
    // Menu
    auto menu2 = Menu::create(arrowLeft, blue, light, NULL);
    menu2->setPosition(Point::ZERO);
    
    this->addChild(menu2, 3);
    this->addChild(grow, 3);
    this->addChild(blueText1, 3);
    this->addChild(blueText2, 3);
    this->addChild(power1, 4);
    this->addChild(power2, 4);
    this->addChild(power3, 4);
    this->addChild(power4, 4);
    this->addChild(power1Text, 5);
    this->addChild(power2Text, 5);
    this->addChild(power3Text, 5);
    this->addChild(get1, 5);
    this->addChild(get2, 5);
    this->addChild(get3, 5);
    this->addChild(get4, 5);
}

void CherryStore::Sale( cocos2d::Ref *sender, unsigned int num)
{
    if(inSale == 0)
    {
        inSale = 1;
        
        Size visibleSize = Director::getInstance()->getVisibleSize();

        cover = Sprite::create("menu_purchase/black.jpg");
        cover->setScaleX(visibleSize.width / cover->getContentSize().width);
        cover->setScaleY(visibleSize.height / cover->getContentSize().height);
        cover->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
        cover->setOpacity(175);
        
        notice = Sprite::create("menu_purchase/notice.png");
        notice->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        
        noticeText = Label::createWithTTF("Successfully Purchased!", "Arial_Regular.ttf", visibleSize.height * 0.04);
        noticeText->setColor(Color3B::RED);
        noticeText->setPosition(Vec2(notice->getPositionX(), notice->getPositionY()));

        this->addChild(cover, 6);
        this->addChild(notice, 7);
        this->addChild(noticeText, 8);
        
        UserDefault *def = UserDefault::getInstance();
        cherries = def->getIntegerForKey("cherry_currency");
        powerNum1 = def->getIntegerForKey("power1",0);
        powerNum2 = def->getIntegerForKey("power2",0);
        powerNum3 = def->getIntegerForKey("power3",0);
        unsigned int amount;
        unsigned int add;

        if(powerup == 1){
            if(num == 1){
                amount = 1000;
                add = 250;
            }
            if(num == 2){
                amount = 500;
                add = 125;
            }
            if(num == 3){
                amount = 250;
                add = 55;
            }
            if(num == 4){
                amount = 50;
                add = 10;
            }
            if(cherries >= amount) {
                cherries = cherries - amount;
                __String *temp = __String::createWithFormat("%i", cherries);
                cherryText->setString(temp->getCString());
                def->setIntegerForKey("power1",powerNum1 + add);
                def->setIntegerForKey("cherry_currency", cherries);
                powerNum1 = powerNum1 + add;
                __String *temp2 = __String::createWithFormat("%i", powerNum1);
                power1Text->setString(temp2->getCString());
            } else {
                noticeText->setString("Insufficient Cherries!");
            }
        }
        if(powerup == 2){
            if(num == 1){
                amount = 2000;
                add = 250;
            }
            if(num == 2){
                amount = 1000;
                add = 125;
            }
            if(num == 3){
                amount = 500;
                add = 55;
            }
            if(num == 4){
                amount = 100;
                add = 10;
            }
            if(cherries >= amount) {
                cherries = cherries - amount;
                __String *temp = __String::createWithFormat("%i", cherries);
                cherryText->setString(temp->getCString());
                def->setIntegerForKey("power2",powerNum2 + add);
                def->setIntegerForKey("cherry_currency", cherries);
                powerNum2 = powerNum2 + add;
                __String *temp2 = __String::createWithFormat("%i", powerNum2);
                power2Text->setString(temp2->getCString());
            } else {
                noticeText->setString("Insufficient Cherries!");
            }
        }
        if(powerup == 3){
            if(num == 1){
                amount = 5000;
                add = 250;
            }
            if(num == 2){
                amount = 2500;
                add = 125;
            }
            if(num == 3){
                amount = 1250;
                add = 55;
            }
            if(num == 4){
                amount = 250;
                add = 10;
            }
            if(cherries >= amount) {
                cherries = cherries - amount;
                __String *temp = __String::createWithFormat("%i", cherries);
                cherryText->setString(temp->getCString());
                def->setIntegerForKey("power3",powerNum3 + add);
                def->setIntegerForKey("cherry_currency", cherries);
                powerNum3 = powerNum3 + add;
                __String *temp2 = __String::createWithFormat("%i", powerNum3);
                power3Text->setString(temp2->getCString());
            } else {
                noticeText->setString("Insufficient Cherries!");
            }
        }
        
        Sequence *seq = Sequence::create(DelayTime::create(2), CallFunc::create(std::bind(&CherryStore::RemoveNotice, this)), NULL);
        this->runAction(seq);
        
        if(powerNum1 > 9999) {
            power1Text->setOpacity(0);
            power1_infinity = Sprite::create("powerups/infinity.png");
            power1_infinity->setPosition(Vec2(power1Text->getPositionX(), power1Text->getPositionY()));
            this->addChild(power1_infinity,5);
        }
        if(powerNum2 > 9999) {
            power2Text->setOpacity(0);
            power2_infinity = Sprite::create("powerups/infinity.png");
            power2_infinity->setPosition(Vec2(power2Text->getPositionX(), power2Text->getPositionY()));
            this->addChild(power2_infinity,5);
        }
        if(powerNum3 > 9999) {
            power3Text->setOpacity(0);
            power3_infinity = Sprite::create("powerups/infinity.png");
            power3_infinity->setPosition(Vec2(power3Text->getPositionX(), power3Text->getPositionY()));
            this->addChild(power3_infinity,5);
        }
        
        def->flush();
    }
}

void CherryStore::RemoveNotice(){
    this->removeChild(noticeText);
    this->removeChild(notice);
    this->removeChild(cover);
    inSale = 0;
}

void CherryStore::GoToGamePlay( cocos2d::Ref *sender )
{
    auto scene = GamePlay::createScene(0);
    Director::getInstance()->replaceScene(scene);
}

void CherryStore::GoToCherryStore( cocos2d::Ref *sender, unsigned int num)
{
    auto scene = CherryStore::createScene(num);
    Director::getInstance()->replaceScene(scene);
}






