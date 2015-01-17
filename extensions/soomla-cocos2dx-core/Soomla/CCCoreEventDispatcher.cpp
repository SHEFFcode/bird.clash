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

#include "CCCoreEventDispatcher.h"
#include "CCDomainFactory.h"
#include "CCSoomlaEventDispatcher.h"

namespace soomla {

    USING_NS_CC;

    static CCCoreEventDispatcher *s_SharedInstance = NULL;

    CCCoreEventDispatcher *CCCoreEventDispatcher::getInstance() {
        if (!s_SharedInstance) {
            s_SharedInstance = new CCCoreEventDispatcher();
            s_SharedInstance->init();
        }

        return s_SharedInstance;
    }

    bool CCCoreEventDispatcher::init() {

        if (!CCAbstractAggregatedEventHandler::init()) {
            return false;
        }
        
        CCSoomlaEventDispatcher *eventDispatcher = CCSoomlaEventDispatcher::getInstance();

        eventDispatcher->registerEventHandler(CCCoreConsts::EVENT_REWARD_GIVEN,
                [this](__Dictionary *parameters) {
                    __String *rewardId = dynamic_cast<__String *>(parameters->objectForKey("reward"));
                    CC_ASSERT(rewardId);
                    CCReward *reward  = CCReward::getReward(rewardId);
                    CC_ASSERT(reward);
                    this->onRewardGivenEvent(reward);
                });
        eventDispatcher->registerEventHandler(CCCoreConsts::EVENT_REWARD_TAKEN,
                [this](__Dictionary *parameters) {
                    __String *rewardId = dynamic_cast<__String *>(parameters->objectForKey("reward"));
                    CC_ASSERT(rewardId);
                    CCReward *reward  = CCReward::getReward(rewardId);
                    CC_ASSERT(reward);
                    this->onRewardTakenEvent(reward);
                });
        eventDispatcher->registerEventHandler(CCCoreConsts::EVENT_CUSTOM,
              [this](__Dictionary *parameters) {
                  __String *name = dynamic_cast<__String *>(parameters->objectForKey("name"));
                  CC_ASSERT(name);
                  __Dictionary *extra = dynamic_cast<__Dictionary *>(parameters->objectForKey("extra"));
                  CC_ASSERT(extra);
                  this->onCustomEvent(name, extra);
              });

        return true;
    }

    void CCCoreEventDispatcher::onRewardGivenEvent(CCReward *reward) {
        FOR_EACH_EVENT_HANDLER(CCCoreEventHandler)
            eventHandler->onRewardGivenEvent(reward);
        }
    }

    void CCCoreEventDispatcher::onRewardTakenEvent(CCReward *reward) {
        FOR_EACH_EVENT_HANDLER(CCCoreEventHandler)
            eventHandler->onRewardTakenEvent(reward);
        }
    }

    void CCCoreEventDispatcher::onCustomEvent(cocos2d::__String *name, cocos2d::__Dictionary *extra) {
        FOR_EACH_EVENT_HANDLER(CCCoreEventHandler)
            eventHandler->onCustomEvent(name, extra);
        }
    }
}
