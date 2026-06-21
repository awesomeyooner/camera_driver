cmake_minimum_required(VERSION 3.16)
project(camera_driver LANGUAGES C CXX)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# In order to compile external libs
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# The Main Exec Name
set(MAIN_EXEC camera_driver)

add_subdirectory(lib/plib)

# Dependency list
set(THIS_PACKAGE_INCLUDE_DEPENDS
  ament_cmake
  backward_ros
  rclcpp
  std_msgs
  sensor_msgs
  geometry_msgs
  cv_bridge
  OpenCV
  generate_parameter_library
  CURL
)

# Find all dependencies
foreach(Dependency IN ITEMS ${THIS_PACKAGE_INCLUDE_DEPENDS})
  find_package(${Dependency} REQUIRED)
endforeach()

# Generate Parameter Library
generate_parameter_library( ${PROJECT_NAME}_parameters
  src/parameters.yaml
)

set(SRC_FILES
    src/main.cpp

    src/camera.cpp
    src/core_node.cpp
)

# Link the dependencies
add_executable( ${MAIN_EXEC} ${SRC_FILES})
ament_target_dependencies( ${MAIN_EXEC} PUBLIC ${THIS_PACKAGE_INCLUDE_DEPENDS})

ament_target_dependencies(${PROJECT_NAME} PUBLIC
  ${THIS_PACKAGE_INCLUDE_DEPENDS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${PROJECT_NAME}_parameters
    rclcpp::rclcpp
    plib
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

message("======================================")
message(${CMAKE_CURRENT_BINARY_DIR})
message("${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/include")

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib>
    $<INSTALL_INTERFACE:lib>

    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    # $<BUILD_INTERFACE:/home/ubuntu/Documents/Polar-One/ros2_ws/build/camera_driver/include>

    /opt/ros/jazzy/include
    /usr/include/opencv4
)

message("======================================")
message(STATUS "CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}")
message(${CMAKE_CURRENT_BINARY_DIR})
message("${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/include")


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

# Install Executable
install(TARGETS
  ${MAIN_EXEC}
  DESTINATION lib/${PROJECT_NAME}
)

# Source code
install(
  DIRECTORY include/
  DESTINATION include/${PROJECT_NAME}
)

# Description (urdf, xml, etc)
# install(
#   DIRECTORY description/rviz description/ros2_control description/urdf
#   DESTINATION share/${PROJECT_NAME}
# )

# Bringup (yaml configs, launch files)
install(
  DIRECTORY bringup/launch bringup/config
  DESTINATION share/${PROJECT_NAME}
)

# Whole project
install(TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# Export for AMENT
ament_export_targets("export_${PROJECT_NAME}" HAS_LIBRARY_TARGET)
ament_export_dependencies(${THIS_PACKAGE_INCLUDE_DEPENDS})
ament_package()
