#pragma once

#include <mutex>
#include "common/frame.h"
#include "trigger/trigger.h"
#include "trigger/croncpp.h"

namespace onevu {

namespace trigger {

struct SchedulerTriggerGeneratorParameter {
    std::string cron_expressions;
    uint64_t duration_ms = 1000;
    int stream_id;  // 0, 1 or -1 for all streams
};

class SchedulerTrigger : public Trigger {
public:
    SchedulerTrigger(const SchedulerTriggerGeneratorParameter& param);
};

class SchedulerTriggerGenerator : public TriggerGenerator {

public:
    SchedulerTriggerGeneratorParameter triggerParam;

    SchedulerTriggerGenerator(const SchedulerTriggerGeneratorParameter& param);

    void updateParammeter(const SchedulerTriggerGeneratorParameter& param);

    void process() override;
    void preStart() override;
    void afterStop() override;

    static bool check_expression_isvalid(const std::string& str);
private:

    void create_cronexpr();

    cron::cronexpr _cronexpr;
    std::time_t _next_trigger_time;
    std::mutex _param_change_mtx;
};

}
}