#ifndef __GAME_OVER_H__
#define __GAME_OVER_H__

#include "cocos2d.h"

class GameOver : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    std::string sceenshot(std::string& filename);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameOver);
    
private:
    void GoToGamePlay( cocos2d::Ref *sender );
    void Replay( cocos2d::Ref *sender );
    void Share( cocos2d::Ref *sender );
    void Rate( cocos2d::Ref *sender );
};

#endif // __GAME_OVER_H__
