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

#include "ExampleEventHandler.h"
#include "cocos2d.h"

#define TAG "ExampleEventHandler >>>"

USING_NS_CC;

using namespace soomla;

void ExampleEventHandler::onBillingNotSupported() {
    CCSoomlaUtils::logDebug(TAG, "BillingNotSupported");
}

void ExampleEventHandler::onBillingSupported() {
    CCSoomlaUtils::logDebug(TAG, "BillingSupported");
}

void ExampleEventHandler::onCurrencyBalanceChanged(CCVirtualCurrency *virtualCurrency, int balance, int amountAdded) {
    CCSoomlaUtils::logDebug(TAG, "CurrencyBalanceChanged");
    __NotificationCenter::getInstance()->postNotification
        (EVENT_ON_CURRENCY_BALANCE_CHANGED, CCInteger::create(amountAdded));
}

void ExampleEventHandler::onGoodBalanceChanged(CCVirtualGood *virtualGood, int balance, int amountAdded) {
    CCSoomlaUtils::logDebug(TAG, "GoodBalanceChanged");
    __NotificationCenter::getInstance()->postNotification
        (EVENT_ON_GOOD_BALANCE_CHANGED, CCArray::create(virtualGood, CCInteger::create(balance), NULL));
}

void ExampleEventHandler::onGoodEquipped(CCEquippableVG *equippableVG) {
    CCSoomlaUtils::logDebug(TAG, "GoodEquipped");
    __NotificationCenter::getInstance()->postNotification(EVENT_ON_GOOD_EQUIPPED, equippableVG);
}

void ExampleEventHandler::onGoodUnEquipped(CCEquippableVG *equippableVG) {
    CCSoomlaUtils::logDebug(TAG, "GoodUnEquipped");
    __NotificationCenter::getInstance()->postNotification(EVENT_ON_GOOD_UNEQUIPPED, equippableVG);
}

void ExampleEventHandler::onGoodUpgrade(CCVirtualGood *virtualGood, CCUpgradeVG *upgradeVG) {
    CCSoomlaUtils::logDebug(TAG, "GoodUpgrade");
    __NotificationCenter::getInstance()->postNotification(EVENT_ON_GOOD_UPGRADE, virtualGood);
}

void ExampleEventHandler::onItemPurchased(CCPurchasableVirtualItem *purchasableVirtualItem, cocos2d::__String *payload) {
    CCSoomlaUtils::logDebug(TAG, "ItemPurchased");
}

void ExampleEventHandler::onItemPurchaseStarted(CCPurchasableVirtualItem *purchasableVirtualItem) {
    CCSoomlaUtils::logDebug(TAG, "ItemPurchaseStarted");
}

void ExampleEventHandler::onMarketPurchaseCancelled(CCPurchasableVirtualItem *purchasableVirtualItem) {
    CCSoomlaUtils::logDebug(TAG, "MarketPurchaseCancelled");
}

void ExampleEventHandler::onMarketPurchase(CCPurchasableVirtualItem *purchasableVirtualItem, cocos2d::__String *token, cocos2d::__String *payload) {
    CCSoomlaUtils::logDebug(TAG, "MarketPurchase");
}

void ExampleEventHandler::onMarketPurchaseStarted(CCPurchasableVirtualItem *purchasableVirtualItem) {
    CCSoomlaUtils::logDebug(TAG, "MarketPurchaseStarted");
}

void ExampleEventHandler::onMarketPurchaseVerification(CCPurchasableVirtualItem *purchasableVirtualItem) {
    CCSoomlaUtils::logDebug(TAG, "MarketPurchaseVerification");
}

void ExampleEventHandler::onRestoreTransactionsStarted() {
    CCSoomlaUtils::logDebug(TAG, "RestoreTransactionsStarted");
}

void ExampleEventHandler::onRestoreTransactionsFinished(bool success) {
    CCSoomlaUtils::logDebug(TAG, "RestoreTransactionsFinished");
}

void ExampleEventHandler::onUnexpectedErrorInStore(cocos2d::__String *errorMessage) {
    CCSoomlaUtils::logDebug(TAG, "UnexpectedErrorInStore");
}

void ExampleEventHandler::onSoomlaStoreInitialized() {
    CCSoomlaUtils::logDebug(TAG, "StoreContorllerInitialized");
}

void ExampleEventHandler::onMarketItemsRefreshed(cocos2d::__Array *virtualItems) {
    CCSoomlaUtils::logDebug(TAG, "onMarketItemsRefreshed");
}

void ExampleEventHandler::onMarketItemsRefreshStarted() {
    CCSoomlaUtils::logDebug(TAG, "MarketItemsRefreshStarted");
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

void ExampleEventHandler::onMarketRefund(CCPurchasableVirtualItem *purchasableVirtualItem) {
    CCSoomlaUtils::logDebug(TAG, "MarketRefund");
}

void ExampleEventHandler::onIabServiceStarted() {
    CCSoomlaUtils::logDebug(TAG, "IabServiceStarted");
}

void ExampleEventHandler::onIabServiceStopped() {
    CCSoomlaUtils::logDebug(TAG, "IabServiceStopped");
}
#endif
