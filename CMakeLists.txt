cmake_minimum_required(VERSION 3.5)
project(onevu-lpr-core)

macro(copy_data file)
    if(EXISTS "${CMAKE_SOURCE_DIR}/${file}")
        install(FILES ${CMAKE_SOURCE_DIR}/${file} DESTINATION bin)
    else()
        message(STATUS "${CMAKE_SOURCE_DIR}/${file} NOT FOUND")
    endif()
endmacro(copy_data file)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# try using c++17, if c++17 does not find, a previous standard will be used
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fPIC")

# https://jpinjpblog.wordpress.com/2017/12/18/including-the-paho-mqtt-cpp-in-cmake/
# find_library(Mqtt NAMES libpaho-mqtt3c.so REQUIRED)
find_package(eclipse-paho-mqtt-c REQUIRED)
find_package(Poco REQUIRED Foundation Util JSON XML Net Crypto NetSSL)
find_package(OpenCV REQUIRED )

include_directories(./)
include_directories(src)
add_subdirectory(src/common)
add_subdirectory(src/mqtt)
add_subdirectory(src/stream)
add_subdirectory(src/detection)
add_subdirectory(src/trigger)
add_subdirectory(src/pipeline)
add_subdirectory(src/apps)

copy_data(data/lpr_stream.properties)

# add_subdirectory(test)

# set( cppcheck_format "{file}:{line}: [{severity}][{id}] {message} {callstack}")
# add_custom_target(
#    cppcheck
#    COMMAND  cppcheck
#        --enable=all
#        --std=c++14
#        --quiet
#        --inline-suppr
#     #    --suppress=useInitializationList
#     #    --suppress=uninitMemberVar
#        --suppress=missingInclude
#     #    --suppress=unusedFunction
#     #    --suppress=unusedPrivateFunction
#     #    --suppress=unmatchedSuppression
#     #    --suppress=useStlAlgorithm
#        --template=${cppcheck_format}
#        --error-exitcode=1
#        "${CMAKE_CURRENT_SOURCE_DIR}/src/"
#        -i "${CMAKE_CURRENT_SOURCE_DIR}/src/external"
#        -i "${CMAKE_CURRENT_SOURCE_DIR}/src/process/vio"
#        --check-config
# )
