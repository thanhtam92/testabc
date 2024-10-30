#pragma once

#include "common/frame.h"
#include "detection/detection_worker.h"

#include <Poco/Thread.h>

namespace onevu {

class DetectionDispatcher {

public:
    static DetectionDispatcher& instance();

    bool startWorkers();
    void stopWorkers();

    void dispatch(Poco::AutoPtr<Poco::Notification> ntfFrame, LPDTag);

    void dispatch(Poco::AutoPtr<Poco::Notification> ntfFrame, VDTag);

private:
    Poco::Thread _lpdThread;
    Poco::Thread _vdThread;
    DetectionWorker::UPtr _lpdworker;
    DetectionWorker::UPtr _vdworker;
};

}