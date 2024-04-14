#include "roundRviz/roundWindow.hpp"
#include "roundRviz/rvizWidget.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QBrush>
#include <QPushButton>
#include <QBoxLayout>


#include "rclcpp/clock.hpp"
#include "rviz_common/render_panel.hpp"
#include "rviz_common/ros_integration/ros_node_abstraction.hpp"
#include "rviz_common/ros_integration/ros_client_abstraction.hpp"
#include "rviz_common/visualization_manager.hpp"
#include "rviz_rendering/render_window.hpp"



roundWindow::roundWindow(QApplication *app,QWidget * parent)
: app_(app),QMainWindow(parent)
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

  auto brush = QBrush(QColor(128,128,128,255)); 
	auto painter = QPainter(&maskImage_); 
	painter.setBrush(brush);
  painter.setPen(Qt::NoPen);

  theRect_ = theRect;
  auto workRect = theRect;
  workRect.moveCenter(QPoint(this->width()/2,this->height()/2));

  painter.drawEllipse(workRect);

  painter.end();

  maskPixmap_ = QPixmap();
  
  auto pixResult = maskPixmap_.convertFromImage(maskImage_);

  auto ros_node_abs = std::make_shared<rviz_common::ros_integration::RosNodeAbstraction>("camviz_ros_node");

  //auto app = QApplication::instance();

  //auto rvWidget = rvizWidget(app, ros_node_abs,dynamic_cast<QWidget&>(*this));

  //auto rvWidget = std::make_shared<rvizWidget>(qApp, ros_node_abs,this);

  auto rvWidget = new rvizWidget(qApp,ros_node_abs,this);

  rvWidget->setFixedSize(QSize(600,600));

  label1.setText("Hello, World!");
  QPalette palette;
  palette.setColor(QPalette::WindowText, Qt::GlobalColor::darkGreen);
  label1.setPalette(palette);
  label1.setFont({label1.font().family(), 34, QFont::Bold, true});
  label1.setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
  label1.setWordWrap(true);

  auto vBoxLayout = new QVBoxLayout;

  //rvWidget->setFixedSize(QSize(500,500));
  rvWidget->setGeometry(workRect);

  vBoxLayout->addWidget(rvWidget);
  vBoxLayout->setAlignment(Qt::AlignTop);

  auto insetRect = workRect.marginsRemoved(QMargins(100,100,100,100));

  vBoxLayout->setGeometry(insetRect);

  this->setLayout(vBoxLayout);

  this->setup
  


}

void roundWindow::setupManagers() {

 // Initialize the classes we need from rviz


  app_->processEvents();
  render_panel_ = new rviz_common::RenderPanel(this);
  
  app_->processEvents();
  render_panel_->getRenderWindow()->initialize();
  auto clock = rviz_ros_node_.lock()->get_raw_node()->get_clock();
  manager_ = new rviz_common::VisualizationManager(render_panel_, rviz_ros_node_, this, clock);
  

  render_panel_->initialize(manager_);
  app_->processEvents();
  manager_->initialize();
  manager_->startUpdate();

}

void roundWindow::paintEvent(QPaintEvent *) {

  auto qp = QPainter(this);
  qp.drawPixmap(QPoint(0,0),this->maskPixmap_);
  qp.end();


}

QSize roundWindow::sizeHint() const {

  return(QSize( theRect_.width(),theRect_.height() ));

}


void roundWindow::closeEvent(QCloseEvent * event)
{
  QWidget::closeEvent(event);
  rclcpp::shutdown();
}
