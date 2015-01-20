#ifndef __POWERUPS_H__
#define __POWERUPS_H__

#include "cocos2d.h"
#include "external/Box2d/Box2d.h"

class Powerups
{
public:
    virtual ~Powerups();
    
    bool menuShow;
    unsigned int power1;
    unsigned int power2;
    unsigned int power3;
    unsigned int soundOn;
    cocos2d::Sprite *menuSprite;
    cocos2d::Sprite *power1_infinity;
    cocos2d::Sprite *power2_infinity;
    cocos2d::Sprite *power3_infinity;
    cocos2d::Sprite *growSelected;
    cocos2d::Sprite *growSprite;
    cocos2d::Menu *menu;
    cocos2d::Menu *menu2;
    cocos2d::Label *power1Text;
    cocos2d::Label *power2Text;
    cocos2d::Label *power3Text;
    
    Powerups( cocos2d::Layer *layer, b2World *world);
    
private:
    void BlueBirds(cocos2d::Ref *sender, cocos2d::Layer *layer, b2World *world);
    void Lightning(cocos2d::Ref *sender, cocos2d::Layer *layer, b2World *world);
    void Grow(cocos2d::Ref *sender, cocos2d::Layer *layer, b2World *world);
    void HideShowMenu(cocos2d::Ref *sender);
    void ShowText();
    void HideText();
    
};

#endif // __POWERUPS_H__