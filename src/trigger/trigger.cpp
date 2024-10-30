
#include "trigger/trigger.h"
#include "trigger/triggermanager.h"

namespace onevu {

Trigger::Trigger()
    : timestamp_ms(milliseconds_since_epoch()), duration_ms(1000) {

}

void TriggerGenerator::dispatch(Poco::AutoPtr<Trigger> trigg){
    TriggerManager::instance().dispatch(trigg);
}

void TriggerGenerator::preStart() {

}

void TriggerGenerator::start() {
    if(_isRunning){
        LOG_INFO("TriggerThread is running!!!");
        return;
    }

    // do something before starting loop
    this->preStart();

    _isRunning = true;
    _threadTrigger = std::thread([&](){
        while(this->_isRunning){

            auto start_time = milliseconds_since_epoch();

            this->process();

            auto process_time = milliseconds_since_epoch() - start_time;
            if(process_time < 10){
                std::this_thread::sleep_for(std::chrono::milliseconds(10 - process_time));
            }
        };
    });
}

void TriggerGenerator::stop() {
    _isRunning = false;
    _threadTrigger.join();

    // do something after stopping loop, maybe release some resources
    this->afterStop();
}

void TriggerGenerator::afterStop() {

}

}