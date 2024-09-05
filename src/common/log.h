
#pragma once

#include <string.h>

namespace onevu {
namespace Log {
    enum class Level { Debug, Info, Error , Warning };
    void log( const char* filename, int file_line, Level level, const char* module_name, const char* format, ... );
}
}

#define LOG_INFO( format, ... ) onevu::Log::log( __FILE__, __LINE__, onevu::Log::Level::Info, format , ## __VA_ARGS__ )
#define LOG_ERROR( format, ... ) onevu::Log::log( __FILE__, __LINE__, onevu::Log::Level::Error, format , ## __VA_ARGS__ )
#define LOG_WARNING( format, ... ) onevu::Log::log( __FILE__, __LINE__, onevu::Log::Level::Warning, format , ## __VA_ARGS__ )
#define LOG_PERROR( format, ... ) onevu::Log::log( __FILE__, __LINE__, onevu::Log::Level::Error, format , ## __VA_ARGS__ , strerror(errno) )
#define LOG_DEBUG( format, ... ) onevu::Log::log( __FILE__, __LINE__, onevu::Log::Level::Debug, format , ## __VA_ARGS__ )





