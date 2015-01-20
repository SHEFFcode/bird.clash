#ifndef __STORE_H__
#define __STORE_H__

#include "cocos2d.h"

class Store : public cocos2d::Layer
{
protected:
    virtual void onEnter();
    virtual void onExit();
public:
    virtual ~Store();
    
    unsigned int cherries;
    cocos2d::Sprite *background;
    cocos2d::Label *cherryText;
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(Store);
    
private:
    virtual void updateCurrencyBalance(Ref *pBalance);
    void GoToGamePlay( cocos2d::Ref *sender );
    void Sale1( cocos2d::Ref *sender );
    void Sale2( cocos2d::Ref *sender );
    void Sale3( cocos2d::Ref *sender );
    void Sale4( cocos2d::Ref *sender );
    bool onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event );
};

#endif // __STORE_H__
