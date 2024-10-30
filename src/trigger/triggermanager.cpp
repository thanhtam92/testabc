#include "triggermanager.h"

namespace onevu {

constexpr static int TIME_DELAY_MS = 10;

TriggerManager& TriggerManager::instance() {
    static TriggerManager triggMgr;
    return triggMgr;
}


void TriggerManager::dispatch(Poco::AutoPtr<Trigger> trigg) {
    std::lock_guard<std::mutex> lock(_triggerInsertMtx);
    _triggerList.push_back(trigg);
}

TriggerList_t TriggerManager::getCurrentTriggers(){
    std::lock_guard<std::mutex> lock(_triggerInsertMtx);
    std::remove_if(_triggerList.begin(), _triggerList.end(), 
                        [](const Poco::AutoPtr<Trigger>& trigger){
                            return (trigger->timestamp_ms + trigger->duration_ms < milliseconds_since_epoch()) ? true : false;
                        });

    return _triggerList;
}

}