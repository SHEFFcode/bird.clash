#ifndef __CHERRY_STORE_H__
#define __CHERRY_STORE_H__

#include "cocos2d.h"

class CherryStore : public cocos2d::Layer
{
public:
    virtual ~CherryStore();
    
    float startWidth;
    float startHeight;
    float backgroundWidth;
    float backgroundHeight;
    unsigned int cherries;
    unsigned int inSale = 0;
    unsigned int powerNum1;
    unsigned int powerNum2;
    unsigned int powerNum3;
    cocos2d::Sprite *cover;
    cocos2d::Sprite *notice;
    cocos2d::Sprite *background;
    cocos2d::Sprite *item1;
    cocos2d::Sprite *item2;
    cocos2d::Sprite *item3;
    cocos2d::Sprite *item4;
    cocos2d::Sprite *cherry1;
    cocos2d::Sprite *cherry2;
    cocos2d::Sprite *cherry3;
    cocos2d::Sprite *cherry4;
    cocos2d::Sprite *power1_infinity;
    cocos2d::Sprite *power2_infinity;
    cocos2d::Sprite *power3_infinity;
    cocos2d::Label *power1Text;
    cocos2d::Label *power2Text;
    cocos2d::Label *power3Text;
    cocos2d::Label *cherryText;
    cocos2d::Label *noticeText;
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene( unsigned int power );
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(CherryStore);
    
private:
    void GoToGamePlay( cocos2d::Ref *sender );
    void GoToCherryStore( cocos2d::Ref *sender, unsigned int num);
    void Blue();
    void Light();
    void Grow();
    void Sale( cocos2d::Ref *sender, unsigned int num );
    void RemoveNotice();
    bool onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event );
};

#endif // __CHERRY_STORE_H__
