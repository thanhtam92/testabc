#include "pipeline/node/preselection.h"

namespace onevu {

namespace node {

PreSelection::PreSelection() 
    : PreSelection(std::make_unique<PreSelectionParameter>()){

}

PreSelection::PreSelection(std::unique_ptr<PreSelectionParameter> params)
 : NodeCRTP<Node, PreSelection, PreSelectionParameter>(std::move(params)) {

}

PreSelection::~PreSelection() {

}

void PreSelection::process() {
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
            if(frameNtf->isTriggered){
                queueOut0->enqueueNotification(ntf);
                if(queueOut1 != nullptr){
                    queueOut1->enqueueNotification(ntf);
                }
            }
        }
    }

}

}
}