#pragma once

#include "pipeline/node.h"
#include "parameter/vehicledetectorparameter.h"

namespace onevu {

namespace node {

class VehicleDetector : public NodeCRTP<Node, VehicleDetector, VehicleDetectorParameter> {

public:
    constexpr static const char* NAME = "VehicleDetector";

    VehicleDetector();
    VehicleDetector(std::unique_ptr<VehicleDetectorParameter> params);
    ~VehicleDetector();

    void process() override;
private:
};

} // namespace node
} // onevu