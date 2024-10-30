#pragma once

#include "common/frame.h"
namespace onevu {

struct DetectionParammeter {
    std::string engine_file;
    int detect_score_threshold;
};

class DetectionWrapper {

public:
    bool init(const DetectionParammeter& param);

    void process(Poco::Notification::Ptr ntf);
};

}