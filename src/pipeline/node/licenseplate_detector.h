#pragma once

#include "pipeline/node.h"
#include "parameter/licenseplateparameter.h"

namespace onevu {

namespace node {

class LicensePlateDetector : public NodeCRTP<Node, LicensePlateDetector, LicensePlateDetectorParameter> {

public:
    constexpr static const char* NAME = "LicensePlateDetector";

    LicensePlateDetector();
    LicensePlateDetector(std::unique_ptr<LicensePlateDetectorParameter> params);
    ~LicensePlateDetector();

    void process() override;
private:
};

} // namespace node
} // onevu