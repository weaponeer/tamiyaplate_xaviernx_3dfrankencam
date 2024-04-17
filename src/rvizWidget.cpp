#include "roundRviz/rvizWidget.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QBrush>


#include "rclcpp/clock.hpp"
#include "rviz_common/render_panel.hpp"
#include "rviz_common/ros_integration/ros_node_abstraction.hpp"
#include "rviz_common/visualization_manager.hpp"
#include "rviz_common/display_context.hpp"
#include "rviz_common/display_group.hpp"

#include "rviz_rendering/render_window.hpp"
#include "rviz_common/properties/property_tree_model.hpp"

#include "rviz_common/panel_dock_widget.hpp"



rvizWidget::rvizWidget(
  QApplication * app,
  rviz_common::ros_integration::RosNodeAbstractionIface::WeakPtr rviz_ros_node,
  QWidget * parent)
: app_(app), rviz_ros_node_(rviz_ros_node), QWidget(parent)
{
 
  // Add visualization
  mainDockLayout_ = new QGridLayout();
  mainBoxLayout_ = new QHBoxLayout();
  central_widget_ = new QWidget();

  setupManagers(this);

  //central_widget->setLayout(mainBoxLayout_);
  
  
  
  mainDockWiget_ =  new QDockWidget(QLatin1String("Panels"));
  mainDockWiget_->setFixedSize(QSize(500,500));
  mainBoxLayout_->addWidget(mainDockWiget_);
  mainBoxLayout_->addWidget(render_panel_);
  auto tempWidget = new QWidget();
  tempWidget->setLayout(mainDockLayout_);
  mainDockWiget_->setWidget(tempWidget);

  this->setLayout(mainBoxLayout_);
  
  setupDisplays();

}


void rvizWidget::setupManagers(QWidget *parent) {

 // Initialize the classes we need from rviz


  app_->processEvents();
  render_panel_ = new rviz_common::RenderPanel(parent);
  
  app_->processEvents();
  render_panel_->getRenderWindow()->initialize();
  auto clock = rviz_ros_node_.lock()->get_raw_node()->get_clock();
  manager_ = new rviz_common::VisualizationManager(render_panel_, rviz_ros_node_, this, clock);
  

  render_panel_->initialize(manager_);
  app_->processEvents();
  manager_->initialize();
  manager_->startUpdate();

}

void rvizWidget::setupDisplays() {

  grid_ = manager_->createDisplay("rviz_default_plugins/Grid", "adjustable grid", true);
  assert(grid_ != NULL);
  grid_->subProp("Line Style")->setValue("Billboards");
  grid_->subProp("Color")->setValue(QColor(Qt::yellow));

  camera_ = manager_->createDisplay("rviz_default_plugins/Image", "image view", true);
  assert(camera_ != NULL);
  //camera_->subProp("Line Style")->setValue("Billboards");
  //camera_->subProp("Color")->setValue(QColor(Qt::yellow));
  camera_->subProp("Topic")->setValue("/camera/camera/depth/image_rect_raw");
  camera_->subProp("Normalize Range")->setValue(true);

  camera_->subProp("Min Value")->setValue(0);
  camera_->subProp("Max Value")->setValue(1);
  camera_->subProp("Median window")->setValue(5);

  // 2nd view -- // /camera/camera/color/image_raw
  camera2_ = manager_->createDisplay("rviz_default_plugins/Image", "image view", true);
  assert(camera_ != NULL);
  //camera_->subProp("Line Style")->setValue("Billboards");
  //camera_->subProp("Color")->setValue(QColor(Qt::yellow));
  camera2_->subProp("Topic")->setValue("/camera/camera/color/image_raw");
  camera2_->subProp("Normalize Range")->setValue(true);

  camera2_->subProp("Min Value")->setValue(0);
  camera2_->subProp("Max Value")->setValue(1);
  camera2_->subProp("Median window")->setValue(5);

  Imu_ = manager_->createDisplay("rviz_imu_plugin/Imu", "Imu", true);
  assert(Imu_ != NULL);
  Imu_->subProp("Topic")->setValue("/imu/data");
  Imu_->subProp("Color")->setValue(QColor(Qt::yellow));

  auto rdg = manager_->getRootDisplayGroup();
  auto globalOptions = rdg->subProp("Global Options");
  auto globalFrame = globalOptions->subProp("Fixed Frame");

  globalFrame->setValue("odom");

  /*debug useful
  auto hamlang = globalFrame->getDescription();
  std::cout << hamlang.toStdString() << std::endl;
  auto theval = globalFrame->getValue();
  std::cout << theval.toString().toStdString() << std::endl;
  */

}


QWidget *
rvizWidget::getParentWindow()
{
  return this;
}


rviz_common::PanelDockWidget * rvizWidget::addPane(
  const QString & name, QWidget * panel,
  Qt::DockWidgetArea area, bool floating)
{
  rviz_common::PanelDockWidget * dock;
  dock = new rviz_common::PanelDockWidget(name);
  //dock->setFixedSize(QSize(200,200));
  dock->setContentWidget(panel);
  dock->setFloating(floating);
  dock->setObjectName(name);   // QMainWindow::saveState() needs objectName to be set.
  mainDockLayout_->addWidget(dock);

  /*

  // we want to know when that panel becomes visible
  connect(dock, SIGNAL(visibilityChanged(bool)), this, SLOT(onDockPanelVisibilityChange(bool)));
  connect(this, SIGNAL(fullScreenChange(bool)), dock, SLOT(overrideVisibility(bool)));

  QAction * toggle_action = dock->toggleViewAction();
  view_menu_->addAction(toggle_action);

  connect(toggle_action, SIGNAL(triggered(bool)), this, SLOT(setDisplayConfigModified()));
  connect(dock, SIGNAL(closed()), this, SLOT(setDisplayConfigModified()));

  dock->installEventFilter(geom_change_detector_);

  // repair/update visibility status
  hideLeftDock(area == Qt::LeftDockWidgetArea ? false : hide_left_dock_button_->isChecked());
  hideRightDock(area == Qt::RightDockWidgetArea ? false : hide_right_dock_button_->isChecked());

  */

  return dock;
}



void
rvizWidget::setStatus(const QString & message)
{
  // TODO(markc)
}


void rvizWidget::initializeRViz()
{
  
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
