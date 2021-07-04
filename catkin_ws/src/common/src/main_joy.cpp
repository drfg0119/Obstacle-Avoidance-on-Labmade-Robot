#include <ros/ros.h>
#include <fstream>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/Float64MultiArray.h>
#include <sensor_msgs/Joy.h>
#include <sensor_msgs/JointState.h>
#include <nav_msgs/Odometry.h>
using namespace std;

#define   BTN_X             0
#define   BTN_A             1
#define   BTN_B             2
#define   BTN_Y             3
#define   BTN_LB            4
#define   BTN_RB            5
#define   BTN_LT            6
#define   BTN_RT            7
#define   BTN_BACK          8
#define   BTN_START         9
#define   BTN_STICK_L      10
#define   BTN_STICK_R      11

double cur_left_vel,cur_right_vel,left_vel,right_vel; 
double linear_vel,angular_vel=0;
double W=0.43;
int bm[12]={0};
int autotrack=0;
double xm[6]={0};
int joystick_mode=0,auto_mode=0,record_mode=0;
int t=0;
const int row = 20000;
const int column = 3;
double arr[row][column];

bool state[4]={1,0,1,0};

std_msgs::Float64MultiArray autocmd_msg;
std_msgs::Int32MultiArray autocmd_arduino_msg;
void chatterCallback(const nav_msgs::Odometry::ConstPtr& msg)  //建立暫存的message(msg),先將資料存到此變數
{    
	

	//const int row = 20000;
	//const int column = 3;
	//double arr[row][column];
	
	double a=0,b=0,c=0;
	
	//	ROS_INFO("Position-> x: [%f], y: [%f], z: [%f] ", msg->pose.pose.position.x,msg->pose.pose.position.y, msg->pose.pose.position.z);

a=msg->pose.pose.position.x ; b=msg->pose.pose.position.y; c=msg->pose.pose.position.z;
		if(arr[t][0]!=a)
		{
			arr[t][0]=a;
			arr[t][1]=b;
			arr[t][2]=c;
			t=t+1;	
		}
std::ofstream outfile("/home/apl/position.txt",ios::trunc);

for(int i = 0; i < row; i++) { 

           outfile << arr[i][0] << "   " << arr[i][1] << "   " << arr[i][2]; 
 	   outfile << endl; 
    }		
outfile.close();

		
}		
void mpccallback(const std_msgs::Float64MultiArray::ConstPtr& msg)  
{
	/*right_vel = 
	left_vel = msg
	msg.data.push_back(abs(left_vel));
	msg.data.push_back(abs(right_vel));
	
	msg.data.push_back(state[0]);
	msg.data.push_back(state[1]);
	msg.data.push_back(state[2]);
	msg.data.push_back(state[3]);*/
/*
	cout << "msg.data[1] = " << msg->data[1] <<endl;
cout << "msg.data[2] = " << msg->data[2] <<endl;
cout << "msg.data[3] = " << msg->data[3] <<endl;
cout << "msg.data[4] = " << msg->data[4] <<endl;
cout << "msg.data[5] = " << msg->data[5] <<endl;
*/
	
	autocmd_msg.data=msg->data;
	if (bm[BTN_LT]==1){
		autotrack = ! autotrack;
		//autotrack = 1;
	}
	
	if (autotrack==1 ){                             //6:LT.  切換自駕autonomous mode
		right_vel = autocmd_msg.data[0];
		left_vel  = autocmd_msg.data[1];
		state[0]  = autocmd_msg.data[2];
		state[1]  = autocmd_msg.data[3];
		state[2]  = autocmd_msg.data[4];
		state[3]  = autocmd_msg.data[5];
/*
		cout << "state[0]  " << state[0] << endl ;
		cout << "state[1]  " << state[1] << endl ;
		cout << "state[2]  " << state[2] << endl ;
		cout << "state[3]  " << state[3] << endl ;
		cout << "autotrack mode: " << autotrack  << endl;
*/
	}
}
void joycallback(const sensor_msgs::Joy::ConstPtr& msg)   //建立暫存的message(msg),先將資料存到此變數
{
	for (int i=0;i<6;i++)
		xm[i] = msg -> axes[i];
	for (int i=0;i<12;i++)
		bm[i] = msg -> buttons[i];
	/*if (1 == HID_Joy.S8_buttons[BTN_RT])
					{ 
						record = !record;                //錄影開始 結束
						if (record == 1)
						cout << "The record is starting" << endl;
						if (record == 0)
						{cout << "The record is closing" << endl;
						cout << t << endl;}
					}             */
	if (bm[BTN_START] == 1){
		joystick_mode = !joystick_mode;
	}
	//joystick_mode = 1;
	if (joystick_mode == 0)	{	
		cout << "搖桿控制尚未啟動！" << endl;
		state[0] = true;
		state[2] = true;
		ROS_INFO("joystick is close");
	}
	
	if (joystick_mode == 1)
	{
		cout << "現在是搖桿控制！" << endl;
		state[0] = false;
		state[2] = false;
		ROS_INFO("joystick is open");
		
		if(xm[4]==1){	//左右鍵     //Mode暗燈xm[0]xm[1]  Mode亮燈xm[4] xm[5]
			left_vel  -= 100;
			right_vel += 100;	
		}
		else if (xm[4]==-1){
			left_vel  += 100;
			right_vel -= 100;	
		}
		if(xm[5]==1){	//上下鍵
			left_vel  += 100;
			right_vel += 100;	
		}
		else if (xm[5]==-1){
			left_vel  -= 100;
			right_vel -= 100;	
		}
	
		//change_track(linear_vel,angular_vel);
	
		if(bm[BTN_X]==1){	//前進模式
			left_vel  = 300;
			right_vel = 300;	
		}
		else if(bm[BTN_A]==1){	//後退模式
			left_vel  = -300;
			right_vel = -300;	
		}
		else if(bm[BTN_B]==1){	//停止模式
			left_vel  = 1;
			right_vel = 1;	
			cout << "履帶車停車！" << endl;
		}

		else if(bm[BTN_Y]==1)	//auto mode;
		{	cout << "Auto mode" << endl;
			auto_mode =! auto_mode;
			
		}
		else if(bm[BTN_LB]==1){   //原地左轉
			left_vel  = -300;
			right_vel = 300;	
		}
		else if(bm[BTN_RB]==1){   //原地右轉
			left_vel  = 300;
			right_vel = -300;	
		}

		else if(bm[BTN_LT]==1){  //歸零自駕模式
			cout << "downzero" << endl;

			
		//autotrack = ! autotrack;
		state[1] = false;
		state[3] = false;
		cout << "自駕模式" <<endl;
		}
		else if(bm[BTN_RT]==1)	//錄影模式
		{	//cout << "record is starting" << endl;
			record_mode = !record_mode;                //錄影開始 結束

			if (record_mode == 1)
				cout << "The record is starting" << endl;
			if (record_mode == 0)
			{
				cout << "The record is closing" << endl;
				cout << t << endl;
			}
		}
	
		
		if(left_vel>0)
			state[1] = false;     //0v enable F/R enable positively rotate for left track
		else
			state[1] = true;      //5v enable F/R enable negatively rotate for left track
		if(right_vel>0)
			state[3] = false;
		else
			state[3] = true;
		 

		cout << "state[0]  " << state[0] << endl ;
		cout << "state[1]  " << state[1] << endl ;
		cout << "state[2]  " << state[2] << endl ;
		cout << "state[3]  " << state[3] << endl ;
		cout << "左側履帶速度為 " << (int)left_vel << endl ;
		cout << "右側履帶速度為 " << (int)right_vel << endl<<endl;

	}





}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "main_joy");         //一開始必須先初始化，指定node名稱為main_joy

 /* 該node與ROS系統通訊的存取點(handle)，建構子會初始化該node，
     當離開此scope，解構子會關閉該node or  eg將這個程式創建成一個ros節點*/		
	ros::NodeHandle nh;
	
	
	ros::Publisher speed_pub = nh.advertise<std_msgs::Int32MultiArray>("speed", 1000);
	ros::Subscriber mpc_sub = nh.subscribe<std_msgs::Float64MultiArray>("/mpc_cmd", 1000, mpccallback); //20210121
	ros::Subscriber joy_sub = nh.subscribe<sensor_msgs::Joy>("joy", 1000, joycallback);  ////subscribe()會將建立topic的資訊告訴master node，回傳一個Subscriber物件(在此為joy_sub)指定的回呼函式名稱為joyCallback
	//ros::Subscriber robot = nh.subscribe<nav_msgs::Odometry>("/rtabmap/odom",100, chatterCallback);
	autocmd_msg.data.resize(6);
	autocmd_arduino_msg.data.resize(6);
	for(int i=0;i<6;i++){
		autocmd_msg.data[i]=0;
		autocmd_arduino_msg.data[i]=0;
	}
	ros::Rate loop_rate(10);         //設定publish頻率10Hz(speed_pub.publish(msg))

	while (ros::ok())           //在Ros順利執行時
	{
		std_msgs::Int32MultiArray	msg;                //建立暫存的message(msg),先將資料存到此變數,在進行發布publish

		//ROS_INFO("successful ");
		//cout << "左側履帶速度為 " << left_vel << endl ;
		//cout << "右側履帶速度為 " << right_vel << endl<<endl;
		//right_vel = autocmd_msg.data[0];
		//left_vel = autocmd_msg.data[1];
		//state[0] = autocmd_msg.data[2];
		//state[1] = autocmd_msg.data[3];
		//state[2] = autocmd_msg.data[4];
		//state[3] = autocmd_msg.data[5];
		
		msg.data.clear();	
		msg.data.push_back(abs(left_vel));
		msg.data.push_back(abs(right_vel));
		
		msg.data.push_back(state[0]);
		msg.data.push_back(state[1]);
		msg.data.push_back(state[2]);
		msg.data.push_back(state[3]);
/*
	        cout << "msg.data[0] = " << msg.data[0] <<endl;
cout << "msg.data[1] = " << msg.data[1] <<endl;
cout << "msg.data[2] = " << msg.data[2] <<endl;
cout << "msg.data[3] = " << msg.data[3] <<endl;
cout << "msg.data[4] = " << msg.data[4] <<endl;
cout << "msg.data[5] = " << msg.data[5] <<endl;
*/
	  	speed_pub.publish(msg);
		

		


		ros::spinOnce();                  //只執行所有程式碼一次

		loop_rate.sleep();               //等待loop_rate所設定時間

	}
		



	return 0;
}


