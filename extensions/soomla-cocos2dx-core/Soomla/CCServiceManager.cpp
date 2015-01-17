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

#include "CCServiceManager.h"
#include "CCNdkBridge.h"

USING_NS_CC;

namespace soomla {
    static CCServiceManager *sInstance = nullptr;

    CCServiceManager *CCServiceManager::getInstance() {
        if (!sInstance)
        {
            sInstance = new CCServiceManager();
            sInstance->init();
            sInstance->retain();
        }
        return sInstance;
    }

    bool CCServiceManager::init() {
        return true;
    }

    void CCServiceManager::setCommonParams(cocos2d::__Dictionary *commonParams) {
        __String *customSecret = dynamic_cast<__String *>(commonParams->objectForKey("customSecret"));
        if (customSecret == NULL || customSecret->length()==0) {
            __String *message = __String::createWithFormat("Can't initialize CCStoreService without customSecret");
            CCLOGERROR("%s %s", "CCServiceManager", message->getCString());

            return;
        }

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCServiceManager::setCommonParams"), "method");
        params->setObject(commonParams, "params");
        CCNdkBridge::callNative(params, NULL);
        
        CCCoreService::initShared();
    }
}
