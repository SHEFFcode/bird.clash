#ifndef __GAME_PLAY_H__
#define __GAME_PLAY_H__

#include "cocos2d.h"
#include "Bird.h"
#include "external/Box2d/Box2d.h"
#include "MyContactListener.h"
#include "Cherry.h"
#include "Powerups.h"
#include "Store.h"
#include "CherryStore.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "ProtocolAds.h"
#endif


#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
class MyAdsListener : public cocos2d::plugin::AdsListener
{
public:
    virtual void onAdsResult(cocos2d::plugin::AdsResultCode code, const char* msg){}
    virtual void onPlayerGetPoints(cocos2d::plugin::ProtocolAds* pAdsPlugin, int points){}
};
#endif

class GamePlay : public cocos2d::Layer
{
public:
    virtual ~GamePlay();
    
    b2World* world;
    cocos2d::Sprite* soundSprite;
    cocos2d::Sprite* cherrySprite;
    cocos2d::Label* cherryScore;
    
    bool redCheck;
    bool redCheck2;
    bool redCheck3;
    bool cherryCreated;
    
    Bird *bird1;
    Bird *bird2;
    Bird *bird3;
    
    Cherry *cherry;
    Powerups *powerups;
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene( unsigned int startGame );
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GamePlay);
    
    // a selector callback
    void menuBackCallback(Ref* pSender);
    void testShow(Ref* pSender);
    void testHide(Ref* pSender);
    
    // option changed callback
    void caseChanged(Ref* pSender);
    void posChanged(Ref* pSender);
    void typeChanged(Ref* pSender);

    
private:
    unsigned int score = 0;
    
    MyContactListener *_contactListener;
    
    cocos2d::PhysicsWorld *sceneWorld;
    b2Body *bottom;
    b2Body *top;
    b2Body *right;
    b2Body *left;
    cocos2d::Label* playText;
    cocos2d::Label* gameText;
    cocos2d::Menu* menu;
    cocos2d::Sprite *redX;
    cocos2d::Sprite *redX2;
    cocos2d::Sprite *redX3;
    
    void BeginContact(b2Contact* contact);
    
    void SetPhysicsWorld( cocos2d::PhysicsWorld *world ){ sceneWorld = world; };
    void StartGame( cocos2d::Ref *sender );
    void AddBird( float dt );
    void AddBird2( float dt );
    void AddBird3( float dt );
    void ResetBirds( float dt );
    void update ( float dt );
    void AddCherry ( float dt );
    void RemoveCherry ( float dt );
    void GameOver( float dt );
    void Tutorial(cocos2d::Ref *sender);
    void Sound(cocos2d::Ref *sender);
    void GoToStore(cocos2d::Ref *sender);
    void UseCherries(cocos2d::Ref *sender);
    void PlayQuack();
    void RemoveBird( cocos2d::Sprite *sprite );
    void RemoveText( cocos2d::Label *label );

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    //AdMob
    cocos2d::plugin::ProtocolAds* _admob;
    cocos2d::plugin::ProtocolAds* _flurryAds;
    MyAdsListener* _listener;
    
    cocos2d::MenuItemToggle* _caseItem;
    cocos2d::MenuItemToggle* _posItem;
    
    cocos2d::plugin::ProtocolAds* _ads;
    cocos2d::plugin::ProtocolAds::AdsPos _pos;
    
    cocos2d::plugin::TAdsInfo adInfo;
#endif
};



#endif // __GAME_PLAY_H__
