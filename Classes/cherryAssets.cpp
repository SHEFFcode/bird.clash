#include "cherryAssets.h"

#include "CCVirtualCurrency.h"
#include "CCVirtualCurrencyPack.h"
#include "CCVirtualGood.h"
#include "CCSingleUseVG.h"
#include "CCPurchaseWithMarket.h"
#include "CCPurchaseWithVirtualItem.h"
#include "CCVirtualCategory.h"
#include "CCUpgradeVG.h"
#include "CCSingleUsePackVG.h"
#include "CCEquippableVG.h"

USING_NS_CC;
using namespace soomla;

cherryAssets *cherryAssets::create() {
    cherryAssets *ret = new cherryAssets();
    ret->autorelease();
    ret->init();
    
    return ret;
}

bool cherryAssets::init() {
    /** Virtual Currencies **/
    CCVirtualCurrency *cherryCurrency = CCVirtualCurrency::create(
                                                                  CCString::create("Cherries"),
                                                                  CCString::create(""),
                                                                  CCString::create(CHERRY_CURRENCY_ITEM_ID)
                                                                  );
    
    
    /** Virtual Currency Packs **/
    
    CCVirtualCurrencyPack *fourHundredCherryPack = CCVirtualCurrencyPack::create(
                                                                       CCString::create("cherry_400"),                                   // name
                                                                       CCString::create("Adds 400 Cherries to use for Powerups in Bird Clash."),                       // description
                                                                       CCString::create("001"),                                   // item id
                                                                       CCInteger::create(400),												// number of currencies in the pack
                                                                       CCString::create(CHERRY_CURRENCY_ITEM_ID),                        // the currency associated with this pack
                                                                       CCPurchaseWithMarket::create(CCString::create(FOURHUNDRED_PACK_PRODUCT_ID), CCDouble::create(0.99))
                                                                       );
    
    CCVirtualCurrencyPack *oneThousandCherryPack = CCVirtualCurrencyPack::create(
                                                                         CCString::create("cherry_1000"),                                   // name
                                                                         CCString::create("Adds 1000 Cherries to use for Powerups in Bird Clash."),                 // description
                                                                         CCString::create("002"),                                   // item id
                                                                         CCInteger::create(1000),												// number of currencies in the pack
                                                                         CCString::create(CHERRY_CURRENCY_ITEM_ID),                        // the currency associated with this pack
                                                                         CCPurchaseWithMarket::create(CCString::create(ONETHOUSAND_PACK_PRODUCT_ID), CCDouble::create(1.99))
                                                                         );
    
    CCVirtualCurrencyPack *threeThousandCherryPack = CCVirtualCurrencyPack::create(
                                                                            CCString::create("cherry_3000"),                                   // name
                                                                            CCString::create("Adds 3000 Cherries to use for Powerups in Bird Clash."),                 // description
                                                                            CCString::create("003"),                                   // item id
                                                                            CCInteger::create(3000),												// number of currencies in the pack
                                                                            CCString::create(CHERRY_CURRENCY_ITEM_ID),                        // the currency associated with this pack
                                                                            CCPurchaseWithMarket::create(CCString::create(THREETHOUSAND_PACK_PRODUCT_ID), CCDouble::create(4.99))
                                                                            );
    
    CCVirtualCurrencyPack *eightThousandCherryPack = CCVirtualCurrencyPack::create(
                                                                            CCString::create("cherry_8000"),                                   // name
                                                                            CCString::create("Adds 8000 Cherries to use for Powerups in Bird Clash."),                 // description
                                                                            CCString::create("004"),                                   // item id
                                                                            CCInteger::create(8000),												// number of currencies in the pack
                                                                            CCString::create(CHERRY_CURRENCY_ITEM_ID),                        // the currency associated with this pack
                                                                            CCPurchaseWithMarket::create(CCString::create(EIGHTTHOUSAND_PACK_PRODUCT_ID), CCDouble::create(9.99))
                                                                            );
    
    mCurrencies = CCArray::create(cherryCurrency, NULL);
    mCurrencies->retain();
    
    mCurrencyPacks = CCArray::create(fourHundredCherryPack, oneThousandCherryPack, threeThousandCherryPack, eightThousandCherryPack, NULL);
    mCurrencyPacks->retain();
    
    return true;
}

cherryAssets::~cherryAssets() {
    CC_SAFE_RELEASE(mCurrencies);
    CC_SAFE_RELEASE(mGoods);
    CC_SAFE_RELEASE(mCurrencyPacks);
    CC_SAFE_RELEASE(mCategories);
}

int cherryAssets::getVersion() {
    return 0;
}

cocos2d::CCArray *cherryAssets::getCurrencies() {
    return mCurrencies;
}

cocos2d::CCArray *cherryAssets::getGoods() {
    return mGoods;
}

cocos2d::CCArray *cherryAssets::getCurrencyPacks() {
    return mCurrencyPacks;
}

cocos2d::CCArray *cherryAssets::getCategories() {
    return mCategories;
}