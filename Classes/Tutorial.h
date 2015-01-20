#ifndef __TUTORIAL_H__
#define __TUTORIAL_H__

#include "cocos2d.h"

class Tutorial : public cocos2d::Layer
{
public:
    virtual ~Tutorial();
    
    bool menuShow;
    cocos2d::Menu *menu;
    cocos2d::Menu *menu2;
    cocos2d::Sprite *menuSprite;
    cocos2d::Label *power1Text;
    cocos2d::Label *power2Text;
    cocos2d::Label *power3Text;
    cocos2d::Sprite *bird;
    cocos2d::Sprite *finger;
    cocos2d::Sprite *target;
    cocos2d::Sprite *arrow_left;
    cocos2d::Sprite *arrow_right;
    cocos2d::Label *playText;
    cocos2d::Label *instructions;
    cocos2d::Label *cherryScore;
    cocos2d::Sprite *redX3;
    cocos2d::Sprite *cherry;
    cocos2d::Sprite *cherryAnimation;
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(Tutorial);
    
private:
    void GoToGamePlay( cocos2d::Ref *sender );
    void HideShowMenu(cocos2d::Ref *sender);
    void BlueBirds(cocos2d::Ref *sender);
    void Lightning(cocos2d::Ref *sender);
    void Grow(cocos2d::Ref *sender);
    void ChangeBird();
    void ChangeBird2();
    void ArrowReset();
    void CallBird();
    void MissBird();
    void Miss();
    void Missed();
    void Cherries();
    void GetCherry();
    void CollectCherry();
    void CherryCollected();
    void Powerups();
    void PowerupsShow();
    void Finish();
    void None(cocos2d::Ref *sender);
    void FinishRedirect();
};

#endif // __TUTORIAL_H__
