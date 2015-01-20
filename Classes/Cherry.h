#ifndef __CHERRY_H__
#define __CHERRY_H__

#include "cocos2d.h"

class Cherry
{
public:
    virtual ~Cherry();
    
    unsigned int soundOn;
    cocos2d::Sprite *cherry;
    
    Cherry( cocos2d::Layer *layer, cocos2d::Label *label);
    
private:
    bool onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event, cocos2d::Layer *layer, cocos2d::Label *label );
    void RemoveCherry( cocos2d::Sprite *sprite, cocos2d::Layer *layer );

};

#endif // __CHERRY_H__