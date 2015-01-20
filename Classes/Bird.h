#ifndef __BIRD_H__
#define __BIRD_H__

#include "cocos2d.h"
#include "external/Box2d/Box2d.h"

class Bird
{
public:
    virtual ~Bird();
    
    cocos2d::Sprite *bird;
    
    Bird( cocos2d::Layer *layer, b2World *world );
    
    void UpdateBird( b2FixtureDef* fixture );
    
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::MoveBy* action;
    
    bool onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event );
};

#endif // __PLAYER_H__