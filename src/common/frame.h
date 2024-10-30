#pragma once

#include <memory>
#include <future>
#include <Poco/Notification.h>
#include <Poco/NotificationQueue.h>
#include <Poco/Event.h>
#include <Poco/SharedPtr.h>

namespace onevu {

struct Frame {

};

enum DetectionStatus {
    DETECTION_NOT_READY,
    DETECTION_NOT_FOUND,
    DETECTION_FOUND
};

struct DetectionInfoEvent {
    DetectionStatus status;
    Poco::Event detectEvtReady;
};

class FrameNotification : public Poco::Notification {

public:
    Poco::AutoPtr<Frame> frame;
    DetectionInfoEvent lpdInfEvent;
    DetectionInfoEvent vdInfoEvent;

    // trigger
    int isTriggered = false;
};

class FrameNotificationQueue {

public:
    using Ptr = std::shared_ptr<FrameNotificationQueue>;

    FrameNotificationQueue();
    ~FrameNotificationQueue();

    void setMaxSize(uint64_t queueSize);

    void enqueueNotification(Poco::Notification::Ptr pNotification);
    Poco::Notification* waitDequeueNotification();
    Poco::Notification* waitDequeueNotification(long milliseconds);
    Poco::Notification* dequeueNotification();

    void wakeUpAll();
    size_t size();
    void clear();

private:
    Poco::NotificationQueue _ntfQueue;
    uint64_t _maxSize = 1024;

};

}