#include "pipeline/node/vehicle_detector.h"
#include "detection/detection_dispatcher.h"

namespace onevu {

namespace node {

VehicleDetector::VehicleDetector() 
    : NodeCRTP<Node, VehicleDetector, VehicleDetectorParameter>(std::make_unique<VehicleDetectorParameter>()){

}

VehicleDetector::VehicleDetector(std::unique_ptr<VehicleDetectorParameter> params)
 : NodeCRTP<Node, VehicleDetector, VehicleDetectorParameter>(std::move(params)) {

}

VehicleDetector::~VehicleDetector() {

}

void VehicleDetector::process() {
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
        if(frameNtf->vdInfoEvent.status == DETECTION_NOT_READY){
            DetectionDispatcher::instance().dispatch(ntf, vd_tag);
            frameNtf->vdInfoEvent.detectEvtReady.wait(1000);
        }

        queueOut0->enqueueNotification(ntf);
        if(queueOut1 != nullptr){
            queueOut1->enqueueNotification(ntf);
        }
    }

}

}
}