#pragma once

#include <QApplication>
#include <QMainWindow>
#include <QToolButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QProcess>
#include <QDesktopWidget>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QRect>
#include <QLabel>
#include <QPalette>

#include "rclcpp/rclcpp.hpp"
#include "rviz_common/display.hpp"
#include <rviz_common/display_context.hpp>
#include "rviz_common/window_manager_interface.hpp"
#include "rviz_common/ros_integration/ros_node_abstraction.hpp"
#include "rviz_common/ros_integration/ros_client_abstraction.hpp"
#include "rviz_common/panel_dock_widget.hpp"


class QPushButton;
class QString;

namespace rviz_common
{
class Display;
class RenderPanel;
class VisualizationManager;
}

class roundWindow: public QMainWindow,public rviz_common::WindowManagerInterface 
{
Q_OBJECT
public:
  roundWindow(QApplication *app = 0,QWidget *parent = 0);
  void setStatus(const QString & message) override;
  QWidget * getParentWindow() override;
  rviz_common::PanelDockWidget * addPane(const QString & name, QWidget * pane, Qt::DockWidgetArea area, bool floating) override;

  void message(std::string &string);

  //QWidget * getParentWindow() override;

protected:
  void paintEvent(QPaintEvent *e);
  QSize sizeHint() const;

private slots:
  void closeEvent(QCloseEvent *event);
  void startCamera();
  void stopCamera();
  void toggleRecord();
  void stopRecord();
  void doShutdown();
  void startDockerBridge();
  void stopDockerBridge();
  void handle_stderr();
  void handle_stdout();
  void handle_state(QProcess::ProcessState state);
  void process_finished(int val,QProcess::ExitStatus status);
  void process_uros_finished(int val,QProcess::ExitStatus status);
  void handle_uros_stderr();
  void handle_uros_stdout();
  void handle_uros_state(QProcess::ProcessState state);


private:

  QApplication * app_;
  QDesktopWidget * desktop_;
  QImage maskImage_;
  QPixmap maskPixmap_;
  QRect theRect_;

  QPushButton * startButton_;
  QPushButton * stopButton_;
  QPushButton * recordButton_;
  QPushButton * shutdownButton_; 


  QWidget * central_widget_;
  QVBoxLayout * main_layout_;
  QProcess * urosBridgeDockerProcess_;
  QProcess * cameraRosProcess_;
  QProcess * recordRosProcess_;
  QProcess * imuRosProcess_;
  QVBoxLayout * mainBoxLayout_;
  QDockWidget * mainDockWiget_;
  QGridLayout * mainDockLayout_;

  rviz_common::RenderPanel * render_panel_;
  rviz_common::Display * grid_;
  rviz_common::Display * camera_;
  rviz_common::VisualizationManager * manager_;

  bool quitting_;

  rviz_common::ros_integration::RosNodeAbstractionIface::WeakPtr rviz_ros_node_;

  std::unique_ptr<rviz_common::ros_integration::RosClientAbstractionIface> ros_client_abstraction_;

  
};
