
set(COMPONENT_NAME onevu_pipeline)

file(GLOB SRC "*.cpp"
                "node/*.cpp")

add_library(${COMPONENT_NAME} STATIC ${SRC})

target_link_libraries(${COMPONENT_NAME} 
        onevu_common
        onevu_trigger
        onevu_detection
        )

install(TARGETS ${COMPONENT_NAME}
                RUNTIME DESTINATION ./
                LIBRARY DESTINATION lib
                ARCHIVE DESTINATION lib
                CONFIGURATIONS RELEASE
                COMPONENT libraries)