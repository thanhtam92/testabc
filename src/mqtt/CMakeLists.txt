set(COMPONENT_NAME onevu_mqtt)

file(GLOB SRC "*.cpp")

add_library(${COMPONENT_NAME} STATIC ${SRC})

target_link_libraries(${COMPONENT_NAME} 
        onevu_common
        eclipse-paho-mqtt-c::paho-mqtt3c
        )

install(TARGETS ${COMPONENT_NAME}
                RUNTIME DESTINATION ./
                LIBRARY DESTINATION lib
                ARCHIVE DESTINATION lib
                CONFIGURATIONS RELEASE
                COMPONENT libraries)