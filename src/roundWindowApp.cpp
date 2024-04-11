#include "roundRviz/roundWindow.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QBrush>


#include "rclcpp/clock.hpp"
#include "rviz_common/render_panel.hpp"
#include "rviz_common/ros_integration/ros_node_abstraction.hpp"
#include "rviz_common/visualization_manager.hpp"
#include "rviz_rendering/render_window.hpp"



roundWindow::roundWindow(QWidget * parent)
: QMainWindow(parent)
{
  // prep from round display
  this->setWindowFlags(Qt::FramelessWindowHint);
  this->setAttribute(Qt::WA_TranslucentBackground);
  desktop_ = QApplication::desktop();
  
  auto screenNum = desktop_->primaryScreen();
  auto theRect = desktop_->screenGeometry(screenNum);
  auto theMiddle = theRect.center();
  this->setGeometry(theRect);
  maskImage_ = QImage(theRect.width(),theRect.height(),QImage::Format_ARGB32);
  maskImage_.fill(Qt::transparent);

  auto brush = QBrush(QColor(0,0,0,255)); 
	auto painter = QPainter(&maskImage_); 
	painter.setBrush(brush);
  painter.setPen(Qt::NoPen);

  auto workRect = theRect;
  workRect.moveCenter(QPoint(this->width()/2,this->height()/2));

  painter.drawEllipse(workRect);

  painter.end();

  maskPixmap_ = QPixmap();

  auto pixResult = maskPixmap_.convertFromImage(maskImage_);

}

void roundWindow::paintEvent(QPaintEvent *) {

  auto qp = QPainter(this);
  qp.drawPixmap(QPoint(0,0),this->maskPixmap_);

}

QWidget *
roundWindow::getParentWindow()
{
  return this;
}


void roundWindow::closeEvent(QCloseEvent * event)
{
  QWidget::closeEvent(event);
  rclcpp::shutdown();
}
