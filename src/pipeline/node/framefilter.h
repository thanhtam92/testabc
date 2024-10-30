#pragma once

#include "pipeline/node.h"
#include "parameter/framefilterparameter.h"

namespace onevu {

namespace node {

class FrameFilter : public NodeCRTP<Node, FrameFilter, FrameFilterParameter> {

public:
    constexpr static const char* NAME = "FrameFilter";

    FrameFilter();
    FrameFilter(std::unique_ptr<FrameFilterParameter> params);

    void process() override;

private:
    uint64_t _first_frame_timestamp;
};

} // namespace
} // namespace