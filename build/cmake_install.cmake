# Install script for directory: /home/q/workspace/tamiyaplate_xaviernx_3dfrankencam

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/aarch64-linux-gnu-objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/round_window_cam/round_window_cam" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/round_window_cam/round_window_cam")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/round_window_cam/round_window_cam"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/round_window_cam" TYPE EXECUTABLE FILES "/home/q/workspace/tamiyaplate_xaviernx_3dfrankencam/build/round_window_cam")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/round_window_cam/round_window_cam" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/round_window_cam/round_window_cam")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/round_window_cam/round_window_cam"
         OLD_RPATH "/home/q/ros2_iron/install/rclcpp_components/lib:/home/q/ros2_iron/install/rviz_default_plugins/lib:/home/q/ros2_iron/install/rviz_common/lib:/home/q/ros2_iron/install/rviz_rendering/lib:/home/q/ros2_iron/install/rviz_ogre_vendor/opt/rviz_ogre_vendor/lib:/home/q/ros2_iron/install/composition_interfaces/lib:/home/q/ros2_iron/install/tf2_ros/lib:/home/q/ros2_iron/install/resource_retriever/lib:/home/q/ros2_iron/install/rclcpp_action/lib:/home/q/ros2_iron/install/rcl_action/lib:/home/q/ros2_iron/install/tf2_msgs/lib:/home/q/ros2_iron/install/action_msgs/lib:/home/q/ros2_iron/install/unique_identifier_msgs/lib:/home/q/ros2_iron/install/urdf/lib:/home/q/ros2_iron/install/class_loader/lib:/home/q/ros2_iron/install/urdfdom/lib:/home/q/ros2_iron/install/image_transport/lib:/home/q/ros2_iron/install/message_filters/lib:/home/q/ros2_iron/install/interactive_markers/lib:/home/q/ros2_iron/install/laser_geometry/lib:/home/q/ros2_iron/install/rclcpp/lib:/home/q/ros2_iron/install/libstatistics_collector/lib:/home/q/ros2_iron/install/rcl/lib:/home/q/ros2_iron/install/rcl_logging_interface/lib:/home/q/ros2_iron/install/rmw_implementation/lib:/home/q/ros2_iron/install/ament_index_cpp/lib:/home/q/ros2_iron/install/type_description_interfaces/lib:/home/q/ros2_iron/install/rcl_interfaces/lib:/home/q/ros2_iron/install/rcl_yaml_param_parser/lib:/home/q/ros2_iron/install/rosgraph_msgs/lib:/home/q/ros2_iron/install/statistics_msgs/lib:/home/q/ros2_iron/install/tracetools/lib:/home/q/ros2_iron/install/tf2/lib:/home/q/ros2_iron/install/map_msgs/lib:/home/q/ros2_iron/install/nav_msgs/lib:/home/q/ros2_iron/install/visualization_msgs/lib:/home/q/ros2_iron/install/sensor_msgs/lib:/home/q/ros2_iron/install/geometry_msgs/lib:/home/q/ros2_iron/install/std_msgs/lib:/home/q/ros2_iron/install/service_msgs/lib:/home/q/ros2_iron/install/builtin_interfaces/lib:/home/q/ros2_iron/install/rosidl_typesupport_fastrtps_c/lib:/home/q/ros2_iron/install/rosidl_typesupport_fastrtps_cpp/lib:/home/q/ros2_iron/install/rmw/lib:/home/q/ros2_iron/install/rosidl_dynamic_typesupport/lib:/home/q/ros2_iron/install/fastcdr/lib:/home/q/ros2_iron/install/rosidl_typesupport_introspection_cpp/lib:/home/q/ros2_iron/install/rosidl_typesupport_introspection_c/lib:/home/q/ros2_iron/install/rosidl_typesupport_cpp/lib:/home/q/ros2_iron/install/rosidl_typesupport_c/lib:/home/q/ros2_iron/install/rcpputils/lib:/home/q/ros2_iron/install/rosidl_runtime_c/lib:/home/q/ros2_iron/install/rcutils/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/aarch64-linux-gnu-strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/round_window_cam/round_window_cam")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/package_run_dependencies" TYPE FILE FILES "/home/q/workspace/tamiyaplate_xaviernx_3dfrankencam/build/ament_cmake_index/share/ament_index/resource_index/package_run_dependencies/round_window_cam")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/parent_prefix_path" TYPE FILE FILES "/home/q/workspace/tamiyaplate_xaviernx_3dfrankencam/build/ament_cmake_index/share/ament_index/resource_index/parent_prefix_path/round_window_cam")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/round_window_cam/environment" TYPE FILE FILES "/home/q/ros2_iron/install/ament_cmake_core/share/ament_cmake_core/cmake/environment_hooks/environment/ament_prefix_path.sh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/round_window_cam/environment" TYPE FILE FILES "/home/q/workspace/tamiyaplate_xaviernx_3dfrankencam/build/ament_cmake_environment_hooks/ament_prefix_path.dsv")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/round_window_cam/environment" TYPE FILE FILES "/home/q/ros2_iron/install/ament_cmake_core/share/ament_cmake_core/cmake/environment_hooks/environment/path.sh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/round_window_cam/environment" TYPE FILE FILES "/home/q/workspace/tamiyaplate_xaviernx_3dfrankencam/build/ament_cmake_environment_hooks/path.dsv")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/round_window_cam" TYPE FILE FILES "/home/q/workspace/tamiyaplate_xaviernx_3dfrankencam/build/ament_cmake_environment_hooks/local_setup.bash")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/round_window_cam" TYPE FILE FILES "/home/q/workspace/tamiyaplate_xaviernx_3dfrankencam/build/ament_cmake_environment_hooks/local_setup.sh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/round_window_cam" TYPE FILE FILES "/home/q/workspace/tamiyaplate_xaviernx_3dfrankencam/build/ament_cmake_environment_hooks/local_setup.zsh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/round_window_cam" TYPE FILE FILES "/home/q/workspace/tamiyaplate_xaviernx_3dfrankencam/build/ament_cmake_environment_hooks/local_setup.dsv")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/round_window_cam" TYPE FILE FILES "/home/q/workspace/tamiyaplate_xaviernx_3dfrankencam/build/ament_cmake_environment_hooks/package.dsv")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/packages" TYPE FILE FILES "/home/q/workspace/tamiyaplate_xaviernx_3dfrankencam/build/ament_cmake_index/share/ament_index/resource_index/packages/round_window_cam")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/round_window_cam/cmake" TYPE FILE FILES
    "/home/q/workspace/tamiyaplate_xaviernx_3dfrankencam/build/ament_cmake_core/round_window_camConfig.cmake"
    "/home/q/workspace/tamiyaplate_xaviernx_3dfrankencam/build/ament_cmake_core/round_window_camConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/round_window_cam" TYPE FILE FILES "/home/q/workspace/tamiyaplate_xaviernx_3dfrankencam/package.xml")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/q/workspace/tamiyaplate_xaviernx_3dfrankencam/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
