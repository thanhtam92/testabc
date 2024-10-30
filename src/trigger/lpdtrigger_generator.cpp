#include "trigger/lpdtrigger_generator.h"
#include "detection/detection_dispatcher.h"

namespace onevu {

namespace trigger {

LPDTrigger::LPDTrigger(const LPDTriggerGeneratorParameter& param)
    : Trigger() {
    this->duration_ms = param.duration_ms;
    this->name = "LPDTrigger";
    this->stream_id = param.stream_id;
}

LPDTriggerGenerator::LPDTriggerGenerator(const LPDTriggerGeneratorParameter& param)
    : triggerParam(param) {
    queueIn = std::make_shared<FrameNotificationQueue>();
}

void LPDTriggerGenerator::preStart(){
    _last_process_timestamp = milliseconds_since_epoch() - 1000;
}

void LPDTriggerGenerator::afterStop(){
    queueIn->clear();
}

void LPDTriggerGenerator::process() {
    Poco::Notification::Ptr ntf = queueIn->waitDequeueNotification(100);
    // run each 1s
    if(ntf
        && milliseconds_since_epoch() - _last_process_timestamp >= 1000){
        // detection
        // wait for result
        // create trigger
        // dispatch trigger
        DetectionDispatcher::instance().dispatch(ntf, lpd_tag);

        auto ntfFrame = dynamic_cast<FrameNotification*>(ntf.get());
        if(ntfFrame){
            if(ntfFrame->lpdInfEvent.status == DETECTION_NOT_READY){
                ntfFrame->lpdInfEvent.detectEvtReady.wait(1000);
            }

            if(ntfFrame->lpdInfEvent.status == DETECTION_FOUND){
                this->dispatch(new LPDTrigger(this->triggerParam));
                _last_process_timestamp = milliseconds_since_epoch();
            }
        }
    }
}

}
}