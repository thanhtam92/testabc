#pragma once

#include "common/frame.h"
#include "trigger/trigger.h"

namespace onevu {

namespace trigger {

struct LPDTriggerGeneratorParameter {
    uint64_t duration_ms = 1000;
    int stream_id;  // 0, 1 or -1 for all streams
};

class LPDTrigger : public Trigger {
public:
    LPDTrigger(const LPDTriggerGeneratorParameter& param);
};

class LPDTriggerGenerator : public TriggerGenerator {

public:
    FrameNotificationQueue::Ptr queueIn;
    LPDTriggerGeneratorParameter triggerParam;

    LPDTriggerGenerator(const LPDTriggerGeneratorParameter& param);

    void process() override;
    void preStart() override;
    void afterStop() override;

private:
    uint64_t _last_process_timestamp;
};

}
}