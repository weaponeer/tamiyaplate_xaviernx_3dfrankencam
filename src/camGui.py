# importing libraries 
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
import os,sys, signal,psutil
import cv2,cv_bridge


from PyQt5.QtCore import pyqtSignal, pyqtSlot, Qt, QThread
import numpy as np

import rclpy
from rclpy.node import Node 
import sensor_msgs.msg as msg
import std_msgs.msg as std_msg
from rclpy.executors import MultiThreadedExecutor
from rclpy.qos import QoSProfile, ReliabilityPolicy, HistoryPolicy, DurabilityPolicy
from cv_bridge import CvBridge, CvBridgeError
from threading import Thread

import docker



class TestDisplayNode(Node):
	def __init__(self,widgetToDraw):
		super().__init__('ImageStream')
		self.__window_name = "depthSubscriber"
		self.updateWidget = widgetToDraw
		
		
		self.bridge = CvBridge()
		
		qos_profile = QoSProfile(
			reliability=ReliabilityPolicy.BEST_EFFORT,
			durability=DurabilityPolicy.TRANSIENT_LOCAL,
			history=HistoryPolicy.KEEP_LAST,
			depth=3
		)

		self.sub = self.create_subscription(msg.Image, '/camera/camera/depth/image_rect_raw', self.msg_callback, qos_profile=qos_profile)

	def print_header_info(self, m : std_msg.Header):
		print("received image stamp {}".format(m.frame_id))
		# print(m.format)

	def msg_callback(self, m : msg.Image):
		#self.print_header_info(m.header)
		foo = self.convert_depth_image(m)
		self.updateWidget.setFrame(foo)

	def display(self, img : np.ndarray):
		cv2.imshow(self.__window_name, cv2.cvtColor(img, cv2.COLOR_RGB2BGR))
		cv2.waitKey(1)
		
	def convert_image(self, ros_image):
		# Use cv_bridge() to convert the ROS image to OpenCV format
		try:
			cv_image = self.bridge.imgmsg_to_cv2(ros_image, "bgr8")       
			return np.array(cv_image, dtype=np.uint8)
		except CvBridgeError as e:
			print(e)
		  
	def convert_depth_image(self, ros_image):
		try:
			depth_image = self.bridge.imgmsg_to_cv2(ros_image, "32FC1")
			# Convert to a numpy array since this is what OpenCV 2.3 uses
			depth_image = np.array(depth_image, dtype=np.float32)
			return depth_image
		
		except CvBridgeError as e:
			print(e)


