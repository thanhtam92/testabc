#include "detection/detection_worker.h"

namespace onevu {

DetectionWorker::UPtr DetectionWorker::create(LPDTag) {
    auto worker = std::make_unique<DetectionWorker>();
    // prepare parammeters for lpd
    DetectionParammeter initParam;
    // todo:
    if(!worker->_detection.init(initParam)){
        return nullptr;
    }
    return std::move(worker);
}

DetectionWorker::UPtr DetectionWorker::create(VDTag) {
    auto worker = std::make_unique<DetectionWorker>();
    // prepare parammeters for vd
    DetectionParammeter initParam;
    // todo:
    if(!worker->_detection.init(initParam)){
        return nullptr;
    }
    return std::move(worker);
}

void DetectionWorker::enqueue(Poco::AutoPtr<Poco::Notification> ntfFrame){
    _frameQueue.enqueueNotification(ntfFrame);
}

void DetectionWorker::stop() {
    this->_isRunning = false;
}

void DetectionWorker::run() {
    _isRunning = true;
    while (_isRunning)
    {
        Poco::Notification::Ptr ntf = _frameQueue.waitDequeueNotification(1000);
        if(ntf){
            _detection.process(ntf);
        }
    };
}

}