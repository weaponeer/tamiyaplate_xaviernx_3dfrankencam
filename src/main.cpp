#include <QApplication>
#include <QProcess>

#include <memory>
#include <vector>

#include "roundRviz/roundWindow.hpp"


int main(int argc, char ** argv)
{
  QApplication app(argc, argv);
  rclcpp::init(argc, argv);
  
  auto round_window = std::make_shared<roundWindow>(&app);
  round_window->showFullScreen();

  while (rclcpp::ok()) {
    app.processEvents();
  }

  return 0;
}