class RoundWindow(QWidget): 
	
	"""Simple window that shows our masked image and text label."""
	def __init__(self): 
		super().__init__() 

		# setting up the geometry
		# Size: 1080 x 1080
		# Available: 1008 x 1053
		self.setWindowFlags(Qt.FramelessWindowHint)
		self.setAttribute(Qt.WA_TranslucentBackground)
		# self.setMouseTracking(True)
        
		theMiddle = QDesktopWidget().availableGeometry().center()
		theRect = QDesktopWidget().availableGeometry()
  
		screenNum = QDesktopWidget().primaryScreen()
		catRect = QDesktopWidget().screenGeometry(screenNum)
		print(catRect)
		theRect = catRect
		theMiddle = catRect.center()
  
		self.setGeometry(theRect) 
  		#self.setGeometry(0, 0, 1008, 1053) 

  
		self.maskImage = QImage(theRect.width(), theRect.height(), QImage.Format_ARGB32)
		self.maskImage.fill(Qt.transparent)
  
		# Create a texture brush and paint a circle 
		# with the original image onto the output image: 
		brush = QBrush(QColor(0,0,0,255)) 
		# Paint the output image 
		painter = QPainter(self.maskImage) 
		painter.setBrush(brush) 

		# Don't draw an outline 
		painter.setPen(Qt.NoPen)
  
		workRect = theRect
  
		workRect.moveCenter(QPoint(self.width()/2,self.height()/2));

		# drawing circle 
		painter.drawEllipse(workRect) #theMiddle.x(), theMiddle.y(), 500, 500)
  
		# brushW = QBrush(QColor(0,128,0,255))
		# painter.setBrush(brushW)
		# painter.drawPie(workRect,-75.0 * 16.0,-15.0 * 16.0)

		# closing painter event 
		painter.end()

		self.theMiddle = theMiddle
		self.theRect = theRect
  
		self.windowPixmap = QPixmap()
  
		pixResult = self.windowPixmap.convertFromImage(self.maskImage)
  
		
		myBoxLayout = QVBoxLayout(self)
		
		label = QLabel()
		label.setPixmap( QPixmap() )
		#label.setPixmap(self.windowPixmap)
		label.resize(100, 100)
		
		# myBoxLayout.addWidget(label)
		
		#myBoxLayout.addStretch()
        
		BottomLayout = QHBoxLayout()
		#myBoxLayout.addStretch()
		#myBoxLayout.addStretch()

		#myBoxLayout.setAlignment(Qt.AlignTop)
		self.count=0
		myBoxLayout.addLayout(BottomLayout)
		self.Button_01 = QPushButton("Start")
		self.Button_02 = QPushButton("Stop")
		self.Button_03 = QPushButton("Shutdown")
		
		self.Button_01.setFixedSize(120, 60)
		self.Button_02.setFixedSize(120, 60)
		self.Button_03.setFixedSize(120, 60)
		self.Button_01.setStyleSheet("background-color:green;")
		self.Button_02.setStyleSheet("background-color:grey;")
		self.Button_03.setStyleSheet("background-color:pink;")
		
		self.Button_01.clicked.connect(self.start_click)
		self.Button_02.clicked.connect(self.stop_click)

		BottomLayout.addWidget(self.Button_01)
		BottomLayout.addWidget(self.Button_02)
		BottomLayout.addWidget(self.Button_03)
		self.setLayout(myBoxLayout)
		
		self.depthSubscriber = TestDisplayNode(self)
		self.mainExecutor = MultiThreadedExecutor()
		self.mainExecutor.add_node(self.depthSubscriber)

		# Start the ROS2 node on a separate thread
		self.rosThread = Thread(target=self.mainExecutor.spin)
		self.rosThread.start()
		self.depthSubscriber.get_logger().info("Spinned ROS2 Node . . .")
		
		self.captureProcess = None
		self.microros_bridge = None
		self.start_microros()
		
		@pyqtSlot(np.ndarray)
		def update_image(self, cv_img):
			"""Updates the image_label with a new opencv image"""
			qt_img = self.convert_cv_qt(cv_img)
			self.image_label.setPixmap(qt_img)
		
		def convert_cv_qt(self, cv_img):
			"""Convert from an opencv image to QPixmap"""
			rgb_image = cv2.cvtColor(cv_img, cv2.COLOR_BGR2RGB)
			h, w, ch = rgb_image.shape
			bytes_per_line = ch * w
			convert_to_Qt_format = QtGui.QImage(rgb_image.data, w, h, bytes_per_line, QtGui.QImage.Format_RGB888)
			p = convert_to_Qt_format.scaled(self.disply_width, self.display_height, Qt.KeepAspectRatio)
			return QPixmap.fromImage(p)
		
	def message(self,message):
		print(message)
		
	def setFrame(self,newFrame):
		foo = None
		# print(newFrame)
		
	def stop_rosthread(self):
		self.rosThread.join()
		self.mainExecutor.shutdown()
		self.depthSubscriber.destroy_node()
		print('rosThread complete...')

	def stop_click(self):
		
		if self.captureProcess is not None:
			
			killPid = self.captureProcess.pid()
			# get rid of node zombies
			parent = psutil.Process(killPid)
			print('parent', parent)
			for child in parent.children(recursive=True):
				print('child',child)
				child.terminate()
				
			self.captureProcess.terminate()
			self.captureProcess.waitForFinished()
			self.capturePtrocess = None
			
	def closeEvent(self,event):
		self.microros_container.stop()
		
	def stop_microros(self):
		self.microros_container.stop()
		
	def start_microros(self):
		
		self.uros_docker_client = docker.from_env()
		bridge_command = 'serial --dev /dev/ttyACM0 -v4 -b 6000000'
		self.microros_container = self.uros_docker_client.containers.run('microros/micro-ros-agent:iron',volumes=['/dev:/dev', '/dev/shm:/dev/shm'],privileged=True,remove=True,name='poodleros',network_mode='host',detach=True,command=bridge_command)
		
		
		
		# self.microros_bridge = QProcess()
		# self.microros_bridge.stateChanged.connect(self.handle_uros_state)
		# self.microros_bridge.readyReadStandardOutput.connect(self.handle_uros_stdout)
		# self.microros_bridge.readyReadStandardError.connect(self.handle_uros_stderr)
		# self.microros_bridge.finished.connect(self.process_uros_finished)  # Clean up once complete.
		
		# docker run -it --rm -v /dev:/dev -v /dev/shm:/dev/shm --privileged --name poodleros --net=host microros/micro-ros-agent:iron serial --dev /dev/ttyACM0 -v4 -b 6000000
		
		#self.microros_bridge.start("docker", ['run','-it','--rm',
		#									'-v /dev:/dev', '-v /dev/shm:/dev/shm','--privileged','--name poodleros','--net=host ', 'microros/micro-ros-agent:iron', '--dev /dev/ttyACM0','-v4','-b 6000000'])
											
		#self.microros_bridge.start("docker", ['run','--rm',
		#									'-v','/dev:/dev', '-v', '/dev/shm:/dev/shm','--privileged','--net=host', '--name','poodleros','microros/micro-ros-agent:iron', 'serial', '--dev', '/dev/ttyACM0', '-v4', '-b', '6000000'])
											
		# print(self.microros_bridge.arguments())
		
				
	def start_click(self):
		self.count += 1
		self.Button_01.setText(f"Click Count: {self.count}")
		
		if self.captureProcess is None:
		
			self.captureProcess = QProcess()
			self.captureProcess.stateChanged.connect(self.handle_state)
			self.captureProcess.readyReadStandardOutput.connect(self.handle_stdout)
			self.captureProcess.readyReadStandardError.connect(self.handle_stderr)
			self.captureProcess.finished.connect(self.process_finished)  # Clean up once complete.
	
			
	
			
			# ros2 launch realsense2_camera rs_multi_camera_launch.py depth_module.profile2:=640x480x90 depth_module.profile1:=640x480x90 pointcloud.enable1:=true pointcloud.enable2:=true serial_no1:=_923322070838 serial_no2:=_126122270529 unite_imu_method:=1 enable_accel:=true enable_gyro:=true
			#self.captureProcess.start("ros2", ['launch','realsense2_camera','rs_multi_camera_launch.py',
			#								'depth_module.profile2:=640x480x90','depth_module.profile1:=640x480x90','pointcloud.enable1:=true ','pointcloud.enable2:=true',
			#								'serial_no1:=_923322070838', 'serial_no2:=_126122270529', 'unite_imu_method:=1', 'enable_accel:=true', 'enable_gyro:=true'])
											
			
			self.captureProcess.start("ros2", ['launch','realsense2_camera','rs_launch.py',
											'depth_module.profile:=640x480x90','pointcloud.enable:=true ','serial_no:=_923322070838','unite_imu_method:=1', 'enable_accel:=true', 'enable_gyro:=true'])
											
			# self.captureProcess.start("bash", ['dummy_script.bash'])

	def handle_stderr(self):
		data = self.captureProcess.readAllStandardError()
		stderr = bytes(data).decode("utf8")
		self.message(stderr)

	def handle_stdout(self):
		data = self.captureProcess.readAllStandardOutput()
		stdout = bytes(data).decode("utf8")
		self.message(stdout)

	def handle_state(self, state):
		states = {
			QProcess.NotRunning: 'Not running',
			QProcess.Starting: 'Starting',
			QProcess.Running: 'Running',
			}
			
		state_name = states[state]
		self.message(f"State changed: {state_name}")
			
		if state == QProcess.NotRunning:
			self.Button_01.setStyleSheet("background-color:green;")
			self.Button_02.setStyleSheet("background-color:gray;")
			self.Button_03.setStyleSheet("background-color:pink;")
			self.message(f"color changed: {state_name}")
		elif state == QProcess.Running:
			self.Button_01.setStyleSheet("background-color:yellow;")
			self.Button_02.setStyleSheet("background-color:red;")
			self.Button_03.setStyleSheet("background-color:pink;")
			self.message(f"color changed: {state_name}")
				
		
		

	def process_finished(self):
		self.message("Process finished.")
		self.captureProcess = None
		
	def process_uros_finished(self):
		self.message("uRos process finished.")
		self.microros_bridge = None
		
	def handle_uros_stderr(self):
		data = self.microros_bridge.readAllStandardError()
		stderr = bytes(data).decode("utf8")
		self.message('uRos' + stderr)

	def handle_uros_stdout(self):
		data = self.microros_bridge.readAllStandardOutput()
		stdout = bytes(data).decode("utf8")
		self.message(stdout)

	def handle_uros_state(self, state):
		states = {
			QProcess.NotRunning: 'Not running',
			QProcess.Starting: 'Starting',
			QProcess.Running: 'Running',
			}
		
		state_name = states[state]
		self.message(f"uRos state changed: {state_name}")
		
        
        

   
	def paintEvent(self, event):
		qp = QPainter()
		qp.begin(self)
		qp.drawPixmap(QPoint(0, 0), self.windowPixmap)
		qp.end()
	def sizeHint(self):
		return QSize(self.theRect.width(), self.theRect.height())  # Set this to the exact image resolution

	def mousePressEvent(self, event):
		self.oldPos = event.globalPos()

	def mouseMoveEvent(self, event):
		delta = QPoint(event.globalPos() - self.oldPos)
		self.move(self.x() + delta.x(), self.y() + delta.y())
		self.oldPos = event.globalPos()
		app = QApplication.instance()
		print(self.oldPos)
	
		app.exit(0)



# main function 
if __name__ == '__main__': 
	import sys 
	from PyQt5.QtWidgets import QApplication
	
	rclpy.init(args=None)

	

	# app created 
	app = QApplication(sys.argv)
	screen = app.primaryScreen()
	print('Screen: %s' % screen.name())
	size = screen.size()
	print('Size: %d x %d' % (size.width(), size.height()))
	rect = screen.availableGeometry()
	print('Available: %d x %d' % (rect.width(), rect.height()))
	w = RoundWindow() 
	#w.show() 
	w.showFullScreen() 

	# begin the app 
	retFoo = app.exec_()
	rclpy.shutdown()
	w.stop_microros()
	w.stop_rosthread()
	w.stop_click()
	print('Exiting')
	sys.exit(retFoo)  



