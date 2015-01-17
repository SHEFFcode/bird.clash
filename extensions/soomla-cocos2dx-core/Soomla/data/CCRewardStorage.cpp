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

#include "CCRewardStorage.h"
#include "CCReward.h"
#include "CCSequenceReward.h"
#include "CCCoreService.h"

#define TAG "SOOMLA RewardStorage"

namespace soomla {
    static CCRewardStorage *s_SharedRewardStorage = NULL;

    CCRewardStorage *CCRewardStorage::getInstance() {
        if (!s_SharedRewardStorage) {
            s_SharedRewardStorage = new CCRewardStorage();
            s_SharedRewardStorage->retain();
        }

        return s_SharedRewardStorage;
    }

    int CCRewardStorage::getTimesGiven(CCReward *reward) {
        return CCCoreService::getInstance()->getTimesGiven(reward);
    }

    bool CCRewardStorage::isRewardGiven(CCReward *reward) {
        return getTimesGiven(reward) > 0;
    }

    void CCRewardStorage::setRewardStatus(CCReward *reward, bool give) {
        setRewardStatus(reward, give, true);
    }

    void CCRewardStorage::setRewardStatus(CCReward *reward, bool give, bool notify) {
        CCCoreService::getInstance()->setRewardStatus(reward, give, notify);
    }

    int CCRewardStorage::getLastSeqIdxGiven(CCSequenceReward *sequenceReward) {
        return CCCoreService::getInstance()->getLastSeqIdxGiven(sequenceReward);
    }

    void CCRewardStorage::setLastSeqIdxGiven(CCSequenceReward *sequenceReward, unsigned int idx) {
        CCCoreService::getInstance()->setLastSeqIdxGiven(sequenceReward, idx);
    }
}
