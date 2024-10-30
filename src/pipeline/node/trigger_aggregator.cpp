#include "pipeline/node/trigger_aggregator.h"
#include "trigger/triggermanager.h"

namespace onevu {

namespace node {

TriggerAggregator::TriggerAggregator() 
    : TriggerAggregator(std::make_unique<TriggerAggregatorParameter>()){

}

TriggerAggregator::TriggerAggregator(std::unique_ptr<TriggerAggregatorParameter> params)
 : NodeCRTP<Node, TriggerAggregator, TriggerAggregatorParameter>(std::move(params)) {

}

TriggerAggregator::~TriggerAggregator() {

}

void TriggerAggregator::process() {
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
        auto triggerList = TriggerManager::instance().getCurrentTriggers();
        if(triggerList.size() > 0){
            auto frameNtf = dynamic_cast<FrameNotification*>(ntf.get());
            if(frameNtf){
                frameNtf->isTriggered = true;
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