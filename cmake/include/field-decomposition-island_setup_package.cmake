configure_file(
  "${PROJECT_SOURCE_DIR}/cmake/pkg/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/cmake/pkg/${PROJECT_NAME}Config.cmake"
  @ONLY
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/cmake/pkg/${PROJECT_NAME}Config-version.cmake"
    VERSION ${${PROJECT_NAME}_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(EXPORT ${PROJECT_NAME}-targets
    DESTINATION "share/cmake/targets"
    NAMESPACE "${PROJECT_NAME}::"
)

install(
    FILES
    "${PROJECT_BINARY_DIR}/cmake/pkg/${PROJECT_NAME}Config.cmake"
    "${PROJECT_BINARY_DIR}/cmake/pkg/${PROJECT_NAME}Config-version.cmake"
    DESTINATION "share/cmake/pkg"
)
