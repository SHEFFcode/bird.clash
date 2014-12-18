#ifndef __CHERRY_H__
#define __CHERRY_H__

#include "cocos2d.h"

class Cherry
{
public:
    unsigned int soundOn;
    cocos2d::Sprite *cherry;
    
    Cherry( cocos2d::Layer *layer);
    
private:
    bool onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event, cocos2d::Layer *layer );
    void RemoveCherry( cocos2d::Sprite *sprite, cocos2d::Layer *layer );

};

#endif // __CHERRY_H__