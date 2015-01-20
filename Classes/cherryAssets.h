//
//  cherryAssets.h
//  birdcrush
//
//  Created by Yuriy Shefer on 1/17/15.
//
//

#ifndef __birdcrush__cherryAssets__
#define __birdcrush__cherryAssets__

#include <stdio.h>
#include "CCStoreAssets.h"
#include "cocos2d.h"

#define CHERRY_CURRENCY_ITEM_ID "currency_cherry"
#define FOURHUNDRED_PACK_PRODUCT_ID "001"
#define ONETHOUSAND_PACK_PRODUCT_ID "002"
#define THREETHOUSAND_PACK_PRODUCT_ID "003"
#define EIGHTTHOUSAND_PACK_PRODUCT_ID "004"
#define NO_ADDS_LTVG_PRODUCT_ID "cocos2dx_no_ads"

class cherryAssets: public soomla::CCStoreAssets {
    
    cocos2d::Array *mCurrencies;
    cocos2d::Array *mGoods;
    cocos2d::Array *mCurrencyPacks;
    cocos2d::Array *mCategories;
    
public:
    cherryAssets():
    
    mCurrencies(NULL),
    mGoods(NULL),
    mCurrencyPacks(NULL),
    mCategories(NULL)
    {}
    
    virtual ~cherryAssets();
    
    static cherryAssets *create();
    bool init();
    
    virtual int getVersion();
    
    virtual cocos2d::CCArray *getCurrencies();
    
    virtual cocos2d::CCArray *getGoods();
    
    virtual cocos2d::CCArray *getCurrencyPacks();
    
    virtual cocos2d::CCArray *getCategories();
};


#endif /* defined(__birdcrush__cherryAssets__) */
