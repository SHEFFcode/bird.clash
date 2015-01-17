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

#ifndef __CCCoreService_H_
#define __CCCoreService_H_

#include "cocos2d.h"
#include "CCError.h"
#include "CCReward.h"
#include "CCSequenceReward.h"

namespace soomla {

    class CCCoreService: public cocos2d::Ref {
    public:
		/**
		   This class is singleton, use this function to access it.
		*/
        static CCCoreService* getInstance();

        static void initShared();

        CCCoreService();

        virtual bool init();

        virtual int getTimesGiven(CCReward *reward) = 0;
        virtual void setRewardStatus(CCReward *reward, bool give, bool notify) = 0;
        virtual int getLastSeqIdxGiven(soomla::CCSequenceReward *sequenceReward) = 0;
        virtual void setLastSeqIdxGiven(CCSequenceReward *sequenceReward, unsigned int idx) = 0;

        virtual const char *kvStorageGetValue(const char *key) const = 0;
        virtual void kvStorageSetValue(const char *key, const char *val) = 0;
        virtual void kvStorageDeleteKeyValue(const char *key) = 0;
        virtual void kvStoragePurge() = 0;
    };
};

#endif // !__CCCoreService_H_
