#include "Cherry.h"
#include "Math.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Cherry::~Cherry(){
//    cherry->release();
}

Cherry::Cherry( cocos2d::Layer *layer, cocos2d::Label *cherryScore)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    UserDefault *def = UserDefault::getInstance();
    soundOn = def->getIntegerForKey("sound");
    def->flush();

    auto cherry = Sprite::create("cherry.png");
    cherry->setScale(1.25);
    cherry->setTag(999);
    unsigned int widthStart = visibleSize.width * 0.2;
    unsigned int widthEnd = visibleSize.width * 0.6;
    unsigned int heightEnd = visibleSize.height * 0.6;
    unsigned int heightStart = visibleSize.height * 0.2;
    srand((unsigned)time(NULL));
    auto width = rand() % widthEnd + widthStart;
    auto height = rand() % heightEnd + heightStart;
    cherry->setPosition(Vec2(width, height));
    layer->addChild(cherry,50);
    
    //setup touch listener
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches( true );
    touchListener->onTouchBegan = CC_CALLBACK_2( Cherry::onTouchBegan, this, layer, cherryScore);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, cherry);
}

bool Cherry::onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event, cocos2d::Layer *layer, cocos2d::Label *cherryScore)
{
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    if (rect.containsPoint(locationInNode))
    {
        UserDefault *def = UserDefault::getInstance();
        auto currency = def->getIntegerForKey("cherry_currency");
        currency++;
        def->setIntegerForKey("cherry_currency", currency);
        __String *tempScore = __String::createWithFormat("%i", currency);
        cherryScore->setString(tempScore->getCString());
        def->flush();
        auto cherryAnimation = Sprite::create("cherry_animation.png");
        cherryAnimation->setPosition(Vec2(target->getPositionX(), target->getPositionY()));
        cherryAnimation->setScale(0.35);
        layer->addChild(cherryAnimation,49);
        layer->removeChild(target);
        auto animation = ScaleTo::create(0.1, .75);
        cherryAnimation->runAction(animation);
        Sequence *seq = Sequence::create(DelayTime::create(0.1), CallFunc::create(std::bind(&Cherry::RemoveCherry, this, cherryAnimation, layer)), NULL);
        layer->runAction(seq);
        if(soundOn){
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/cherry.wav");
        }
        return true;
    }
    return false;
}

void Cherry::RemoveCherry( cocos2d::Sprite *sprite, cocos2d::Layer *layer )
{
    layer->removeChild(sprite);
}
