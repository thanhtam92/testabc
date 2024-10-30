#pragma once
#include "parameter.h"

namespace onevu {

struct FrameFilterParameter : Parameters {
    int max_num_frames = 60;
    int max_filter_delay_ms = 2000;
    double filter_threshold_value = 0.8;
};

}