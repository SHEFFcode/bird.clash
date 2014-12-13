#ifndef __POWERUPS_H__
#define __POWERUPS_H__

#include "cocos2d.h"
#include "external/Box2d/Box2d.h"

class Powerups
{
public:
    bool menuShow;
    cocos2d::Sprite *menuSprite;
    cocos2d::Menu *menu2;
    
    Powerups( cocos2d::Layer *layer, b2World *world);
    
private:
    void BlueBirds(cocos2d::Ref *sender);
    void Lightning(cocos2d::Ref *sender);
    void Grow(cocos2d::Ref *sender);
    void X2(cocos2d::Ref *sender);
    void X3(cocos2d::Ref *sender);
    void HideShowMenu(cocos2d::Ref *sender);
};

#endif // __POWERUPS_H__