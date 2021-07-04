#ifndef DATA_CONTROL_H_
#define DATA_CONTROL_H_

/****************************************************************
 * Constants
 ****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <string>
#include <termios.h>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sstream>
#include <iterator>
#include <array>
#include <math.h>
#include <cstdlib>
#include <ctime>

/**************ROS**************/
#include "ros/ros.h"
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include "std_msgs/UInt8MultiArray.h"
#include "std_msgs/UInt8.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Int8MultiArray.h"
#include "std_msgs/Int16.h"
#include "std_msgs/Int32.h"
#include "sensor_msgs/JointState.h"
#include "std_msgs/Int16MultiArray.h"
#include "cv_bridge/cv_bridge.h"
#include "sensor_msgs/Image.h"
#include <image_transport/image_transport.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>
#include <gazebo_msgs/LinkStates.h>
#include <gazebo_msgs/ModelStates.h>
#include <gazebo_msgs/SetModelState.h>
#include <relative_angle_nb/Apl.h>
#include <relative_angle_nb/Apl_kai.h>
#include <geometry_msgs/Pose.h>

/**********DARKNET_ROS*********/
#include <darknet_ros_msgs/BoundingBoxes.h>
#include <darknet_ros_msgs/BoundingBox.h>

/**************opencv************/
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

//IO GPIO
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64
#define Object_Num_Max 10

//laser angle
#define Cam_High 575// 715 //mm
#define Laser_High 680//820 //mm
#define Laser_Sin_Rotate 0//(-0.045) //(0.005) //0.0174
#define Laser_Cos_Rotate 1//0.9989 //(0.99998) //0.9998
#define Laser_W_Offset 0 //(-5) //-46 //mm
#define Laser_H_Offset 38 //56 //55.75 //mm
#define Laser_W_Offset_Angle 0.013 //0.040 //0.046
#define Laser_W_Offset_Pixel 0 //(-5) //-46
#define Laser_H_Offset_Pixel 0 //55.75 //mm
#define Laser_Votage_Radio_y 1 //0.95
#define Laser_Votage_Radio_x 1 //0.95
#define Distance_Motor2Laser  40  //mm
#define Distance_Laser2Depth 31 //31 //mm

#if 0
/*******SR305 H68 V41.5***********/
#define Cam_FOV_H 52 //52 //54(54.31) //68
#define Cam_FOV_V 41.5//42.2 //44(42)
#define Cam_H 640
#define Cam_V 480
#define Cam_H_Center (Cam_H/2)
#define Cam_V_Center (Cam_V/2)
#define PI 3.1416

#elif 1
/*******D435i H87 V58***********/
#define Cam_FOV_H 70.33 //52 //54(54.31) //68
#define Cam_FOV_V 43.54//42.2 //44(42)
#define Cam_H 1280
#define Cam_V 720
#define Cam_H_Center (Cam_H/2)
#define Cam_V_Center (Cam_V/2)
#define PI 3.1416

#else
/*******R200 in Turtlebot3***********/
#define Cam_FOV_H 59 //52 //54(54.31) //68
#define Cam_FOV_V 46//42.2 //44(42)
#define Cam_H 1920
#define Cam_V 1080
#define Cam_H_Center (Cam_H/2)
#define Cam_V_Center (Cam_V/2)
#define PI 3.1416

#endif


#define X_Pixel_Size Cam_High*tan(Cam_FOV_H*PI/360)/Cam_H_Center //mm
#define Y_Pixel_Size Cam_High*tan(Cam_FOV_V*PI/360)/Cam_V_Center //mm

#define Object_Num_Max 10
#define X_Destination 5.0
#define Y_Destination 5.0
typedef struct{
	unsigned char object_num;
	unsigned int x_center;
	unsigned int y_center;
	unsigned int x_min;
	unsigned int y_min;
	unsigned int x_max;
	unsigned int y_max;
	unsigned int x_length;
	unsigned int depth;
	float h_angle;
	float v_angle;
	int h_distance;
}BOX_;

typedef struct{
	BOX_ BOX_OBSTACLE[Object_Num_Max];
	unsigned int nearest_depth;
	float obs_direction;
	int h_dist_between_obsandrobot;
	unsigned char flg;
	unsigned char total_num;
	float odom_diff;
	float x_goal;
	float y_goal;
	float linear_vel;
	float angular_vel;
	float current_abs_robot_z_pose;
}DETECT_BOX_;

extern DETECT_BOX_ DETECT_BOX;

/****************function**************/
void Sub_Msgdata(const std_msgs::Int8::ConstPtr& msg);
void Image_Boxes_Data_Read_Laser(const darknet_ros_msgs::BoundingBoxes::ConstPtr& msg);
void Depth_Image(const sensor_msgs::ImageConstPtr& msg);
void Depth_Image_rl(const sensor_msgs::ImageConstPtr& msg);
void Find_BboxMidXYposition_Sequence(const darknet_ros_msgs::BoundingBoxes::ConstPtr& msg);
void Sub_and_Dealwith_Odomdata(const nav_msgs::Odometry::ConstPtr &msg);
void Sub_Respawned_Goal_X(const std_msgs::Int8::ConstPtr& msg);
void Sub_Respawned_Goal_Y(const std_msgs::Int8::ConstPtr& msg);
void Check_if_Reset_Rosbot_Pose(const gazebo_msgs::ModelStates::ConstPtr& msg);
//void Find_Model_Name(const gazebo_msgs::ModelStates::ConstPtr &msg);
#endif // JETSONGPIO_H_


















