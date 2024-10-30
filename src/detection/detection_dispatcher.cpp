#include "detection/detection_dispatcher.h"

namespace onevu {

DetectionDispatcher& DetectionDispatcher::instance() {
    static DetectionDispatcher dispatcher;
    return dispatcher;
}

bool DetectionDispatcher::startWorkers() {
    // create lpd worker and start it
    _lpdworker = DetectionWorker::create(lpd_tag);
    if(!_lpdworker){
        return false;
    }
    
    // create vd worker and start it
    _vdworker = DetectionWorker::create(vd_tag);
    if(!_vdworker){
        return false;
    }

    _lpdThread.start(*_lpdworker);
    _vdThread.start(*_vdworker);
    return true;
}

void DetectionDispatcher::stopWorkers() {
    if(_lpdworker){
        _lpdworker->stop();
        _lpdThread.tryJoin(5000);
        _lpdworker.release();
    }

    if(_vdworker){
        _vdworker->stop();
        _vdThread.tryJoin(5000);
        _vdworker.release();
    }
}

void DetectionDispatcher::dispatch(Poco::AutoPtr<Poco::Notification> ntfFrame, LPDTag) {
    if(_lpdworker){
        _lpdworker->enqueue(ntfFrame);
    }
}

void DetectionDispatcher::dispatch(Poco::AutoPtr<Poco::Notification> ntfFrame, VDTag) {
    if(_vdworker){
        _vdworker->enqueue(ntfFrame);
    }
}

}