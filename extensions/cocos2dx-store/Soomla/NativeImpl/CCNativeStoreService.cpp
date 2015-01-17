/*
 Copyright (C) 2012-2014 Soomla Inc.
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "CCNativeStoreService.h"
#include "CCNdkBridge.h"
#include "CCSoomlaUtils.h"

namespace soomla {
    
    USING_NS_CC;
    
#define TAG "SOOMLA CCNativeStoreService"

    bool CCNativeStoreService::init(CCStoreAssets *storeAssets, __Dictionary *storeParams) {
        bool result = CCStoreService::init(storeAssets, storeParams);
        
        if (!result) {
            return false;
        }
        
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        __Bool *SSV = dynamic_cast<__Bool *>(storeParams->objectForKey("SSV"));
        if (SSV == NULL) {
            SSV = __Bool::create(false);
        }
        
        {
            __Dictionary *params = __Dictionary::create();
            params->setObject(__String::create("CCSoomlaStore::setSSV"), "method");
            params->setObject(SSV, "ssv");
            CCNdkBridge::callNative (params, NULL);
        }
    #endif
        
        {
            __Dictionary *params = __Dictionary::create();
            params->setObject(__String::create("CCStoreService::init"), "method");
            CCError *error = NULL;
            CCNdkBridge::callNative (params, &error);
            
            if (error) {
                CCSoomlaUtils::logError(TAG, error->getInfo());
                return false;
            }
            
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            {
                __String *androidPublicKey = dynamic_cast<__String *>(storeParams->objectForKey("androidPublicKey"));
                if (androidPublicKey != NULL && androidPublicKey->length()>0) {
                    __Dictionary *params = __Dictionary::create();
                    params->setObject(__String::create("CCSoomlaStore::setAndroidPublicKey"), "method");
                    params->setObject(androidPublicKey, "androidPublicKey");
                    CCNdkBridge::callNative (params, NULL);
                }
                
            }
            
            {
                __Bool *testPurchases = dynamic_cast<__Bool *>(storeParams->objectForKey("testPurchases"));
                if (testPurchases == NULL) {
                    testPurchases = __Bool::create(false);
                }
                __Dictionary *params = __Dictionary::create();
                params->setObject(__String::create("CCSoomlaStore::setTestPurchases"), "method");
                params->setObject(testPurchases, "testPurchases");
                CCNdkBridge::callNative (params, NULL);
            }
    #endif
        }
        
        return true;
    }
}