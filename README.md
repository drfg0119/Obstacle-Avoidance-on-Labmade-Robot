# Obstacle-Avoidance-on-Labmade-Robot
Path Following and Obstacle Avoidance by Deep Deterministic Policy Gradient

## Demonstration

###Gazebo

![image](https://github.com/drfg0119/Obstacle-Avoidance-on-Labmade-Robot/blob/main/gazebo_demo.gif)

###Lab-made-robot

![image](https://github.com/drfg0119/Obstacle-Avoidance-on-Labmade-Robot/blob/main/lab-made-robot_demo.gif)

You can resource your ros workspace in your bashrc :

    git clone https://github.com/drfg0119/Obstacle-Avoidance-on-Labmade-Robot.git
    cd Obstacle-Avoidance-on-Labmade-Robot/catkin_ws
    catkin_make

This project can be used to do DDPG training and verification with Simulink under Gazebo. 
Also, it can do verification on robot which equipped realsense depth camera and joystick. 
More details can be found in the [**oral presentation**](https://youtu.be/CSfS9ozWRXU) and [**thesis**](https://github.com/drfg0119/Obstacle-Avoidance-on-Labmade-Robot/blob/main/Path%20Following%20and%20Obstacle%20Avoidance%20by%20Deep%20Deterministic%20Policy%20Gradient_abstract&reference.pdf). 

## Dependency

### realsense-ros
### darknet_ros
### RTAB-Map
### joystick_drivers
### turtlebot3
### rosserial