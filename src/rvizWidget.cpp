#include "roundRviz/rvizWidget.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QBrush>


#include "rclcpp/clock.hpp"
#include "rviz_common/render_panel.hpp"
#include "rviz_common/ros_integration/ros_node_abstraction.hpp"
#include "rviz_common/visualization_manager.hpp"
#include "rviz_rendering/render_window.hpp"



rvizWidget::rvizWidget(
  QApplication * app,
  rviz_common::ros_integration::RosNodeAbstractionIface::WeakPtr rviz_ros_node,
  QWidget * parent)
: app_(app), rviz_ros_node_(rviz_ros_node), QWidget(parent)
{
  // prep from round display
  this->setWindowFlags(Qt::FramelessWindowHint);
  this->setAttribute(Qt::WA_TranslucentBackground);
  desktop_ = QApplication::desktop();
  //auto theMiddle = desktop_->availableGeometry().center();
  //auto theRect = desktop_->availableGeometry();
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

  // Construct the layout
  QLabel * thickness_label = new QLabel("Line Thickness");
  QSlider * thickness_slider = new QSlider(Qt::Horizontal);
  thickness_slider->setMinimum(1);
  thickness_slider->setMaximum(100);
  QLabel * cell_size_label = new QLabel("Cell Size");
  QSlider * cell_size_slider = new QSlider(Qt::Horizontal);
  cell_size_slider->setMinimum(1);
  cell_size_slider->setMaximum(100);
  QGridLayout * controls_layout = new QGridLayout();
  controls_layout->addWidget(thickness_label, 0, 0);
  controls_layout->addWidget(thickness_slider, 0, 1);
  controls_layout->addWidget(cell_size_label, 1, 0);
  controls_layout->addWidget(cell_size_slider, 1, 1);

  // Add visualization
  main_layout = new QVBoxLayout;
  main_layout->addLayout(controls_layout);
  central_widget = new QWidget();
  main_layout->setSpacing(0);
  main_layout->setMargin(0);

  // Initialize the classes we need from rviz
  initializeRViz();

  central_widget->setLayout(main_layout);
  //setCentralWidget(central_widget);
  main_layout->addWidget(render_panel_);

  // Signals
  connect(thickness_slider, SIGNAL(valueChanged(int)), this, SLOT(setThickness(int)));
  connect(cell_size_slider, SIGNAL(valueChanged(int)), this, SLOT(setCellSize(int)));

  // Display the rviz grid plugin
  DisplayGrid();

  // Intialize the sliders
  thickness_slider->setValue(25);
  cell_size_slider->setValue(10);
}

void rvizWidget::paintEvent(QPaintEvent *) {

  auto qp = QPainter(this);
  qp.drawPixmap(QPoint(0,0),this->maskPixmap_);

}

QWidget *
rvizWidget::getParentWindow()
{
  return this;
}

rviz_common::PanelDockWidget *
rvizWidget::addPane(const QString & name, QWidget * pane, Qt::DockWidgetArea area, bool floating)
{
  // TODO(mjeronimo)
  return nullptr;
}

void
rvizWidget::setStatus(const QString & message)
{
  // TODO(markc)
}

void rvizWidget::DisplayGrid()
{
  grid_ = manager_->createDisplay("rviz_default_plugins/Grid", "adjustable grid", true);
  assert(grid_ != NULL);
  grid_->subProp("Line Style")->setValue("Billboards");
  grid_->subProp("Color")->setValue(QColor(Qt::yellow));

  //camera_ = manager_->createDisplay("rviz_default_plugins/Camera", "camera view", true);
  //assert(camera_ != NULL);
  //camera_->subProp("Line Style")->setValue("Billboards");
  //camera_->subProp("Color")->setValue(QColor(Qt::yellow));
  //camera_->subProp("Image")->setValue("/camera/camera/color/image_raw");
}

void rvizWidget::initializeRViz()
{
  app_->processEvents();
  render_panel_ = new rviz_common::RenderPanel(central_widget);
  app_->processEvents();
  render_panel_->getRenderWindow()->initialize();
  auto clock = rviz_ros_node_.lock()->get_raw_node()->get_clock();
  manager_ = new rviz_common::VisualizationManager(render_panel_, rviz_ros_node_, this, clock);
  render_panel_->initialize(manager_);
  app_->processEvents();
  manager_->initialize();
  manager_->startUpdate();
}

void rvizWidget::setThickness(int thickness_percent)
{
  if (grid_ != NULL) {
    grid_->subProp("Line Style")->subProp("Line Width")->setValue(thickness_percent / 100.0f);
  }
}

void rvizWidget::setCellSize(int cell_size_percent)
{
  if (grid_ != NULL) {
    grid_->subProp("Cell Size")->setValue(cell_size_percent / 10.0f);
  }
}

void rvizWidget::closeEvent(QCloseEvent * event)
{
  QWidget::closeEvent(event);
  rclcpp::shutdown();
}
