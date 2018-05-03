set(fdi_sources fdi/fdi.cpp)

add_library(fdi ${fdi_sources})

target_include_directories(fdi PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/fdi>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/fdi>
)



add_library(fdi_tpl INTERFACE ${fdi_template_sources})
target_include_directories(fdi_tpl INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/fdi>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/fdi>
  )


SET(HEADERS_TO_INSTALL
    "fdi/fdi.h"
    "fdi/fdi.hpp"
)

INSTALL(FILES ${HEADERS_TO_INSTALL} DESTINATION "include/fdi")
INSTALL(TARGETS fdi DESTINATION "lib" EXPORT ${PROJECT_NAME}-targets)

