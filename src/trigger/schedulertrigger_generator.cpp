#include "trigger/schedulertrigger_generator.h"


namespace onevu {

namespace trigger {

SchedulerTrigger::SchedulerTrigger(const SchedulerTriggerGeneratorParameter& param)
    : Trigger() {
    this->stream_id = param.stream_id;
    this->duration_ms = param.duration_ms;
    this->name = "SchedulerTrigger";
}

SchedulerTriggerGenerator::SchedulerTriggerGenerator(const SchedulerTriggerGeneratorParameter& param)
    : triggerParam(param) {
    this->create_cronexpr();
}

bool SchedulerTriggerGenerator::check_expression_isvalid(const std::string& str){
    try {
        auto cronexpr = cron::make_cron(str);
        return true;
    }
    catch(cron::bad_cronexpr const & ex){
        LOG_INFO("cron expression (%s) is not valid: %s", str.c_str(), ex.what());
        return false;
    }
}

void SchedulerTriggerGenerator:: create_cronexpr(){
    if(!this->triggerParam.cron_expressions.empty()){
        try {
            _cronexpr = cron::make_cron(this->triggerParam.cron_expressions);
        }
        catch(cron::bad_cronexpr const & ex){
            LOG_INFO("cron expression (%s) is not valid: %s", this->triggerParam.cron_expressions.c_str(), ex.what());
        }
    }
}

void SchedulerTriggerGenerator::preStart() {
    _next_trigger_time = cron::cron_next(_cronexpr, std::time(0));
}

void SchedulerTriggerGenerator::afterStop() {

}

void SchedulerTriggerGenerator::process() {
    std::lock_guard<std::mutex> lock(_param_change_mtx);
    
    if(_next_trigger_time == cron::INVALID_TIME){
        return;
    }

    if(_next_trigger_time - std::time(0) >= 0){
        
        this->dispatch(new SchedulerTrigger(this->triggerParam));

        _next_trigger_time = cron::cron_next(_cronexpr, std::time(0));
    }
}

void SchedulerTriggerGenerator::updateParammeter(const SchedulerTriggerGeneratorParameter& param){
    std::lock_guard<std::mutex> lock(_param_change_mtx);
    this->triggerParam = param;
    this->create_cronexpr();
}

}
}