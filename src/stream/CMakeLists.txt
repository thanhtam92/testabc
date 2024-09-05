
set(COMPONENT_NAME onevu_stream)

file(GLOB SRC "*.cpp")

add_library(${COMPONENT_NAME} STATIC ${SRC})

install(TARGETS ${COMPONENT_NAME}
                RUNTIME DESTINATION ./
                LIBRARY DESTINATION lib
                ARCHIVE DESTINATION lib
                CONFIGURATIONS RELEASE
                COMPONENT libraries)