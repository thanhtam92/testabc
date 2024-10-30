#pragma once

#include <string>
#include <unordered_map>

#include "common/frame.h"
#include "common/log.h"
#include "common/util.h"
#include "parameter/parameter.h"

namespace onevu {

class Node {

public:
    using UPtr = std::unique_ptr<Node>;

    Node() = default;
    virtual ~Node() = default;

    FrameNotificationQueue::Ptr queueIn0;
    FrameNotificationQueue::Ptr queueIn1;

    FrameNotificationQueue::Ptr queueOut0;
    FrameNotificationQueue::Ptr queueOut1;

    virtual const char* getName() const = 0;
    virtual bool preProcess();
    virtual void process() = 0;
    virtual void postProcess();
};

// Node CRTP class
template <typename Base, typename Derived, typename Params>
class NodeCRTP : public Base {
public:
    using Parameters_t = Params;

    Parameters_t& getParameter() { return *_paramsHolder; }

    const char* getName() const override {
        return Derived::NAME;
    };

protected:
    std::unique_ptr<Parameters_t> _paramsHolder;

private:
    NodeCRTP(std::unique_ptr<Parameters_t> params)
        : Base(), _paramsHolder(std::move(params)) {
        }
    NodeCRTP() : NodeCRTP(std::move(std::make_unique<Parameters_t>())) {
    }
    friend Derived;
    friend Base;
};

}