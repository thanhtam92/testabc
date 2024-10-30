
#include <string>
#include <memory>
#include <string.h>
#include <cstdarg>

#include <sys/time.h>
#include <Poco/Util/Application.h>
#include <Poco/Util/LoggingSubsystem.h>

#include "common/log.h"
#include "common/util.h"

namespace onevu {
namespace Log {
static Poco::Logger *pLogger = nullptr;

void log( const char* filename, int file_line, Level level, const char* format, ... ) {
    pLogger = &Poco::Util::Application::instance().logger();
    if(!pLogger) return;

    va_list format_args;
    va_start( format_args, format);
    uint32_t buffer_size = 1024;
    
    auto buffer = std::make_unique< char[] >( buffer_size  );
    memset( buffer.get(), 0x00, buffer_size );
     // If we would have static buffer, then the trouble is with multithread access
    ssize_t ret = vsnprintf(buffer.get(), buffer_size, format, format_args );
    va_end( format_args );
    
    if ( ret <= 0 ) {
        pLogger->error("Log call failed from '%s':%d", filename, file_line );
        return; 
    }
        
    if ( ret >= buffer_size ) {
        pLogger->error("Too long log at '%s':%d", filename, file_line );
        return;
    }
    
    std::string msg(buffer.get());

    switch ( level )
    {
        case Level::Debug:
            pLogger->debug(msg, filename, file_line);
            break;

        case Level::Info:
            pLogger->information(msg, filename, file_line);
            break;
            
        case Level::Warning:
            pLogger->warning(msg, filename, file_line);
            break;
            
        default:
            pLogger->error(msg, filename, file_line);
            break;
    }
}

} // namespace
} // namespace

