cmake_minimum_required(VERSION 3.5)
project(test_pybind)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(${PROJECT_NAME}_module src/test_pybind.cpp)
target_include_directories(${PROJECT_NAME}_module PRIVATE /opt/ros/foxy/include)
target_link_libraries(${PROJECT_NAME}_module PRIVATE pybind11::module pthread ${rclcpp_LIBRARIES} ${std_msgs_LIBRARIES})
install(TARGETS ${PROJECT_NAME}_module DESTINATION lib/${PROJECT_NAME})

ament_package()
