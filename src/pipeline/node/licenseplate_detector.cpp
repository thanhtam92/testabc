#include "pipeline/node/licenseplate_detector.h"
#include "detection/detection_dispatcher.h"

namespace onevu {

namespace node {

LicensePlateDetector::LicensePlateDetector() 
    : LicensePlateDetector(std::make_unique<LicensePlateDetectorParameter>()){

}

LicensePlateDetector::LicensePlateDetector(std::unique_ptr<LicensePlateDetectorParameter> params)
 : NodeCRTP<Node, LicensePlateDetector, LicensePlateDetectorParameter>(std::move(params)) {

}

LicensePlateDetector::~LicensePlateDetector() {

}

void LicensePlateDetector::process() {
    if(queueIn0 == nullptr) {
        LOG_ERROR("input queue is nullptr\n");
        return;
    };

    if(queueOut0 == nullptr) {
        LOG_ERROR("output queue is nullptr");
        return;
    }

    if(queueIn0->size() == 0){
        return;
    }

    // get a frame from queue
    Poco::Notification::Ptr ntf = queueIn0->dequeueNotification();
    if(ntf){
        auto frameNtf = dynamic_cast<FrameNotification*>(ntf.get());
        if(frameNtf){
            if(frameNtf->lpdInfEvent.status == DETECTION_NOT_READY){
                DetectionDispatcher::instance().dispatch(ntf, lpd_tag);
                // frameNtf->lpdInfEvent.detectEvtReady.wait(1000);
            }
        }
        
        queueOut0->enqueueNotification(ntf);
        if(queueOut1 != nullptr){
            queueOut1->enqueueNotification(ntf);
        }
    }

}

}
}