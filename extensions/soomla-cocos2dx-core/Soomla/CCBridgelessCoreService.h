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

#ifndef __CCBridgelessCoreService_H_
#define __CCBridgelessCoreService_H_

#include "CCCoreService.h"
#include "cocos2d.h"

namespace soomla {
    
    class CCBridgelessCoreService: public CCCoreService {
    public:
        CCBridgelessCoreService();
        
        int getTimesGiven(CCReward *reward);
        void setRewardStatus(CCReward *reward, bool give, bool notify);
        int getLastSeqIdxGiven(soomla::CCSequenceReward *sequenceReward);
        void setLastSeqIdxGiven(CCSequenceReward *sequenceReward, unsigned int idx);
        
        const char *kvStorageGetValue(const char *key) const;
        void kvStorageSetValue(const char *key, const char *val);
        void kvStorageDeleteKeyValue(const char *key);
        void kvStoragePurge();
    protected:
        cocos2d::__Set *mStoredKeys;
        void addStoredKeys(const char* key);
        void removeStoredKeys(const char *key);
        void saveStoredKeys();
        void loadStoredKeys();
    private:
        void setTimesGiven(CCReward *reward, bool up, bool notify);
        const char* keyRewards(const char* rewardId, const char* postfix);
        const char* keyRewardIdxSeqGiven(const char* rewardId);
        const char* keyRewardTimesGiven(const char* rewardId);
        const char* keyRewardLastGiven(const char* rewardId);
    };
}

#endif /* __CCBridgelessCoreService_H_ */
