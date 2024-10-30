#pragma once

#include "common/frame.h"
#include "detection/detection.h"
#include <Poco/Runnable.h>

namespace onevu {

struct LPDTag {};
struct VDTag {};

static LPDTag lpd_tag;
static VDTag vd_tag;

class DetectionWorker : public Poco::Runnable {

public:
    using UPtr = std::unique_ptr<DetectionWorker>;

    static DetectionWorker::UPtr create(LPDTag);
    static DetectionWorker::UPtr create(VDTag);

    void stop();

    void enqueue(Poco::AutoPtr<Poco::Notification> ntfFrame);

private:
    void run() override;

    bool _isRunning = false;

    DetectionWrapper _detection;
    FrameNotificationQueue _frameQueue;
};

}