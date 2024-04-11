#include <QApplication>
#include <QProcess>

#include <memory>
#include <vector>

#include "roundWindow/roundWindow.hpp"
#include "rviz_common/ros_integration/ros_node_abstraction.hpp"

int main(int argc, char ** argv)
{
  QApplication app(argc, argv);
  rclcpp::init(argc, argv);

  auto ros_node_abs =
    std::make_shared<rviz_common::ros_integration::RosNodeAbstraction>("rviz_render_node");

  auto round_window = std::make_shared<roundWindow>(&app, ros_node_abs);
  round_window->show();

  while (rclcpp::ok()) {
    app.processEvents();
  }

  return 0;
}
