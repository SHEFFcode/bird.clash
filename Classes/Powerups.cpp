#include "Powerups.h"
#include "external/Box2d/Box2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Powerups::~Powerups(){
//    menu->release();
//    menu2->release();
//    menuSprite->release();
//    power1Text->release();
//    power2Text->release();
//    power3Text->release();
//    power1_infinity->release();
//    power2_infinity->release();
//    power3_infinity->release();
//    growSelected->release();
//    growSprite->release();
}

Powerups::Powerups( cocos2d::Layer *layer, b2World *world )
{
    UserDefault *def = UserDefault::getInstance();
    power1 = def->getIntegerForKey("power1",0);
    power2 = def->getIntegerForKey("power2",0);
    power3 = def->getIntegerForKey("power3",0);
    menuShow = def->getIntegerForKey("powerupShow",0);
    soundOn = def->getIntegerForKey("sound");
    
    menuSprite = Sprite::create("powerups/minus2.png");
    menuSprite->setScale(1.25);
    auto plusMinus = MenuItemSprite::create(menuSprite, menuSprite, CC_CALLBACK_1(Powerups::HideShowMenu, this));
    plusMinus->setPosition(Vec2((menuSprite->getContentSize().width) / 2, plusMinus->getContentSize().height / 2));
    
    menu = Menu::create(plusMinus, NULL);
    menu->setPosition(Point::ZERO);
    layer->addChild(menu,1);
    
    auto blueSprite = Sprite::create("powerups/blue.png");
    auto blueSelected = blueSprite;
    if(power1 > 0)
        blueSelected = Sprite::create("powerups/blue_selected.png");
    blueSelected->setScale(1.25);
    blueSprite->setScale(1.25);
    auto bluebird = MenuItemSprite::create(blueSprite, blueSelected, CC_CALLBACK_1(Powerups::BlueBirds, this, layer, world));
    auto width = blueSprite->getContentSize().width * 1.25;
    auto height = blueSprite->getContentSize().height / 2;
    bluebird->setPosition(Vec2(plusMinus->getPositionX() + width, height));
    
    auto lightSprite = Sprite::create("powerups/lightning.png");
    auto lightSelected = lightSprite;
    if(power2 > 0)
        lightSelected = Sprite::create("powerups/lightning_selected.png");
    lightSprite->setScale(1.25);
    lightSelected->setScale(1.25);
    auto light = MenuItemSprite::create(lightSprite, lightSelected, CC_CALLBACK_1(Powerups::Lightning, this, layer, world));
    light->setPosition(Vec2(bluebird->getPositionX() + width, height));
    
    growSprite = Sprite::create("powerups/grow.png");
    growSelected = growSprite;
    if(power3 > 0)
        growSelected = Sprite::create("powerups/grow_selected.png");
    growSprite->setScale(1.25);
    growSelected->setScale(1.25);
    auto grow = MenuItemSprite::create(growSprite, growSelected, CC_CALLBACK_1(Powerups::Grow, this, layer, world));
    grow->setPosition(Vec2(light->getPositionX() + width, height));
    
    __String *pText1 = __String::createWithFormat("%i",power1);
    __String *pText2 = __String::createWithFormat("%i",power2);
    __String *pText3 = __String::createWithFormat("%i",power3);
    
    auto wSet = (bluebird->getContentSize().width * 1.25) * 0.4;
    
    power1Text = Label::createWithTTF( pText1->getCString(), "Arial_Regular.ttf", wSet);
    power1Text->setColor(Color3B::BLACK);
    auto textHeight = width + (((bluebird->getContentSize().height * 1.25) - width - power1Text->getContentSize().height / 2) / 2);
    power1Text->setPosition(Vec2(width * 1.5 + (power1Text->getContentSize().width * 0.15), textHeight));
    
    power2Text = Label::createWithTTF( pText2->getCString(), "Arial_Regular.ttf", wSet);
    power2Text->setColor(Color3B::BLACK);
    power2Text->setPosition(Point(power1Text->getPositionX() + width, textHeight));
    
    power3Text = Label::createWithTTF( pText3->getCString(), "Arial_Regular.ttf", wSet);
    power3Text->setColor(Color3B::BLACK);
    power3Text->setPosition(Point(power2Text->getPositionX() + width, textHeight));
    
    layer->addChild(power1Text,2);
    layer->addChild(power2Text,2);
    layer->addChild(power3Text,2);
    
    menu2 = Menu::create(bluebird, light, grow, NULL);
    menu2->setPosition(Point::ZERO);
    layer->addChild(menu2,1);
    
    if(menuShow){
        menuSprite->setTexture("powerups/minus2.png");
        menu2->setOpacity(255);
        menu2->setEnabled(true);
    } else {
        menuSprite->setTexture("powerups/plus2.png");
        menu2->setOpacity(0);
        menu2->setEnabled(false);
    }
    def->flush();
    
    if(power1 > 99) {
        power1Text->setOpacity(0);
        power1_infinity = Sprite::create("powerups/infinity.png");
        power1_infinity->setScale(1.25);
        power1_infinity->setPosition(Vec2(power1Text->getPositionX(), power1Text->getPositionY()));
        layer->addChild(power1_infinity,2);
    }
    if(power2 > 99) {
        power2Text->setOpacity(0);
        power2_infinity = Sprite::create("powerups/infinity.png");
        power2_infinity->setScale(1.25);
        power2_infinity->setPosition(Vec2(power2Text->getPositionX(), power2Text->getPositionY()));
        layer->addChild(power2_infinity,2);
    }
    if(power3 > 99) {
        power3Text->setOpacity(0);
        power3_infinity = Sprite::create("powerups/infinity.png");
        power3_infinity->setScale(1.25);
        power3_infinity->setPosition(Vec2(power3Text->getPositionX(), power3Text->getPositionY()));
        layer->addChild(power3_infinity,2);
    }
    if(!menuShow){
        this->HideText();
    } else {
        this->ShowText();
    }
}

