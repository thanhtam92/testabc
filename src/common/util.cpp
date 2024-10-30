#include "common/util.h"

namespace onevu {

uint64_t milliseconds_since_epoch()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
};

}