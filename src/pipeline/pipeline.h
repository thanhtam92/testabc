#pragma once

#include <thread>
#include <chrono>
#include <list>

#include "pipeline/node.h"
#include "pipeline/node/inputstream.h"
#include "pipeline/node/trigger_aggregator.h"
#include "pipeline/node/framefilter.h"
#include "pipeline/node/preselection.h"
#include "pipeline/node/licenseplate_detector.h"
#include "pipeline/node/vehicle_detector.h"
#include "pipeline/node/roi_mapping.h"
#include "trigger/trigger.h"
#include "trigger/triggermanager.h"

namespace onevu {

class Pipeline {

public:

    static Pipeline create();

    void start();
    void stop();

private:
    void run();

    bool _isRunning = false;
    std::thread _pipelineThread;
    std::list<std::unique_ptr<Node>> _listNode;
    std::list<std::unique_ptr<TriggerGenerator>> _listTriggerGenerator;
};

}