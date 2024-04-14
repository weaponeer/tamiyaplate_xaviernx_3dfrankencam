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

#include "rviz_rendering/render_window.hpp"
#include "rviz_common/properties/property_tree_model.hpp"

#include "rviz_common/panel_dock_widget.hpp"



rvizWidget::rvizWidget(
  QApplication * app,
  rviz_common::ros_integration::RosNodeAbstractionIface::WeakPtr rviz_ros_node,
  QWidget * parent)
: app_(app), rviz_ros_node_(rviz_ros_node), QWidget(parent)
{
  
/*
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
  */

  // Add visualization
  mainDockLayout_ = new QGridLayout();
  mainBoxLayout_ = new QVBoxLayout();
  central_widget = new QWidget();

  #ifdef LOACH
  // main_layout->addLayout(controls_layout);
  

  auto label1 = new QLabel(this);

  label1->setText("---Hello, World!");
  QPalette palette;
  palette.setColor(QPalette::WindowText, Qt::GlobalColor::darkGreen);
  label1->setPalette(palette);
  label1->setFont({label1->font().family(), 34, QFont::Bold, true});
  label1->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
  label1->setWordWrap(true);
  //label1->setFixedSize(QSize(200,200));

  auto label2 = new QLabel(label1);
  auto label3 = new QLabel(label1);

  label2->setText("Hello, World! 2");
  label3->setText("Hello, World! 3");

  label2->setPalette(palette);
  label2->setFont({label1->font().family(), 34, QFont::Bold, true});
  label2->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
  label2->setWordWrap(true);
  //label2->setFixedSize(QSize(200,200));

  label3->setPalette(palette);
  label3->setFont({label1->font().family(), 34, QFont::Bold, true});
  label3->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
  label3->setWordWrap(true);
  //label3->setFixedSize(QSize(200,200));



  #endif 
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

  //central_widget->setLayout(mainBoxLayout_);
  
#ifdef LOACH
  
  fooOut->addWidget(label1,2,2,1,1);
  fooOut->addWidget(label2,1,1,1,1);
  fooOut->addWidget(label3,3,3,1,1);
  //fooOut->addWidget(render_panel_,0,0,1,4);
#endif

  mainBoxLayout_->addWidget(render_panel_);
  mainDockWiget_ =  new QDockWidget(QLatin1String("Panels"));
  mainBoxLayout_->addWidget(mainDockWiget_);
  auto tempWidget = new QWidget();
  tempWidget->setLayout(mainDockLayout_);
  mainDockWiget_->setWidget(tempWidget);

  //this->setLayout(fooOut);
  this->setLayout(mainBoxLayout_);


  //main_layout->addWidget(render_panel_);

  // Signals
  //connect(thickness_slider, SIGNAL(valueChanged(int)), this, SLOT(setThickness(int)));
  //connect(cell_size_slider, SIGNAL(valueChanged(int)), this, SLOT(setCellSize(int)));

  // Display the rviz grid plugin
  DisplayGrid();

  // Intialize the sliders
  //thickness_slider->setValue(25);
  //cell_size_slider->setValue(10);
}


QWidget *
rvizWidget::getParentWindow()
{
  return this;
}


/*
rviz_common::PanelDockWidget *
rvizWidget::addPane(const QString & name, QWidget * pane, Qt::DockWidgetArea area, bool floating)
{
  // TODO(mjeronimo)
  auto catfoo = area;




  return nullptr;
}
*/

rviz_common::PanelDockWidget * rvizWidget::addPane(
  const QString & name, QWidget * panel,
  Qt::DockWidgetArea area, bool floating)
{
  rviz_common::PanelDockWidget * dock;
  dock = new rviz_common::PanelDockWidget(name);
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

void rvizWidget::DisplayGrid()
{

  grid_ = manager_->createDisplay("rviz_default_plugins/Grid", "adjustable grid", true);
  assert(grid_ != NULL);
  grid_->subProp("Line Style")->setValue("Billboards");
  grid_->subProp("Color")->setValue(QColor(Qt::yellow));

  camera_ = manager_->createDisplay("rviz_default_plugins/Image", "image view", true);

  auto foo = camera_->getModel();

  assert(camera_ != NULL);
  //camera_->subProp("Line Style")->setValue("Billboards");
  //camera_->subProp("Color")->setValue(QColor(Qt::yellow));
  camera_->subProp("Topic")->subProp("Value")->setValue("/camera/camera/depth/image_rect_raw");
  camera_->subProp("Normalize Range")->setValue(true);

  camera_->subProp("Min Value")->setValue(0);
  camera_->subProp("Max Value")->setValue(1);
  camera_->subProp("Median window")->setValue(5);

  auto larb = 10.0;


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
