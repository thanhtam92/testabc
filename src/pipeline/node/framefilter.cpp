#include "pipeline/node/framefilter.h"

namespace onevu {
namespace node {
FrameFilter::FrameFilter()
    : NodeCRTP<Node, FrameFilter, FrameFilterParameter>() {
}

FrameFilter::FrameFilter(std::unique_ptr<FrameFilterParameter> params)
    : NodeCRTP<Node, FrameFilter, FrameFilterParameter>(std::move(params)) {

}

void FrameFilter::process() {
    if(queueIn0 == nullptr) {
        LOG_ERROR("input queue is nullptr\n");
        return;
    };

    if(queueOut0 == nullptr) {
        LOG_ERROR("output queue is nullptr");
        return;
    }

    if(queueIn0->size() == 1) {
        _first_frame_timestamp = milliseconds_since_epoch();
    }
    if(queueIn0->size() == getParameter().max_num_frames
        || milliseconds_since_epoch() - _first_frame_timestamp > getParameter().max_filter_delay_ms){
        // find best frame in list
        Poco::Notification::Ptr bestFrameNtf;
        auto qsize = queueIn0->size();
        for(int i =0; i < qsize; i++){
            Poco::Notification::Ptr ntf = queueIn0->dequeueNotification();
            auto frameNtf = dynamic_cast<FrameNotification*>(ntf.get());
            if(frameNtf){
                

                bestFrameNtf = ntf;
            }
        }

        if(bestFrameNtf){
            queueOut0->enqueueNotification(bestFrameNtf);
            if(queueOut1){
                queueOut1->enqueueNotification(bestFrameNtf);
            }
        }
    }
}

}
}