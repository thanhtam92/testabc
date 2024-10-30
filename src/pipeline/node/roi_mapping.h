#pragma once

#include "pipeline/node.h"
#include "parameter/roimappingparameter.h"

namespace onevu {

namespace node {

class RoiMapping : public NodeCRTP<Node, RoiMapping, RoiMappingParameter> {

public:
    constexpr static const char* NAME = "RoiMapping";

    RoiMapping();
    RoiMapping(std::unique_ptr<RoiMappingParameter> params);
    ~RoiMapping();

    void process() override;
private:
};

} // namespace node
} // onevu