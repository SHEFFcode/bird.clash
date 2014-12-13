#include "Powerups.h"
#include "external/Box2d/Box2d.h"

USING_NS_CC;

Powerups::Powerups( cocos2d::Layer *layer, b2World *world )
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    menuShow = 1;
    menuSprite = Sprite::create("powerups/minus.png");
    auto plusMinus = MenuItemSprite::create(menuSprite, menuSprite, CC_CALLBACK_1(Powerups::HideShowMenu, this));
    plusMinus->setPosition(Vec2(plusMinus->getContentSize().width / 2, plusMinus->getContentSize().height / 2));
    
    // Menu
    auto menu = Menu::create(plusMinus, NULL);
    menu->setPosition(Point::ZERO);
    layer->addChild(menu);

    auto bluebird = MenuItemImage::create("powerups/blue.png", "powerups/blue.png", CC_CALLBACK_1(Powerups::BlueBirds, this));
    auto width = bluebird->getContentSize().width;
    auto height = bluebird->getContentSize().height / 2;
    bluebird->setPosition(Vec2(plusMinus->getContentSize().width + (width / 2), height));
    
    auto light = MenuItemImage::create("powerups/lightning.png", "powerups/lightning.png", CC_CALLBACK_1(Powerups::Lightning, this));
    light->setPosition(Vec2(bluebird->getPositionX() + width, height));
    
    auto grow = MenuItemImage::create("powerups/grow.png", "powerups/grow.png", CC_CALLBACK_1(Powerups::Grow, this));
    grow->setPosition(Vec2(light->getPositionX() + width, height));
    
    auto x2 = MenuItemImage::create("powerups/x2.png", "powerups/x2.png", CC_CALLBACK_1(Powerups::X2, this));
    x2->setPosition(Vec2(grow->getPositionX() + width, height));
    
    auto x3 = MenuItemImage::create("powerups/x3.png", "powerups/x3.png", CC_CALLBACK_1(Powerups::X3, this));
    x3->setPosition(Vec2(x2->getPositionX() + width, height));
    
    // Menu
    menu2 = Menu::create(bluebird, light, grow, x2, x3, NULL);
    menu2->setPosition(Point::ZERO);
    layer->addChild(menu2);
}

void Powerups::BlueBirds( cocos2d::Ref *sender )
{
    
}

void Powerups::Lightning( cocos2d::Ref *sender )
{
    
}

void Powerups::Grow( cocos2d::Ref *sender )
{
    
}

void Powerups::X2( cocos2d::Ref *sender )
{
    
}

void Powerups::X3( cocos2d::Ref *sender )
{
    
}

void Powerups::HideShowMenu( cocos2d::Ref *sender )
{
    if(!menuShow){
        menuShow = 1;
        menuSprite->setTexture("powerups/minus.png");
        menu2->setOpacity(255);
        menu2->setEnabled(true);
    } else {
        menuShow = 0;
        menuSprite->setTexture("powerups/plus.png");
        menu2->setOpacity(0);
        menu2->setEnabled(false);
    }
}








