# Sort-of portable dual realsense camera pointcloud vacuum.

## Description

I needed a somewhat mobile rig for pointcloud collection, and after exploring various provided in walled
gardens... Decided to put one together, using open software and workflows.

This is based around a nVidia xavier nx module, the carrier board is a seeed studio [A203](https://www.seeedstudio.com/A203-Carrier-Board-for-Jetson-Nano-Xavier-NX-V2-p-5214.html 'Seeed A203')



<img src="resources/IMG_4556.png" alt="usfsmax" width="200"/>

## Notes

- This is for the teensy 4.1 and lib_depends https://github.com/micro-ROS/micro_ros_platformio
- ROS distro is configured as iron
- teensy_41_colcon.meta is set up for 3 publishers, the board meta file is specified in platformio.ini

## Running the containerized uROS bridge

- transport is 'serial', at 6M bits. See microros docs to change transport
- command line -- docker run -it --rm -v /dev:/dev -v /dev/shm:/dev/shm --privileged --name yournamehere --net=host microros/micro-ros-agent:iron serial --dev /dev/ttyACM0 -v4 -b 6000000

## python docker api

```python

self.uros_docker_client = docker.from_env()
bridge_command = 'serial --dev /dev/ttyACM0 -v4 -b 6000000'
self.microros_container = self.uros_docker_client.containers.run('microros/micro-ros-agent:iron',volumes=['/dev:/dev', '/dev/shm:/dev/shm'],privileged=True,remove=True,name='yournamehere',network_mode='host',detach=True,command=bridge_command)

```
<img src="resources/docker_output.png" alt="usfsmax" width="600"/>

## TODO

[There is a lack of syncronization between the ros publisher timer and the usfsmax data update. I will update this as I test.]