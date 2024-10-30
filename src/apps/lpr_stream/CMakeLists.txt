


set(COMPONENT_NAME lpr_stream)

file(GLOB SRC "*.cpp")

add_executable(${COMPONENT_NAME} ${SRC})

target_link_libraries(${COMPONENT_NAME} 
        # onevu_common
        onevu_mqtt
        onevu_detection
        onevu_trigger
        onevu_stream
        onevu_pipeline
        )

install(TARGETS ${COMPONENT_NAME} DESTINATION "bin")