/*
 * Copyright (C) 2012 Soomla Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __ExampleEventHandler__
#define __ExampleEventHandler__

#include <string>
#include "Cocos2dxStore.h"


#define EVENT_ON_CURRENCY_BALANCE_CHANGED "onCurrencyBalanceChanged"
#define EVENT_ON_GOOD_BALANCE_CHANGED "onGoodBalanceChanged"
#define EVENT_ON_GOOD_EQUIPPED "onGoodEquipped"
#define EVENT_ON_GOOD_UNEQUIPPED "onGoodUnEquipped"
#define EVENT_ON_GOOD_UPGRADE "onGoodUpgrade"

using namespace std;

class ExampleEventHandler : public soomla::CCStoreEventHandler {
public:

    virtual void onBillingNotSupported();

    virtual void onBillingSupported();

    virtual void onCurrencyBalanceChanged(soomla::CCVirtualCurrency *virtualCurrency, int balance, int amountAdded);

    virtual void onGoodBalanceChanged(soomla::CCVirtualGood *virtualGood, int balance, int amountAdded);

    virtual void onGoodEquipped(soomla::CCEquippableVG *equippableVG);

    virtual void onGoodUnEquipped(soomla::CCEquippableVG *equippableVG);

    virtual void onGoodUpgrade(soomla::CCVirtualGood *virtualGood, soomla::CCUpgradeVG *upgradeVG);

    virtual void onItemPurchased(soomla::CCPurchasableVirtualItem *purchasableVirtualItem, cocos2d::__String *payload);

    virtual void onItemPurchaseStarted(soomla::CCPurchasableVirtualItem *purchasableVirtualItem);

    virtual void onMarketPurchaseCancelled(soomla::CCPurchasableVirtualItem *purchasableVirtualItem);

    virtual void onMarketPurchase(soomla::CCPurchasableVirtualItem *purchasableVirtualItem, cocos2d::__String *token, cocos2d::__String *payload);

    virtual void onMarketPurchaseStarted(soomla::CCPurchasableVirtualItem *purchasableVirtualItem);

    virtual void onMarketPurchaseVerification(soomla::CCPurchasableVirtualItem *purchasableVirtualItem);

    virtual void onRestoreTransactionsStarted();

    virtual void onRestoreTransactionsFinished(bool success);

    virtual void onUnexpectedErrorInStore(cocos2d::__String *errorMessage);

    virtual void onSoomlaStoreInitialized();

    virtual void onMarketItemsRefreshed(cocos2d::__Array *virtualItems);

    virtual void onMarketItemsRefreshStarted();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    virtual void onMarketRefund(soomla::CCPurchasableVirtualItem *purchasableVirtualItem);
	virtual void onIabServiceStarted();
	virtual void onIabServiceStopped();
#endif
};

#endif /* !__ExampleEventHandler__ */
