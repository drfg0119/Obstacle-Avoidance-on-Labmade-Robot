
#include <geometry_msgs/Twist.h>
#include "data_control.h"
#include <std_msgs/UInt16.h>
#include <std_msgs/Float32.h>

DETECT_BOX_ DETECT_BOX;
/* set model state server
ros::ServiceClient set_model_state_client;
void set_model_state(std::string model_name)
{
	gazebo_msgs::SetModelState msg;
	msg.request.model_state.model_name = model_name;
	set_model_state_client.call(msg);
}
*/
ros::ServiceClient set_model_state_client;
gazebo_msgs::SetModelState setmodelstate;
gazebo_msgs::ModelState model;
geometry_msgs::Pose pose;


int main (int argc, char** argv){
	///****************init ros****************///
	ros::init(argc, argv, "dynamixel_node");
	ros::NodeHandle n;



	///**************publish and subscriber and ros function***********///
	//ros::Subscriber rysub_depth = n.subscribe<sensor_msgs::Image>("/camera/aligned_depth_to_color/image_raw", 1, boost::bind(&Depth_Image, _1));
	ros::Subscriber rysub_depth = n.subscribe<sensor_msgs::Image>("/camera/depth/image_raw", 1, boost::bind(&Depth_Image, _1));
	//ros::Subscriber rysub_dataprocess = n.subscribe<darknet_ros_msgs::BoundingBoxes>("darknet_ros/detepoint1_", 1000, boost::bind(&Image_Boxes_Data_Read_Laser, _1));
	ros::Subscriber rysub_bboxmidseq = n.subscribe<darknet_ros_msgs::BoundingBoxes>("/darknet_ros/bounding_boxes", 1000, boost::bind(&Find_BboxMidXYposition_Sequence, _1));
	ros::Subscriber rysub_deteobj = n.subscribe<std_msgs::Int8>("/darknet_ros/found_object", 1000, Sub_Msgdata);
	ros::Subscriber rysub_spawnedgoal_x = n.subscribe<std_msgs::Int8>("/respawn_goal_x", 1, Sub_Respawned_Goal_X);
	ros::Subscriber rysub_spawnedgoal_y = n.subscribe<std_msgs::Int8>("/respawn_goal_y", 1, Sub_Respawned_Goal_Y);
	ros::Subscriber rysub_robotstate = n.subscribe<gazebo_msgs::ModelStates>("/gazebo/model_states", 1, boost::bind(&Check_if_Reset_Rosbot_Pose, _1));
	ros::Subscriber rysub_poseread = n.subscribe<nav_msgs::Odometry>("/odom", 10, boost::bind(&Sub_and_Dealwith_Odomdata, _1));
	//ros::Subscriber rysub_poseread = n.subscribe("/odom", 10, boost::bind(&Sub_and_Dealwith_Odomdata, _1));
	//ros::Subscriver rysub_modelname = n.subscribe<gazebo_msgs::ModelStates>("/gazebo/model_states", 100, boost::bind(&Find_model_name, _1));

/*
	ros::Publisher obs_distance_pub = n.advertise<std_msgs::UInt16>("obs_distance", 1);
	ros::Publisher obs_direction_pub = n.advertise<std_msgs::Float32>("obs_direction", 1);
	ros::Publisher current_odom_diff = n.advertise<std_msgs::Float32>("odom_diff", 1);
	ros::Publisher h_distance_pub = n.advertise<std_msgs::Int16>("h_dist_obsandrobot", 1);
*/	
	//ros::Publisher model_name_pub = n.advertise<std_msgs::String>("string_for_simulink", 1);
	set_model_state_client = n.serviceClient<gazebo_msgs::SetModelState>("/gazebo/set_model_state");
	
	ros::Publisher apl_pub = n.advertise<relative_angle_nb::Apl_kai>("apl_pub", 1);
	//ros::Rate r();
	DETECT_BOX.x_goal = X_Destination;
	DETECT_BOX.y_goal = Y_Destination;
	float reg_x_goal=0.0, reg_y_goal=0.0;
	

	while(ros::ok())
	{
		
		relative_angle_nb::Apl_kai apl_msg;
		apl_msg.obs_dist = DETECT_BOX.nearest_depth;
		apl_msg.obs_direct = DETECT_BOX.obs_direction;
		//apl_msg.goal_dist = DETECT_BOX.odom_diff;
		apl_msg.goal_x = DETECT_BOX.x_goal;
		apl_msg.goal_y = DETECT_BOX.y_goal;
		apl_msg.obs_latelitude = DETECT_BOX.h_dist_between_obsandrobot;
		apl_pub.publish(apl_msg);
		
		

		if(reg_x_goal - DETECT_BOX.x_goal != 0 || reg_y_goal - DETECT_BOX.y_goal != 0)
		//if(DETECT_BOX.linear_vel < 0.1)
		{
			srand( time(NULL) );
			int min = -9;
			int max = -6;
			int pose_x = rand() % (max - min + 1) + min;
			int pose_y = rand() % (max - min + 1) + min;
			model.pose = pose;
			model.model_name = "red_cylinder";
			model.pose.position.x = pose_x;
			model.pose.position.y = pose_y;
			setmodelstate.request.model_state = model;
			bool success = set_model_state_client.call(setmodelstate);
			if (success)
			{
				ROS_INFO_STREAM("Setting position of " << model.model_name << "model was successful!");
			}
			else
			{
				ROS_INFO_STREAM("Setting position of " << model.model_name << "was failed");
			}
			ros::Duration(3).sleep();
		}

		reg_x_goal = DETECT_BOX.x_goal;
		reg_y_goal = DETECT_BOX.y_goal;

		if(DETECT_BOX.odom_diff > 8.15 || DETECT_BOX.current_abs_robot_z_pose > 0.05)
		{
			
			model.pose = pose;
			//ros::Publisher model_pub = n.advertise<gazebo_msgs::SetModelState>("gazebo/set_model_state", 1);
			model.model_name = "turtlebot3_waffle";
			model.pose.position.x = -9.5;
			model.pose.position.y = -9.5;
			model.pose.position.z = -0.001;
			//model_pub.publish(model);

			setmodelstate.request.model_state = model;
			bool success = set_model_state_client.call(setmodelstate);
			if (success)
			{
				ROS_INFO_STREAM("Setting position of " << model.model_name << "model was successful!");
			}
			else
			{
				ROS_INFO_STREAM("Setting position of " << model.model_name << "was failed");
			}			
			ros::Duration(3).sleep();	
		}
		/*
		std_msgs::UInt16 obs_distance_msg;
		std_msgs::Float32 obs_direction_msg;
		std_msgs::Float32 odom_diff_msg;
		std_msgs::Int16 h_dist_obsandrobot_msg;
		//std_msgs::String string_for_simulink_msg;
		
		//DETECT_BOX.nearest_depth = 20;
		obs_distance_msg.data = DETECT_BOX.nearest_depth;
		obs_direction_msg.data = DETECT_BOX.obs_direction;
		odom_diff_msg.data = DETECT_BOX.odom_diff;
		h_dist_obsandrobot_msg.data = DETECT_BOX.h_dist_between_obsandrobot;
		
		//string_for_simulink_msg.data = "turtlebot3_waffle";

		obs_distance_pub.publish(obs_distance_msg);
		obs_direction_pub.publish(obs_direction_msg);
		current_odom_diff.publish(odom_diff_msg);
		h_distance_pub.publish(h_dist_obsandrobot_msg);
		*/
		
		//model_name_pub.publish(string_for_simulink_msg);
		//set_model_state("turtlebot3_waffle");
		//DETECT_BOX.flg=0;
		
		ros::spinOnce();
		//r.sleep();
	}
}

