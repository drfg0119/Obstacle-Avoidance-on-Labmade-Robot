#include <ros/ros.h>
#include <fstream>
#include <std_msgs/Int32MultiArray.h>
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
double L=0.43;
int bm[12]={0};
double xm[6]={0};
int joystick_mode=0,auto_mode=0,record_mode=0;
int t=0;
const int row = 20000;
const int column = 3;
double arr[row][column];

bool state[4]={1,0,1,0};



void Start()         
{
	
	state[0]=false;     //0v enable the EN
	state[2]=false;
	
}
void Forward()
{
	
	Start();
	linear_vel=0.1;
	angular_vel=0;
	cout << "Forward mode" << endl;
	state[1]=false;     //0v enable S/R enable positively rotate for left track
	state[3]=false;     //0v enable S/R enable positively rotate for right track
}	

void change_track(double linear_vel,double angular_vel)   //y=14.949+1853.87*x; x=轉速(m/s) y=輸入電壓(0~4095轉0~5V)
{
	if (0.01 >= abs(angular_vel))
	{
		cur_left_vel  = linear_vel;
		cur_right_vel = linear_vel;
	}
	else 
	{
		cur_left_vel =  (linear_vel/angular_vel-L/2)*angular_vel;
		cur_right_vel = (linear_vel/angular_vel+L/2)*angular_vel;
	}
	

	if (cur_left_vel <= 0.01)
		left_vel = 0;		
	else if(0.01 < cur_left_vel  &&  cur_left_vel<=2.2)
		left_vel = 1853.87*cur_left_vel + 14.949;
	else if(2.2< cur_left_vel)
		left_vel=4095;
	if (cur_right_vel <= 0.01)
		right_vel = 0;		
	else if(0.01 < cur_right_vel  &&  cur_right_vel<=2.2)
		right_vel = 1853.87*cur_right_vel + 14.949;
	else if(2.2< cur_right_vel)
		right_vel=4095;
	

	
}
void Backward()
{
	Start();
	cout << "Backward mode" << endl;
	linear_vel=0.1;     //200
	angular_vel=0;
    change_track(linear_vel,angular_vel);
	state[1]=true;      //5v enable S/R enable negatively rotate for left track
	state[3]=true;

}
void Left()
{
	Start();
	linear_vel=0.15;   //293
	angular_vel=0;
    change_track(linear_vel,angular_vel);
	
	cout << "Left mode" << endl;
	cout << left_vel << right_vel << endl;
	state[1]=true;     //5v enable S/R enable negatively rotate for left track
	state[3]=false; 


}
void Right()
{	
	Start();
	linear_vel=0.15;
	angular_vel=0;
    change_track(linear_vel,angular_vel);
	cout << "Right mode" << endl;
	state[1]=false;     //0v enable S/R enable positively rotate for left track
	state[3]=true; 

}
void Stop()
{
	cout << "Stop mode" << endl;


	linear_vel=0;
	angular_vel=0;
    change_track(linear_vel,angular_vel);
	//left_vel = 0;
	//right_vel = 0;

	state[0]=true;     //5v disable for EN   
	state[2]=true;


}

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
std::ofstream outfile("/home/apl/trajectory.txt",ios::trunc);

for(int i = 0; i < row; i++) { 

           outfile << arr[i][0] << "   " << arr[i][1] << "   " << arr[i][2]; 
 	   outfile << endl; 
    }		
outfile.close();



		
		
}		
void joycallback(const sensor_msgs::Joy::ConstPtr& msg)   ///建立暫存的message(msg),先將資料存到此變數
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
	//if (bm[BTN_START] == 1)
	//{
	//	joystick_mode = !joystick_mode;
	//}
	joystick_mode = 1;
	if (joystick_mode == 0)
	{	
	cout << "搖桿控制尚未啟動！" << endl;
	//ROS_INFO("joystick is close");
	}
	
	if (joystick_mode == 1)
	{
		
		cout << "現在是搖桿控制！" << endl;
		ROS_INFO("joystick is open");
		
		if(xm[4]==1)	//左右鍵
			angular_vel = angular_vel + 0.1;
		else if (xm[4]==-1)
			angular_vel = angular_vel - 0.1;	
		if(xm[5]==1)	//上下鍵
			linear_vel = linear_vel + 0.1;
		else if (xm[5]==-1)
			linear_vel = linear_vel - 0.1;
	
	
		change_track(linear_vel,angular_vel);
	
		if(bm[BTN_X]==1)	//前進模式
			Forward();
		else if(bm[BTN_A]==1)	//後退模式
			Backward();
		else if(bm[BTN_B]==1)	//停止模式
			Stop();
		else if(bm[BTN_Y]==1)	//auto mode;
		{	cout << "Auto mode" << endl;
			auto_mode =! auto_mode;
			
		}
		else if(bm[BTN_LB]==1)   //左轉
			Left();
		else if(bm[BTN_RB]==1)   //右轉
			Right();
		else if(bm[BTN_LT]==1)   //歸零
			cout << "downzero" << endl;
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
		cout << "線'角速度為 " << linear_vel << " " << angular_vel << " " << L << endl ;
		

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
	ros::init(argc, argv, "tracked_joy");         //一開始必須先初始化，指定node名稱為main_joy

 /* 該node與ROS系統通訊的存取點(handle)，建構子會初始化該node，
     當離開此scope，解構子會關閉該node or  eg將這個程式創建成一個ros節點*/		
	ros::NodeHandle nh;
	
	
	ros::Publisher speed_pub = nh.advertise<std_msgs::Int32MultiArray>("speed", 1000);
	ros::Subscriber joy_sub = nh.subscribe<sensor_msgs::Joy>("joy", 1000, joycallback);  ////subscribe()會將建立topic的資訊告訴master node，回傳一個Subscriber物件(在此為joy_sub)指定的回呼函式名稱為joyCallback
	ros::Subscriber robot = nh.subscribe<nav_msgs::Odometry>("/rtabmap/odom",100, chatterCallback);

	ros::Rate loop_rate(10);         //設定publish頻率10Hz (speed_pub.publish(msg))

	while (ros::ok())           //在Ros順利執行時
	{
		std_msgs::Int32MultiArray	msg;                //建立暫存的message(msg),先將資料存到此變數,在進行發布publish


		//ROS_INFO("successful ");
		//cout << "左側履帶速度為 " << left_vel << endl ;
		//cout << "右側履帶速度為 " << right_vel << endl<<endl;

		msg.data.clear();	

		msg.data.push_back(state[0]);
		msg.data.push_back(state[1]);
		msg.data.push_back(state[2]);
		msg.data.push_back(state[3]);
		msg.data.push_back(left_vel);
		msg.data.push_back(right_vel);

	  speed_pub.publish(msg);

		
		ros::spinOnce();                  //只執行所有程式碼一次

		loop_rate.sleep();               //等待loop_rate所設定時間

	}
		



	return 0;
}

