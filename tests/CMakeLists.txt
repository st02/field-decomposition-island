include_directories(${PROJECT_SOURCE_DIR}/third-party include)

add_executable(test_fdi_tpl test_fdi_tpl.cpp)

target_link_libraries(test_fdi_tpl fdi_tpl)


add_executable(test_fdi test_fdi.cpp)

target_link_libraries(test_fdi fdi)

