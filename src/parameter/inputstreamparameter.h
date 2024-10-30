#pragma once

#include "parameter.h"

namespace onevu {

struct InputStreamParameter : Parameters {
    std::string stream_url;
    uint32_t    res_iwidth;
    uint32_t    res_iheigh;
};

}