void Powerups::BlueBirds( cocos2d::Ref *sender, cocos2d::Layer *layer, b2World *world)
{
    if(power1 > 0)
    {
        UserDefault *def = UserDefault::getInstance();
        auto power1 = def->getIntegerForKey("power1");
        long whichBird;
        power1--;
        def->setIntegerForKey("power1", power1);
        __String *tmp = __String::createWithFormat("%i", power1);
        power1Text->setString(tmp->getCString());
        
        if(power1 == 99) {
            power1_infinity->setOpacity(0);
            power1Text->setOpacity(255);
        }
        
        auto tmpBirdCount = 0;
        
        for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
        {
            if(b->GetType() == b2_dynamicBody)
            {
                for (b2Fixture *f = b->GetFixtureList(); f; f=f->GetNext()) {
                    if (f->GetUserData() != NULL) {
                        whichBird = (long)f->GetUserData();
                        if(whichBird == 1)
                            def->setIntegerForKey("blue1", 1);
                        if(whichBird == 2)
                            def->setIntegerForKey("blue2", 1);
                        if(whichBird == 3)
                            def->setIntegerForKey("blue3", 1);
                    }
                }
                tmpBirdCount++;
                Sprite* sprite = (Sprite*)b->GetUserData();
                auto tag = sprite->getTag();
                if(tag == 1 || tag == 2 || tag == 4 || tag == 8) {
                    sprite->setTexture("blue_right.png");
                } else {
                    sprite->setTexture("blue_left.png");
                }
            }
        }
        
        Size visibleSize = Director::getInstance()->getVisibleSize();
        
        auto blueBig = Sprite::create("powerups/blue_big.png");
        auto ratio = visibleSize.height / blueBig->getContentSize().width;
        blueBig->setScale(ratio);
        blueBig->setOpacity(0);
        blueBig->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        layer->addChild(blueBig,5);
        auto reveal = FadeTo::create(0.5,150);
        auto hide = FadeTo::create(0.5,0);
        Sequence *seq = Sequence::create(reveal, DelayTime::create(0.25), hide, NULL);
        blueBig->runAction(seq);
        
        if(soundOn){
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/splash.wav");
        }
        
        def->flush();
    }
}

