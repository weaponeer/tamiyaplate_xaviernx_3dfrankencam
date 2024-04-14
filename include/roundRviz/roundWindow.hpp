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
  void setupManagers();

  //QWidget * getParentWindow() override;

protected:
  void paintEvent(QPaintEvent *e);
  QSize sizeHint() const;

private slots:
  void closeEvent(QCloseEvent *event);


private:

  QApplication * app_;
  QDesktopWidget * desktop_;
  QImage maskImage_;
  QPixmap maskPixmap_;
  QRect theRect_;

  QWidget * central_widget;
  QVBoxLayout * main_layout;
  QProcess * imuDockerProcess;

  QLabel label1 {this};

  rviz_common::RenderPanel * render_panel_;
  rviz_common::Display * grid_;
  rviz_common::Display * camera_;
  rviz_common::VisualizationManager * manager_;

  rviz_common::ros_integration::RosNodeAbstractionIface::WeakPtr rviz_ros_node_;
};
