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
#include "rviz_common/properties/property_tree_model.hpp"



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
  auto fooOut = new QGridLayout();
  auto famOut = new QVBoxLayout();
  // main_layout->addLayout(controls_layout);
  central_widget = new QWidget();

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



  // Initialize the classes we need from rviz
  
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
  

  //central_widget->setLayout(main_layout);
  fooOut->addWidget(label1,2,2,1,1);
  fooOut->addWidget(label2,1,1,1,1);
  fooOut->addWidget(label3,3,3,1,1);
  //fooOut->addWidget(render_panel_,0,0,1,4);

  famOut->addWidget(render_panel_);
  //this->setLayout(fooOut);
  this->setLayout(famOut);


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
  //grid_ = manager_->createDisplay("rviz_default_plugins/Grid", "adjustable grid", true);
  //assert(grid_ != NULL);
  //grid_->subProp("Line Style")->setValue("Billboards");
  //grid_->subProp("Color")->setValue(QColor(Qt::yellow));

  camera_ = manager_->createDisplay("rviz/Image", "image view", true);

  auto foo = camera_->getModel();

  assert(camera_ != NULL);
  //camera_->subProp("Line Style")->setValue("Billboards");
  //camera_->subProp("Color")->setValue(QColor(Qt::yellow));
  camera_->subProp("Topic")->setValue("/camera/camera/depth/image_rect_raw");

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