void Powerups::Lightning( cocos2d::Ref *sender, cocos2d::Layer *layer, b2World *world)
{
    if(power2 > 0)
    {
        UserDefault *def = UserDefault::getInstance();
        auto power2 = def->getIntegerForKey("power2");
        power2--;
        def->setIntegerForKey("power2", power2);
        __String *tmp = __String::createWithFormat("%i", power2);
        power2Text->setString(tmp->getCString());
        def->flush();
        
        if(power2 == 99) {
            power2_infinity->setOpacity(0);
            power2Text->setOpacity(255);
        }
        
        
        for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
        {
            if(b->GetType() == b2_dynamicBody)
            {
                Sprite* sprite = (Sprite*)b->GetUserData();
                sprite->setTag(0);
            }
        }
        
        Size visibleSize = Director::getInstance()->getVisibleSize();
        
        auto lightBig = Sprite::create("powerups/lightning_big.png");
        auto ratio = visibleSize.height / lightBig->getContentSize().width;
        lightBig->setScale(ratio);
        lightBig->setOpacity(0);
        lightBig->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        layer->addChild(lightBig,5);
        auto reveal = FadeTo::create(0.5,150);
        auto hide = FadeTo::create(0.5,0);
        Sequence *seq = Sequence::create(reveal, DelayTime::create(0.25), hide, NULL);
        lightBig->runAction(seq);
        
        if(soundOn){
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/thunder.wav");
        }
    }
}

void Powerups::Grow( cocos2d::Ref *sender, cocos2d::Layer *layer, b2World *world)
{
    if(power3 > 0)
    {
        UserDefault *def = UserDefault::getInstance();
        if(def->getIntegerForKey("power3_activated") == 1)
            return;
        auto power3 = def->getIntegerForKey("power3");
        power3--;
        def->setIntegerForKey("power3", power3);
        def->setIntegerForKey("power3_activated", 1);
        __String *tmp = __String::createWithFormat("%i", power3);
        power3Text->setString(tmp->getCString());
        def->flush();
        
        if(power3 == 99) {
            power3_infinity->setOpacity(0);
            power3Text->setOpacity(255);
        }
        
        growSprite->setTexture("powerups/grow_disabled.png");
        growSelected->setTexture("powerups/grow_disabled.png");
        
        for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
        {
            if(b->GetType() == b2_dynamicBody)
            {
                Sprite* sprite = (Sprite*)b->GetUserData();
                auto scale = sprite->getScale();
                sprite->setScale(scale * 1.25);
            }
        }

        Size visibleSize = Director::getInstance()->getVisibleSize();
        
        auto growBig = Sprite::create("powerups/grow_big.png");
        auto ratio = visibleSize.height / growBig->getContentSize().width;
        growBig->setScale(ratio);
        growBig->setOpacity(0);
        growBig->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        layer->addChild(growBig,5);
        auto reveal = FadeTo::create(0.5,150);
        auto hide = FadeTo::create(0.5,0);
        Sequence *seq = Sequence::create(reveal, DelayTime::create(0.25), hide, NULL);
        growBig->runAction(seq);
        
        if(soundOn){
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/grow.mp3");
        }
    }
}

void Powerups::HideShowMenu( cocos2d::Ref *sender )
{
    UserDefault *def = UserDefault::getInstance();
    if(!menuShow){
        menuShow = 1;
        menuSprite->setTexture("powerups/minus2.png");
        menu2->setOpacity(255);
        menu2->setEnabled(true);
        this->ShowText();
        def->setIntegerForKey("powerupShow", 1);
    } else {
        menuShow = 0;
        menuSprite->setTexture("powerups/plus2.png");
        menu2->setOpacity(0);
        menu2->setEnabled(false);
        this->HideText();
        def->setIntegerForKey("powerupShow", 0);
    }
    def->flush();
}

void Powerups::ShowText(){
    if(power1 > 99) {
        power1_infinity->setOpacity(255);
    } else {
        power1Text->setOpacity(255);
    }
    if(power2 > 99) {
        power2_infinity->setOpacity(255);
    } else {
        power2Text->setOpacity(255);
    }
    if(power3 > 99) {
        power3_infinity->setOpacity(255);
    } else {
        power3Text->setOpacity(255);
    }
}

void Powerups::HideText(){
    if(power1 > 99) {
        power1_infinity->setOpacity(0);
    } else {
        power1Text->setOpacity(0);
    }
    if(power2 > 99) {
        power2_infinity->setOpacity(0);
    } else {
        power2Text->setOpacity(0);
    }
    if(power3 > 99) {
        power3_infinity->setOpacity(0);
    } else {
        power3Text->setOpacity(0);
    }
}












