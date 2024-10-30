#pragma once

#include "pipeline/node.h"
#include "parameter/preselectionparameter.h"

namespace onevu {

namespace node {

class PreSelection : public NodeCRTP<Node, PreSelection, PreSelectionParameter> {

public:
    constexpr static const char* NAME = "PreSelection";

    PreSelection();
    PreSelection(std::unique_ptr<PreSelectionParameter> params);
    ~PreSelection();

    void process() override;
private:
};

} // namespace node
} // onevu