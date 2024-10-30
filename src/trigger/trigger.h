#pragma once

#include <string>
#include <thread>
#include <list>

#include <Poco/Notification.h>
#include <Poco/NotificationQueue.h>
#include <Poco/AutoPtr.h>

#include "common/frame.h"
#include "common/log.h"
#include "common/util.h"

namespace onevu {

enum TriggerState {
    eTriggerNone,
    eTriggerStart,
    eTriggerStop,
    eTriggerTimeout
};

class Trigger : public Poco::Notification {
public:
    // type
    // state: 
    // timestamp
    Trigger();
    virtual ~Trigger() {}

    std::string name;
    uint64_t timestamp_ms;
    uint64_t duration_ms;
    int stream_id = -1; // 0, 1 or -1 for all streams

    // virtual std::string to_string() = 0;
};

using TriggerList_t = std::list<Poco::AutoPtr<Trigger>>;

class TriggerManager;

class TriggerGenerator {

public:
    void start();
    void stop();
    void dispatch(Poco::AutoPtr<Trigger> trigg);
protected:
    virtual void preStart();
    virtual void afterStop();
    virtual void process() = 0;
private:
    std::thread _threadTrigger;
    bool _isRunning = false;

};

}