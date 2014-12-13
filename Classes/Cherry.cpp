#include "Cherry.h"
#include "Math.h"

USING_NS_CC;

Cherry::Cherry( cocos2d::Layer *layer )
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto cherry = Sprite::create("cherry.png");
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
    touchListener->onTouchBegan = CC_CALLBACK_2( Cherry::onTouchBegan, this, layer );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, cherry);
}

bool Cherry::onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event, cocos2d::Layer *layer )
{
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    if (rect.containsPoint(locationInNode))
    {
        layer->removeChild(target);
        return true;
    }
    return false;
}
