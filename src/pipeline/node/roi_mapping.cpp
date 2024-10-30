#include "pipeline/node/roi_mapping.h"

namespace onevu {

namespace node {

RoiMapping::RoiMapping() 
    : NodeCRTP<Node, RoiMapping, RoiMappingParameter>(std::make_unique<RoiMappingParameter>()){

}

RoiMapping::RoiMapping(std::unique_ptr<RoiMappingParameter> params)
 : NodeCRTP<Node, RoiMapping, RoiMappingParameter>(std::move(params)) {

}

RoiMapping::~RoiMapping() {

}

void RoiMapping::process() {
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
    auto frameNtf = queueIn0->dequeueNotification();
    if(frameNtf != nullptr){
        // todo:

        queueOut0->enqueueNotification(frameNtf);
        if(queueOut1 != nullptr){
            queueOut1->enqueueNotification(frameNtf);
        }
    }

}

}
}