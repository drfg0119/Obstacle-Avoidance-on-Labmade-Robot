#include <iostream>
#include <math.h>
#include <cmath>
#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/String.h>
#include "std_msgs/UInt64.h"
#include <vector>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>

int state_data = 0;
double vr_value;
double vl_value;
double current_vel_linear_x;
double current_vel_angular_z;
double rounding(double, int);

void state_callback(const std_msgs::UInt64::ConstPtr &msg)
{
    state_data = msg->data;
}

void Convert_Twist_to_Tracked_Vel(const geometry_msgs::Twist::ConstPtr &msg)
{

    current_vel_linear_x = msg->linear.x;
    current_vel_angular_z = msg->angular.z;

    //**** "inverse ICR matrix times twist matrix" is equal to tracked velocity matrix ****
    vr_value = current_vel_linear_x + 0.5 * current_vel_angular_z;
    vl_value = current_vel_linear_x - 0.5 * current_vel_angular_z;
    //*************************************************************************************
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "apl_rl_node");
    ros::NodeHandle n;
    ros::Rate loop_rate(10);

    ros::Publisher cmd_pub = n.advertise<std_msgs::Float64MultiArray>("/mpc_cmd", 1000);
    ros::Subscriber state_sub = n.subscribe<std_msgs::UInt64>("state_vehicle", 10, state_callback);
    ros::Subscriber rysub_commanded_twist = n.subscribe<geometry_msgs::Twist>("/cmd_vel", 1, Convert_Twist_to_Tracked_Vel);

    std_msgs::Float64MultiArray cmd_msg;

    while (ros::ok())
    {
        //vr_value = rounding(vr_value, 1);
        //vl_value = rounding(vl_value, 1);
        cmd_msg.data.clear();
        /*
        if (state_data == 0)
        { ////0正走 1停止 2反走
            cmd_msg.data.push_back(vr_value * 1853 + 15);
            cmd_msg.data.push_back(vl_value * 1853 + 15);
            cmd_msg.data.push_back(0); //EN、F/N
            cmd_msg.data.push_back(0);
            cmd_msg.data.push_back(0); //+ to - 20210121
            cmd_msg.data.push_back(0);
            cmd_pub.publish(cmd_msg);
	    std::cout << "vr right track velocity: " << vr_value * 1853 + 15 << std::endl;
            std::cout << "vl left track velocity: " << vl_value * 1853 + 15 << std::endl;
        } //transfer to autonomous
        else if (state_data == 1)
        {
            cmd_msg.data.push_back(0);
            cmd_msg.data.push_back(0);
            cmd_msg.data.push_back(1);
            cmd_msg.data.push_back(0);
            cmd_msg.data.push_back(1);
            cmd_msg.data.push_back(0);
            cmd_pub.publish(cmd_msg);
	    std::cout << "vr right track velocity: " << vr_value * 1853 + 15 << std::endl;
            std::cout << "vl left track velocity: " << vl_value * 1853 + 15 << std::endl;
        }
        else
        {
            cmd_msg.data.push_back(vl_value * 1853 + 15);
            cmd_msg.data.push_back(vr_value * 1853 + 15);
            cmd_msg.data.push_back(0);
            cmd_msg.data.push_back(1);
            cmd_msg.data.push_back(0);
            cmd_msg.data.push_back(1);
            cmd_pub.publish(cmd_msg);
	    std::cout << "vr right track velocity: " << vr_value * 1853 + 15 << std::endl;
            std::cout << "vl left track velocity: " << vl_value * 1853 + 15 << std::endl;
        }
	*/
        for (double i=0.001; i<0.01; i++)
        {
            int overrounding_r = rounding(vr_value * 1853 + 15, 1);
            int overrounding_l = rounding(vl_value * 1853 + 15, 1);

            if (state_data == 0)
            { ////0正走 1停止 2反走
                cmd_msg.data.push_back(overrounding_r);
                cmd_msg.data.push_back(overrounding_l);
                cmd_msg.data.push_back(0); //EN、F/N
                cmd_msg.data.push_back(0);
                cmd_msg.data.push_back(0); //+ to - 20210121
                cmd_msg.data.push_back(0);
                cmd_pub.publish(cmd_msg);
                std::cout << "vr right track velocity: " << vr_value * 1853 + 15 << std::endl;
                std::cout << "vl left track velocity: " << vl_value * 1853 + 15 << std::endl;
            } //transfer to autonomous
            
            vr_value = vr_value + i;
            vl_value = vl_value + i;
            ros::spinOnce();
            loop_rate.sleep();
        }

        //ros::spinOnce();
        //loop_rate.sleep();
    }
}

double rounding(double num, int index)
{
    bool isNegative = false;

    if (num < 0)
    {
        isNegative = true;
        num = -num;
    }
    if (index >= 0)
    {
        int multiplier;
        multiplier = pow(10, index);
        num = (int)(num * multiplier + 0.5) / (multiplier * 1.0);
    }
    if (isNegative)
    {
        num = -num;
    }
    return num;
}
