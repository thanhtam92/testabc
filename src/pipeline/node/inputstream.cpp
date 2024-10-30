#include "pipeline/node/inputstream.h"

namespace onevu {

namespace node {

InputStream::InputStream() 
    : InputStream(std::make_unique<InputStreamParameter>()){
}

InputStream::InputStream(std::unique_ptr<InputStreamParameter> params)
 : NodeCRTP<Node, InputStream, InputStreamParameter>(std::move(params)) {

}

InputStream::~InputStream() {

}

void InputStream::process() {
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