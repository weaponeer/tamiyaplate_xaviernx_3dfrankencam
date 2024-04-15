#include "roundRviz/roundWindow.hpp"
#include "roundRviz/rvizWidget.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QBrush>
#include <QPushButton>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>


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
  ros_client_abstraction_ = std::make_unique<rviz_common::ros_integration::RosClientAbstraction>();

  //auto app = QApplication::instance();

  //auto rvWidget = rvizWidget(app, ros_node_abs,dynamic_cast<QWidget&>(*this));

  //auto rvWidget = std::make_shared<rvizWidget>(qApp, ros_node_abs,this);

  
/*
  label1.setText("Hello, World!");
  QPalette palette;
  palette.setColor(QPalette::WindowText, Qt::GlobalColor::darkGreen);
  label1.setPalette(palette);
  label1.setFont({label1.font().family(), 34, QFont::Bold, true});
  label1.setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
  label1.setWordWrap(true);
*/

  central_widget_ = new QWidget();
  central_widget_->setFixedSize(QSize(1000,1000));
  

  auto rvWidget = new rvizWidget(qApp,ros_node_abs,central_widget_);
  auto vBoxLayout = new QVBoxLayout();


  ////rvWidget->setFixedSize(QSize(500,500));
  //rvWidget->setGeometry(workRect);

  vBoxLayout->setAlignment(Qt::AlignCenter);

  auto insetRect = workRect.marginsRemoved(QMargins(300,300,300,300));

  //vBoxLayout->setGeometry(insetRect);

  
  QHBoxLayout * buttonLayout = new QHBoxLayout();
  startButton_ = new QPushButton("Start");
  stopButton_ = new QPushButton("Stop");
  shutdownButton_ = new QPushButton("Shutdown");

  startButton_->setStyleSheet("background-color:green;");
	stopButton_->setStyleSheet("background-color:grey;");
	shutdownButton_->setStyleSheet("background-color:pink;");

  insetRect = workRect.marginsRemoved(QMargins(300,300,300,400));
  
  central_widget_->setGeometry(insetRect);

  startButton_->setFixedSize(QSize(150,100));
  stopButton_->setFixedSize(QSize(150,100));
  shutdownButton_->setFixedSize(QSize(150,100));

  buttonLayout->addWidget(startButton_);
  buttonLayout->addWidget(stopButton_);
  buttonLayout->addWidget(shutdownButton_);
  //vBoxLayout->addWidget();
  buttonLayout->setAlignment(Qt::AlignHCenter);
  //buttonLayout->setGeometry(insetRect);

  vBoxLayout->addWidget(rvWidget);

  vBoxLayout->addLayout(buttonLayout);

  central_widget_->setLayout(vBoxLayout);

  //vBoxLayout->addLayout(buttonLayout);
  
  //vBoxLayout->addWidget(central_widget_);
  this->setCentralWidget(central_widget_);
  //this->setCentralWidget(startButton);

}


QWidget *
roundWindow::getParentWindow()
{
  return central_widget_;
}


rviz_common::PanelDockWidget * roundWindow::addPane(
  const QString & name, QWidget * panel,
  Qt::DockWidgetArea area, bool floating)
{
  rviz_common::PanelDockWidget * dock;
  dock = new rviz_common::PanelDockWidget(name);
  dock->setContentWidget(panel);
  dock->setFloating(floating);
  dock->setObjectName(name);   // QMainWindow::saveState() needs objectName to be set.
  this->addDockWidget(Qt::BottomDockWidgetArea,dock);

  

  // we want to know when that panel becomes visible
  connect(dock, SIGNAL(visibilityChanged(bool)), this, SLOT(onDockPanelVisibilityChange(bool)));
  connect(this, SIGNAL(fullScreenChange(bool)), dock, SLOT(overrideVisibility(bool)));

  //QAction * toggle_action = dock->toggleViewAction();
  //view_menu_->addAction(toggle_action);

  //connect(toggle_action, SIGNAL(triggered(bool)), this, SLOT(setDisplayConfigModified()));
  connect(dock, SIGNAL(closed()), this, SLOT(setDisplayConfigModified()));

  //dock->installEventFilter(geom_change_detector_);

  // repair/update visibility status
  //hideLeftDock(area == Qt::LeftDockWidgetArea ? false : hide_left_dock_button_->isChecked());
  //hideRightDock(area == Qt::RightDockWidgetArea ? false : hide_right_dock_button_->isChecked());

  

  return dock;
}




void
roundWindow::setStatus(const QString & message)
{
  // TODO(markc)
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
