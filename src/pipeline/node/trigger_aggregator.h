#pragma once

#include "pipeline/node.h"
#include "parameter/triggeraggregatorparameter.h"

namespace onevu {

namespace node {

class TriggerAggregator : public NodeCRTP<Node, TriggerAggregator, TriggerAggregatorParameter> {

public:
    constexpr static const char* NAME = "TriggerAggregator";

    TriggerAggregator();
    TriggerAggregator(std::unique_ptr<TriggerAggregatorParameter> params);
    ~TriggerAggregator();

    void process() override;
private:
};

} // namespace node
} // onevu