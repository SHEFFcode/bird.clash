#include "CherryStore.h"
#include "GamePlay.h"

USING_NS_CC;

Scene* CherryStore::createScene()
{
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
    
    __String *storeString = __String::create("Store");
    __String *doneString = __String::create("Done");
    
    auto storeText = Label::createWithTTF( storeString->getCString(), "fonts/Arial_Regular.ttf", visibleSize.height * 0.075);
    storeText->setColor(Color3B::BLACK);
    storeText->setPosition(Point(startWidth + (backgroundWidth * 0.1), startHeight + (backgroundHeight * 0.935)));
    
    auto cherry = Sprite::create("cherry.png");
    cherry->setPosition(Point(startWidth + (backgroundWidth * 0.95), startHeight + (backgroundHeight * 0.935)));
    cherry->setScale(0.75);
    
    // Done Button
    auto done = MenuItemImage::create("menu_purchase/done2.png", "menu_purchase/done2.png", CC_CALLBACK_1(CherryStore::GoToGamePlay, this));
    done->setPosition(Point(startWidth + (backgroundWidth * 0.5), startHeight + (backgroundHeight * 0.06)));
    
    auto doneText = Label::createWithTTF( doneString->getCString(), "fonts/Arial_Regular.ttf", visibleSize.height * 0.04);
    doneText->setColor(Color3B::BLACK);
    doneText->setPosition(Point(done->getPositionX(), done->getPositionY()));
    
    // Purchase Buttons
    auto item1 = MenuItemImage::create("menu_purchase/sale_cherry.png", "menu_purchase/sale_cherry.png", CC_CALLBACK_1(CherryStore::Sale1, this));
    item1->setPosition(Point(startWidth + (backgroundWidth * 0.27), startHeight + (backgroundHeight * 0.77)));
    
    auto item2 = MenuItemImage::create("menu_purchase/sale_cherry.png", "menu_purchase/sale_cherry.png", CC_CALLBACK_1(CherryStore::Sale2, this));
    item2->setPosition(Point(startWidth + (backgroundWidth * 0.73), startHeight + (backgroundHeight * 0.77)));
    
    auto item3 = MenuItemImage::create("menu_purchase/sale_cherry.png", "menu_purchase/sale_cherry.png", CC_CALLBACK_1(CherryStore::Sale3, this));
    item3->setPosition(Point(startWidth + (backgroundWidth * 0.27), startHeight + (backgroundHeight * 0.58)));
    
    auto item4 = MenuItemImage::create("menu_purchase/sale_cherry.png", "menu_purchase/sale_cherry.png", CC_CALLBACK_1(CherryStore::Sale4, this));
    item4->setPosition(Point(startWidth + (backgroundWidth * 0.73), startHeight + (backgroundHeight * 0.58)));
    
    auto item5 = MenuItemImage::create("menu_purchase/sale_cherry.png", "menu_purchase/sale_cherry.png", CC_CALLBACK_1(CherryStore::Sale5, this));
    item5->setPosition(Point(startWidth + (backgroundWidth * 0.27), startHeight + (backgroundHeight * 0.39)));
    
    auto item6 = MenuItemImage::create("menu_purchase/sale_cherry.png", "menu_purchase/sale_cherry.png", CC_CALLBACK_1(CherryStore::Sale6, this));
    item6->setPosition(Point(startWidth + (backgroundWidth * 0.73), startHeight + (backgroundHeight * 0.39)));
    
    auto item7 = MenuItemImage::create("menu_purchase/sale_cherry.png", "menu_purchase/sale_cherry.png", CC_CALLBACK_1(CherryStore::Sale7, this));
    item7->setPosition(Point(startWidth + (backgroundWidth * 0.27), startHeight + (backgroundHeight * 0.2)));
    
    auto item8 = MenuItemImage::create("menu_purchase/sale_cherry.png", "menu_purchase/sale_cherry.png", CC_CALLBACK_1(CherryStore::Sale8, this));
    item8->setPosition(Point(startWidth + (backgroundWidth * 0.73), startHeight + (backgroundHeight * 0.2)));
    
    // Menu
    auto menu = Menu::create(item1, item2, item3, item4, item5, item6, item7, item8, done, NULL);
    menu->setPosition(Point::ZERO);
    
    this->addChild(black,1);
    this->addChild(background,2);
    this->addChild(storeText, 3);
    this->addChild(cherry, 3);
    this->addChild(doneText, 4);
    this->addChild(menu, 3);
    
    
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

void CherryStore::GoToGamePlay( cocos2d::Ref *sender )
{
    auto scene = GamePlay::createScene();
    Director::getInstance()->replaceScene(scene);
}

void CherryStore::Sale1( cocos2d::Ref *sender )
{
    
}

void CherryStore::Sale2( cocos2d::Ref *sender )
{
    
}

void CherryStore::Sale3( cocos2d::Ref *sender )
{
    
}

void CherryStore::Sale4( cocos2d::Ref *sender )
{
    
}

void CherryStore::Sale5( cocos2d::Ref *sender )
{
    
}

void CherryStore::Sale6( cocos2d::Ref *sender )
{
    
}

void CherryStore::Sale7( cocos2d::Ref *sender )
{
    
}

void CherryStore::Sale8( cocos2d::Ref *sender )
{
    
}
