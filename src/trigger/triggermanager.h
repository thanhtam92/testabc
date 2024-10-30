#pragma once

#include <string>
#include <mutex>
#include <list>

#include <Poco/Notification.h>
#include <Poco/NotificationQueue.h>
#include <Poco/Thread.h>
#include <Poco/Runnable.h>

#include "common/log.h"
#include "common/util.h"
#include "trigger.h"

namespace onevu {

class TriggerManager {

public:

    static TriggerManager& instance();

    void dispatch(Poco::AutoPtr<Trigger> trigg);

    TriggerList_t getCurrentTriggers();

private:
    TriggerList_t _triggerList;
    std::mutex _triggerInsertMtx;
};

}