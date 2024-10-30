#pragma once

#include "pipeline/node.h"
#include "parameter/inputstreamparameter.h"

namespace onevu {

namespace node {

class InputStream : public NodeCRTP<Node, InputStream, InputStreamParameter> {

public:
    constexpr static const char* NAME = "InputStream";

    InputStream();
    InputStream(std::unique_ptr<InputStreamParameter> params);
    ~InputStream();

    void process() override;
private:
};

} // namespace node
} // onevu