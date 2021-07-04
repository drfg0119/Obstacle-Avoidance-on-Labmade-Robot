
#include <geometry_msgs/Twist.h>
#include "data_control.h"
#include <std_msgs/UInt16.h>
#include <std_msgs/Float32.h>

DETECT_BOX_ DETECT_BOX;

ros::ServiceClient set_model_state_client;
gazebo_msgs::SetModelState setmodelstate;
gazebo_msgs::ModelState model;
geometry_msgs::Pose pose;


int main (int argc, char** argv){
	///****************init ros****************///
	ros::init(argc, argv, "dynamixel_node");
	ros::NodeHandle n;
	ros::Rate loop_rate(10);

	///**************publish and subscriber and ros function***********///
	ros::Publisher apl_pub = n.advertise<relative_angle_nb::Apl_kai>("apl_pub", 1);
	ros::Subscriber rysub_depth_turtlebot3 = n.subscribe<sensor_msgs::Image>("/camera/depth/image_raw", 1, boost::bind(&Depth_Image, _1));
	ros::Subscriber rysub_depth = n.subscribe<sensor_msgs::Image>("/camera/aligned_depth_to_color/image_raw", 1, boost::bind(&Depth_Image_rl, _1));
	ros::Subscriber rysub_bboxmidseq = n.subscribe<darknet_ros_msgs::BoundingBoxes>("/darknet_ros/bounding_boxes", 1000, boost::bind(&Find_BboxMidXYposition_Sequence, _1));
	ros::Subscriber rysub_deteobj = n.subscribe<std_msgs::Int8>("/darknet_ros/found_object", 1000, Sub_Msgdata);


	DETECT_BOX.x_goal = X_Destination;
	DETECT_BOX.y_goal = Y_Destination;
	float reg_x_goal=0.0, reg_y_goal=0.0; 
	

	while(ros::ok())
	{
		
		relative_angle_nb::Apl_kai apl_msg;
		apl_msg.obs_dist = DETECT_BOX.nearest_depth;
		apl_msg.obs_direct = DETECT_BOX.obs_direction;
		apl_msg.goal_x = DETECT_BOX.x_goal;
		apl_msg.goal_y = DETECT_BOX.y_goal;
		apl_msg.obs_latelitude = DETECT_BOX.h_dist_between_obsandrobot;
		apl_pub.publish(apl_msg);
		//ros::Duration(3).sleep();		
		ros::spinOnce();
		loop_rate.sleep();
	}
}

