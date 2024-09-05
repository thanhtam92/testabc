


set(COMPONENT_NAME lpr_stream)

file(GLOB SRC "*.cpp")

add_executable(${COMPONENT_NAME} ${SRC})

target_link_libraries(${COMPONENT_NAME} 
        onevu_common
        onevu_mqtt
        onevu_pipeline
        onevu_stream
        )

install(TARGETS ${COMPONENT_NAME} DESTINATION "bin")