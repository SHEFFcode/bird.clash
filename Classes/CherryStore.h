#ifndef __CHERRY_STORE_H__
#define __CHERRY_STORE_H__

#include "cocos2d.h"

class CherryStore : public cocos2d::Layer
{
public:
    cocos2d::Sprite *background;
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(CherryStore);
    
private:
    void GoToGamePlay( cocos2d::Ref *sender );
    void Sale1( cocos2d::Ref *sender );
    void Sale2( cocos2d::Ref *sender );
    void Sale3( cocos2d::Ref *sender );
    void Sale4( cocos2d::Ref *sender );
    void Sale5( cocos2d::Ref *sender );
    void Sale6( cocos2d::Ref *sender );
    void Sale7( cocos2d::Ref *sender );
    void Sale8( cocos2d::Ref *sender );
    bool onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event );
};

#endif // __CHERRY_STORE_H__
