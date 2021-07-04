
#include "data_control.h"
#include <iostream>
#include <unistd.h>
#include <vector>
#include<algorithm>
#include<numeric>

using namespace std;
using namespace cv;
/*************************************************************************************
**flg ==0 detect and grab obstacle numbers and insert to global variable --> flg = 1**
**flg ==1 check if detected pixel position is correct 			 --> flg = 2**
**flg ==2 insert depth value to BOX 					 --> flg = 0**
*************************************************************************************/

unsigned char G_Object_Num;
//unsigned int G_Small_ObsDistance;
//float G_X_Current_Pose_Differential;
//float G_Y_Current_Pose_Differential;

////flg ==0 detect and grab obstacle numbers and insert to global variable --> flg = 1
void Sub_Msgdata(const std_msgs::Int8::ConstPtr& msg)
{
    if(DETECT_BOX.flg==0)
    {
        G_Object_Num = msg->data;
        if(G_Object_Num > 0)
        {
            if(G_Object_Num > Object_Num_Max)
            {
                G_Object_Num = Object_Num_Max;
            }
            DETECT_BOX.flg = 1;
        }
        /*comment out because I let MATLAB do this, that give 0 to the observaiotn. 
        else // clean the register when no any detection
        {
                DETECT_BOX.nearest_depth = 0;
                DETECT_BOX.obs_direction = 0.0;
                DETECT_BOX.h_dist_between_obsandrobot = 0;
        }
        */
    }
    //printf("Sub_Msgdata DETECT_BOX.flg =%d\n",DETECT_BOX.flg);
}

void Sub_Respawned_Goal_X(const std_msgs::Int8::ConstPtr& msg)
{
        float tmp_goal_x = msg->data;
        DETECT_BOX.x_goal = tmp_goal_x - 0.5;
}
void Sub_Respawned_Goal_Y(const std_msgs::Int8::ConstPtr& msg)
{
        float tmp_goal_y = msg->data;
        DETECT_BOX.y_goal = tmp_goal_y - 0.5;
}
/* JJ Part
// flg ==1 check if detected pixel position is correct --> flg = 2
void Image_Boxes_Data_Read_Laser(const darknet_ros_msgs::BoundingBoxes::ConstPtr& msg)
{
        static unsigned int x_center = 0, y_center = 0; //用 static 不會被洗掉

        if(DETECT_BOX.flg == 1 && G_Object_Num > 0)
        {
                for(int i = 0; i < G_Object_Num; i++)
                {
                        int x_center_tmp = msg->bounding_boxes[i].xmin;
                        int y_center_tmp = msg->bounding_boxes[i].ymin;

                        if(x_center_tmp < 0 || y_center_tmp < 0 || x_center_tmp > Cam_H || y_center_tmp > Cam_V)
                        {
                                x_center_tmp = x_center;
                                y_center_tmp = y_center;
                                printf("detected pixel position error");
                        }
                        else
                        {
                                x_center = x_center_tmp;
                                y_center = y_center_tmp;
                        }
                        DETECT_BOX.BOX_OBSTACLE[i].x_center = x_center;
                        DETECT_BOX.BOX_OBSTACLE[i].y_center = y_center;
                        DETECT_BOX.BOX_OBSTACLE[i].object_num = i;
                        DETECT_BOX.total_num = G_Object_Num;
                }
                DETECT_BOX.flg = 2;
        }
        //printf("Image_Boxes_Data_Read_Laser DETECT_BOX.flg =%d\n",DETECT_BOX.flg);
}
*/

void Find_BboxMidXYposition_Sequence(const darknet_ros_msgs::BoundingBoxes::ConstPtr& msg)
{
        static unsigned int x_min = 0, y_min = 0, x_max = 0, y_max = 0; //用 static 不會被洗掉

        if(DETECT_BOX.flg == 1 && G_Object_Num > 0)
        {
                for(int i = 0; i < G_Object_Num; i++) //find the edge bounding boxes
                {
                        int x_min_tmp = msg->bounding_boxes[i].xmin;
                        int y_min_tmp = msg->bounding_boxes[i].ymin;
                        int x_max_tmp = msg->bounding_boxes[i].xmax;
                        int y_max_tmp = msg->bounding_boxes[i].ymax;

                        if(x_min_tmp < 0 || y_min_tmp < 0 || x_min_tmp > Cam_H || y_min_tmp > Cam_V)
                        {
                                x_min_tmp = x_min;
                                y_min_tmp = y_min;
                                printf("detected pixel position error");
                        }
                        else
                        {
                                x_min = x_min_tmp;
                                y_min = y_min_tmp;
                                x_max = x_max_tmp;
                                y_max = y_max_tmp;
                        }
                        DETECT_BOX.BOX_OBSTACLE[i].x_length = x_max - x_min + 1;
                        DETECT_BOX.BOX_OBSTACLE[i].y_center = ((y_max - y_min + 1) / 2) + y_min;
                        DETECT_BOX.BOX_OBSTACLE[i].x_center = ((x_max - x_min + 1) / 2) + x_min;

                        /*
                        DETECT_BOX.BOX_OBSTACLE[i].x_min = x_min;
                        DETECT_BOX.BOX_OBSTACLE[i].y_min = y_min;
                        DETECT_BOX.BOX_OBSTACLE[i].x_max = y_max;
                        DETECT_BOX.BOX_OBSTACLE[i].y_max = y_max;
                        DETECT_BOX.BOX_OBSTACLE[i].object_num = i;
                        */
                        DETECT_BOX.total_num = G_Object_Num;
                }
                DETECT_BOX.flg = 2;
        }
        //printf("Find_BboxMidXYposition_Sequence DETECT_BOX.flg =%d\n",DETECT_BOX.flg);
}


void Depth_Image_rl(const sensor_msgs::ImageConstPtr& msg)
{


        static unsigned int cam_height = 0;
        static unsigned char *pt, *pt_test;
        cv::Mat image;
        cv_bridge::CvImageConstPtr cv_ptr;
        cv_ptr = cv_bridge::toCvShare(msg,"16UC1");
        image = cv_ptr -> image;
        pt = image.data;
        //pt_test = image.data;
        //cout << "size(pt) =" << size(pt) << endl;
        //cout << "size(pt_test) =" << size(pt_test) << endl;
        //cout << "sizeof pt =" << sizeof pt/sizeof pt[0] << endl;
        //cout << "sizeof pt_test =" << sizeof pt_test/sizeof pt_test[0] << endl;
        vector<int> depth_temp; //use vector for finding mean of the depth value
        //namedWindow("image",0);
        //imshow( "image", image );
        //printf("initial vector\n");
        //printf("Size of image:\ncol: %d\nrow: %d\n", image.cols, image.rows);

#if 1
        if(DETECT_BOX.flg==2){
                
                for(int i=0; i<DETECT_BOX.total_num; i++)
                {

                        depth_temp.clear(); // clean the regist at each bbox
                        /* add vector by Jen Yo
                        unsigned int length_tmp = DETECT_BOX.BOX_OBSTACLE[i].x_length;
                        for (int j = 0; j < length_tmp; j++)
                        {
                                unsigned int mid_depthpixel_vector = ((DETECT_BOX.BOX_OBSTACLE[i].x_min + j) + DETECT_BOX.BOX_OBSTACLE[i].y_center * Cam_H) << 1;
                                unsigned int depth_val = pt[mid_depthpixel_vector] + (pt[mid_depthpixel_vector + 1] << 8);
                                depth_temp.push_back(depth_val);
                        }

                        //**** find the middle values of the vector, then mean them, in order to prevent objects overlapped. ****

                        sort(depth_temp.begin(), depth_temp.end());
                        unsigned char delete_elements_num = depth_temp.size() / 4;
                        depth_temp.erase(depth_temp.begin(),depth_temp.begin()+delete_elements_num);
                        depth_temp.erase(depth_temp.end()-delete_elements_num,(depth_temp.end()-delete_elements_num)+delete_elements_num);
                        unsigned int depth_vec_sum = accumulate(begin(depth_temp), end(depth_temp), 0);
                        unsigned int depth_mean = depth_vec_sum / depth_temp.size();

                        DETECT_BOX.BOX_OBSTACLE[i].depth = depth_mean;
                        printf("DETECT_BOX.BOX_OBSTACLE[i].depth=%d\n", DETECT_BOX.BOX_OBSTACLE[i].depth);

                        //*******************************************************************************************************
                        */
                        // no vector version by JJ
                        //get camera depth and laser depth
                        unsigned int depth_pixel = (DETECT_BOX.BOX_OBSTACLE[i].x_center + DETECT_BOX.BOX_OBSTACLE[i].y_center*Cam_H)<<1;
                        DETECT_BOX.BOX_OBSTACLE[i].depth = pt[depth_pixel] + (pt[depth_pixel+1]<<8);
                       
                        //printf("pt[0]=%d\n",pt[0]);
                        //printf("pt[1]=%d\n",pt[1]);
                        //printf("pt[2]=%d\n",pt[2]);
                        
                        //printf("pt[3838]=%d\n",pt[3838]);
                        //printf("pt[3839]=%d\n",pt[3839]);
                        

                        printf("object = %d\n",i);
                        printf("pixelx & y:%d %d\n",DETECT_BOX.BOX_OBSTACLE[i].x_center,DETECT_BOX.BOX_OBSTACLE[i].y_center);
                        printf("cam_height=%d\n",DETECT_BOX.BOX_OBSTACLE[i].depth);
                        

                        /*
                        //******compute angle between obstacle and vehicle*******
                        //1st quadrant
                        if(DETECT_BOX.BOX_OBSTACLE[i].x_center > Cam_H_Center && DETECT_BOX.BOX_OBSTACLE[i].y_center < Cam_V_Center)
                        {
                            DETECT_BOX.BOX_OBSTACLE[i].h_angle = (float)((atan(((DETECT_BOX.BOX_OBSTACLE[i].x_center - Cam_H_Center)*w_pixcel)/cam_height))*180/PI);
                            DETECT_BOX.BOX_OBSTACLE[i].v_angle = (float)((atan(((Cam_V_Center - DETECT_BOX.BOX_OBSTACLE[i].y_center)*h_pixcel)/cam_height))*180/PI);
                            printf("Angle_H on the Right=%f deg \nAngle_V on the Top %f deg\n",DETECT_BOX.BOX_OBSTACLE[i].h_angle, DETECT_BOX.BOX_OBSTACLE[i].v_angle);
                        }

                        //2nd quadrant
                        else if(DETECT_BOX.BOX_OBSTACLE[i].x_center < Cam_H_Center && DETECT_BOX.BOX_OBSTACLE[i].y_center < Cam_V_Center)
                        {
                            DETECT_BOX.BOX_OBSTACLE[i].h_angle = (float)((atan(((Cam_H_Center - DETECT_BOX.BOX_OBSTACLE[i].x_center)*w_pixcel)/cam_height))*180/PI);
                            DETECT_BOX.BOX_OBSTACLE[i].v_angle = (float)((atan(((Cam_V_Center - DETECT_BOX.BOX_OBSTACLE[i].y_center)*h_pixcel)/cam_height))*180/PI);
                            printf("Angle_H on the Left=%f deg \nAngle_V on the Top %f deg\n",DETECT_BOX.BOX_OBSTACLE[i].h_angle, DETECT_BOX.BOX_OBSTACLE[i].v_angle);
                        }

                        //3rd quadrant
                        else if (DETECT_BOX.BOX_OBSTACLE[i].x_center < Cam_H_Center && DETECT_BOX.BOX_OBSTACLE[i].y_center > Cam_V_Center)
                        {
                            DETECT_BOX.BOX_OBSTACLE[i].h_angle = (float)((atan(((Cam_H_Center - DETECT_BOX.BOX_OBSTACLE[i].x_center)*w_pixcel)/cam_height))*180/PI);
                            DETECT_BOX.BOX_OBSTACLE[i].v_angle = (float)((atan(((DETECT_BOX.BOX_OBSTACLE[i].y_center - Cam_V_Center)*h_pixcel)/cam_height))*180/PI);
                            printf("Angle_H on the Left=%f deg \nAngle_V at Bottom %f deg\n",DETECT_BOX.BOX_OBSTACLE[i].h_angle, DETECT_BOX.BOX_OBSTACLE[i].v_angle);
                        }

                        //4th quadrant
                        else
                        {
                            DETECT_BOX.BOX_OBSTACLE[i].h_angle = (float)((atan(((DETECT_BOX.BOX_OBSTACLE[i].x_center - Cam_H_Center)*w_pixcel)/cam_height))*180/PI);
                            DETECT_BOX.BOX_OBSTACLE[i].v_angle = (float)((atan(((DETECT_BOX.BOX_OBSTACLE[i].y_center - Cam_V_Center)*h_pixcel)/cam_height))*180/PI);
                            printf("Angle_H on the Right=%f deg \nAngle_V at Bottom %f deg\n",DETECT_BOX.BOX_OBSTACLE[i].h_angle, DETECT_BOX.BOX_OBSTACLE[i].v_angle);
                        }
                        */

                        printf("\n\n");
                }

                unsigned int min_temp_depth = 0;
                unsigned int min_x_center = 0;
                //float min_temp_h_angle = 0.0;
                //int min_temp_h_distance = 0;
                min_temp_depth = DETECT_BOX.BOX_OBSTACLE[0].depth;
                

                if(DETECT_BOX.total_num>1)
                {
                        for (int j = 1; j < DETECT_BOX.total_num; j++)
                        {
                                if(DETECT_BOX.BOX_OBSTACLE[j].depth < min_temp_depth)
                                {
                                        min_temp_depth = DETECT_BOX.BOX_OBSTACLE[j].depth;
                                        min_x_center = DETECT_BOX.BOX_OBSTACLE[j].x_center;
                                        /*
                                        float w_pixcel = (2*DETECT_BOX.BOX_OBSTACLE[j].depth*tan((Cam_FOV_H/2)*PI/180))/Cam_H;
                                        if (DETECT_BOX.BOX_OBSTACLE[j].x_center > Cam_H_Center)
                                        {
                                                DETECT_BOX.BOX_OBSTACLE[j].h_angle = (float)((atan(((DETECT_BOX.BOX_OBSTACLE[j].x_center - Cam_H_Center) * w_pixcel) / DETECT_BOX.BOX_OBSTACLE[j].depth)) * 180 / PI);
                                                printf("Angle_H =%f deg \n", DETECT_BOX.BOX_OBSTACLE[j].h_angle);
                                                DETECT_BOX.BOX_OBSTACLE[j].h_distance = (DETECT_BOX.BOX_OBSTACLE[j].x_center - Cam_H_Center) * w_pixcel;
                                        }
                                        else
                                        {
                                                DETECT_BOX.BOX_OBSTACLE[j].h_angle = (float)((atan(((Cam_H_Center - DETECT_BOX.BOX_OBSTACLE[j].x_center) * w_pixcel) / DETECT_BOX.BOX_OBSTACLE[j].depth)) * 180 / PI);
                                                printf("Angle_H =%f deg \n", DETECT_BOX.BOX_OBSTACLE[j].h_angle);
                                                DETECT_BOX.BOX_OBSTACLE[j].h_distance = (Cam_H_Center - DETECT_BOX.BOX_OBSTACLE[j].x_center) * w_pixcel *(-1);
                                        }
                                        min_temp_h_angle = DETECT_BOX.BOX_OBSTACLE[j].h_angle;
                                        min_temp_h_distance = DETECT_BOX.BOX_OBSTACLE[j].h_distance;
                                        */
                                }
                        }
                }
                else
                {
                        /*
                        float w_pixcel = (2*DETECT_BOX.BOX_OBSTACLE[0].depth*tan((Cam_FOV_H/2)*PI/180))/Cam_H;
                        if (DETECT_BOX.BOX_OBSTACLE[0].x_center > Cam_H_Center)
                        {
                                DETECT_BOX.BOX_OBSTACLE[0].h_angle = (float)((atan(((DETECT_BOX.BOX_OBSTACLE[0].x_center - Cam_H_Center) * w_pixcel) / DETECT_BOX.BOX_OBSTACLE[0].depth)) * 180 / PI);
                                printf("Angle_H =%f deg \n", DETECT_BOX.BOX_OBSTACLE[0].h_angle);
                                DETECT_BOX.BOX_OBSTACLE[0].h_distance = (DETECT_BOX.BOX_OBSTACLE[0].x_center - Cam_H_Center) * w_pixcel;
                        }
                        else
                        {
                                DETECT_BOX.BOX_OBSTACLE[0].h_angle = (float)((atan(((Cam_H_Center - DETECT_BOX.BOX_OBSTACLE[0].x_center) * w_pixcel) / DETECT_BOX.BOX_OBSTACLE[0].depth)) * (-180) / PI);
                                printf("Angle_H =%f deg \n", DETECT_BOX.BOX_OBSTACLE[0].h_angle);
                                DETECT_BOX.BOX_OBSTACLE[0].h_distance = (Cam_H_Center - DETECT_BOX.BOX_OBSTACLE[0].x_center) * w_pixcel * (-1);
                        }
                        */
                        min_temp_depth = DETECT_BOX.BOX_OBSTACLE[0].depth;
                        min_x_center = DETECT_BOX.BOX_OBSTACLE[0].x_center;
                        //min_temp_h_angle = DETECT_BOX.BOX_OBSTACLE[0].h_angle;
                        //min_temp_h_distance = DETECT_BOX.BOX_OBSTACLE[0].h_distance;
                }
                DETECT_BOX.nearest_depth = min_temp_depth;
                //DETECT_BOX.obs_direction = min_temp_h_angle;
                DETECT_BOX.h_dist_between_obsandrobot = min_x_center; //sorry, borrow to regist
		
		//DETECT_BOX.nearest_depth = 5000;
                DETECT_BOX.flg=0;
        }
        //printf("Depth_Image DETECT_BOX.flg =%d\n",DETECT_BOX.flg);
#else
        DETECT_BOX.BOX_OBSTACLE[0].x_center=320;
        DETECT_BOX.BOX_OBSTACLE[0].y_center=240;
                for(int i=0; i<1; i++)
                {
                        unsigned int depth_pixel = (DETECT_BOX.BOX_OBSTACLE[i].x_center + DETECT_BOX.BOX_OBSTACLE[i].y_center*Cam_H)<<1;
                        DETECT_BOX.BOX_OBSTACLE[i].depth = pt[depth_pixel] + (pt[depth_pixel+1]<<8);
                        printf("object %d depth = %d \n",i,DETECT_BOX.BOX_OBSTACLE[i].depth);
                }
#endif
}

// flg = 2 insert depth value to BOX --> flg = 0
void Depth_Image(const sensor_msgs::ImageConstPtr& msg)
{


        static unsigned int cam_height = 0;
        static unsigned char *pt, *pt_test;
        cv::Mat image;
        cv_bridge::CvImageConstPtr cv_ptr;
        cv_ptr = cv_bridge::toCvShare(msg,"32FC1");
        image = cv_ptr -> image;
        cv::Mat depth_converted; 
        cv::normalize(image, depth_converted, 0, 65535, NORM_MINMAX, CV_16UC1);
        pt = depth_converted.data;
        //pt_test = image.data;
        //cout << "size(pt) =" << size(pt) << endl;
        //cout << "size(pt_test) =" << size(pt_test) << endl;
        //cout << "sizeof pt =" << sizeof pt/sizeof pt[0] << endl;
        //cout << "sizeof pt_test =" << sizeof pt_test/sizeof pt_test[0] << endl;
        vector<int> depth_temp; //use vector for finding mean of the depth value
        //namedWindow("image",0);
        //imshow( "image", image );
        //printf("initial vector\n");
        //printf("Size of image:\ncol: %d\nrow: %d\n", image.cols, image.rows);

#if 1
        if(DETECT_BOX.flg==2){
                
                for(int i=0; i<DETECT_BOX.total_num; i++)
                {

                        depth_temp.clear(); // clean the regist at each bbox
                        /* add vector by Jen Yo
                        unsigned int length_tmp = DETECT_BOX.BOX_OBSTACLE[i].x_length;
                        for (int j = 0; j < length_tmp; j++)
                        {
                                unsigned int mid_depthpixel_vector = ((DETECT_BOX.BOX_OBSTACLE[i].x_min + j) + DETECT_BOX.BOX_OBSTACLE[i].y_center * Cam_H) << 1;
                                unsigned int depth_val = pt[mid_depthpixel_vector] + (pt[mid_depthpixel_vector + 1] << 8);
                                depth_temp.push_back(depth_val);
                        }

                        //**** find the middle values of the vector, then mean them, in order to prevent objects overlapped. ****

                        sort(depth_temp.begin(), depth_temp.end());
                        unsigned char delete_elements_num = depth_temp.size() / 4;
                        depth_temp.erase(depth_temp.begin(),depth_temp.begin()+delete_elements_num);
                        depth_temp.erase(depth_temp.end()-delete_elements_num,(depth_temp.end()-delete_elements_num)+delete_elements_num);
                        unsigned int depth_vec_sum = accumulate(begin(depth_temp), end(depth_temp), 0);
                        unsigned int depth_mean = depth_vec_sum / depth_temp.size();

                        DETECT_BOX.BOX_OBSTACLE[i].depth = depth_mean;
                        printf("DETECT_BOX.BOX_OBSTACLE[i].depth=%d\n", DETECT_BOX.BOX_OBSTACLE[i].depth);

                        //*******************************************************************************************************
                        */
                        // no vector version by JJ
                        //get camera depth and laser depth
                        unsigned int depth_pixel = (DETECT_BOX.BOX_OBSTACLE[i].x_center + DETECT_BOX.BOX_OBSTACLE[i].y_center*Cam_H)<<1;
                        DETECT_BOX.BOX_OBSTACLE[i].depth = pt[depth_pixel] + (pt[depth_pixel+1]<<8);
                       
                        //printf("pt[0]=%d\n",pt[0]);
                        //printf("pt[1]=%d\n",pt[1]);
                        //printf("pt[2]=%d\n",pt[2]);
                        
                        //printf("pt[3838]=%d\n",pt[3838]);
                        //printf("pt[3839]=%d\n",pt[3839]);
                        

                        printf("object = %d\n",i);
                        printf("pixelx & y:%d %d\n",DETECT_BOX.BOX_OBSTACLE[i].x_center,DETECT_BOX.BOX_OBSTACLE[i].y_center);
                        printf("cam_height=%d\n",DETECT_BOX.BOX_OBSTACLE[i].depth);
                        

                        /*
                        //******compute angle between obstacle and vehicle*******
                        //1st quadrant
                        if(DETECT_BOX.BOX_OBSTACLE[i].x_center > Cam_H_Center && DETECT_BOX.BOX_OBSTACLE[i].y_center < Cam_V_Center)
                        {
                            DETECT_BOX.BOX_OBSTACLE[i].h_angle = (float)((atan(((DETECT_BOX.BOX_OBSTACLE[i].x_center - Cam_H_Center)*w_pixcel)/cam_height))*180/PI);
                            DETECT_BOX.BOX_OBSTACLE[i].v_angle = (float)((atan(((Cam_V_Center - DETECT_BOX.BOX_OBSTACLE[i].y_center)*h_pixcel)/cam_height))*180/PI);
                            printf("Angle_H on the Right=%f deg \nAngle_V on the Top %f deg\n",DETECT_BOX.BOX_OBSTACLE[i].h_angle, DETECT_BOX.BOX_OBSTACLE[i].v_angle);
                        }

                        //2nd quadrant
                        else if(DETECT_BOX.BOX_OBSTACLE[i].x_center < Cam_H_Center && DETECT_BOX.BOX_OBSTACLE[i].y_center < Cam_V_Center)
                        {
                            DETECT_BOX.BOX_OBSTACLE[i].h_angle = (float)((atan(((Cam_H_Center - DETECT_BOX.BOX_OBSTACLE[i].x_center)*w_pixcel)/cam_height))*180/PI);
                            DETECT_BOX.BOX_OBSTACLE[i].v_angle = (float)((atan(((Cam_V_Center - DETECT_BOX.BOX_OBSTACLE[i].y_center)*h_pixcel)/cam_height))*180/PI);
                            printf("Angle_H on the Left=%f deg \nAngle_V on the Top %f deg\n",DETECT_BOX.BOX_OBSTACLE[i].h_angle, DETECT_BOX.BOX_OBSTACLE[i].v_angle);
                        }

                        //3rd quadrant
                        else if (DETECT_BOX.BOX_OBSTACLE[i].x_center < Cam_H_Center && DETECT_BOX.BOX_OBSTACLE[i].y_center > Cam_V_Center)
                        {
                            DETECT_BOX.BOX_OBSTACLE[i].h_angle = (float)((atan(((Cam_H_Center - DETECT_BOX.BOX_OBSTACLE[i].x_center)*w_pixcel)/cam_height))*180/PI);
                            DETECT_BOX.BOX_OBSTACLE[i].v_angle = (float)((atan(((DETECT_BOX.BOX_OBSTACLE[i].y_center - Cam_V_Center)*h_pixcel)/cam_height))*180/PI);
                            printf("Angle_H on the Left=%f deg \nAngle_V at Bottom %f deg\n",DETECT_BOX.BOX_OBSTACLE[i].h_angle, DETECT_BOX.BOX_OBSTACLE[i].v_angle);
                        }

                        //4th quadrant
                        else
                        {
                            DETECT_BOX.BOX_OBSTACLE[i].h_angle = (float)((atan(((DETECT_BOX.BOX_OBSTACLE[i].x_center - Cam_H_Center)*w_pixcel)/cam_height))*180/PI);
                            DETECT_BOX.BOX_OBSTACLE[i].v_angle = (float)((atan(((DETECT_BOX.BOX_OBSTACLE[i].y_center - Cam_V_Center)*h_pixcel)/cam_height))*180/PI);
                            printf("Angle_H on the Right=%f deg \nAngle_V at Bottom %f deg\n",DETECT_BOX.BOX_OBSTACLE[i].h_angle, DETECT_BOX.BOX_OBSTACLE[i].v_angle);
                        }
                        */

                        printf("\n\n");
                }

                unsigned int min_temp_depth = 0;
                unsigned int min_x_center = 0;
                //float min_temp_h_angle = 0.0;
                //int min_temp_h_distance = 0;
                min_temp_depth = DETECT_BOX.BOX_OBSTACLE[0].depth;
                

                if(DETECT_BOX.total_num>1)
                {
                        for (int j = 1; j < DETECT_BOX.total_num; j++)
                        {
                                if(DETECT_BOX.BOX_OBSTACLE[j].depth < min_temp_depth)
                                {
                                        min_temp_depth = DETECT_BOX.BOX_OBSTACLE[j].depth;
                                        min_x_center = DETECT_BOX.BOX_OBSTACLE[j].x_center;
                                        /*
                                        float w_pixcel = (2*DETECT_BOX.BOX_OBSTACLE[j].depth*tan((Cam_FOV_H/2)*PI/180))/Cam_H;
                                        if (DETECT_BOX.BOX_OBSTACLE[j].x_center > Cam_H_Center)
                                        {
                                                DETECT_BOX.BOX_OBSTACLE[j].h_angle = (float)((atan(((DETECT_BOX.BOX_OBSTACLE[j].x_center - Cam_H_Center) * w_pixcel) / DETECT_BOX.BOX_OBSTACLE[j].depth)) * 180 / PI);
                                                printf("Angle_H =%f deg \n", DETECT_BOX.BOX_OBSTACLE[j].h_angle);
                                                DETECT_BOX.BOX_OBSTACLE[j].h_distance = (DETECT_BOX.BOX_OBSTACLE[j].x_center - Cam_H_Center) * w_pixcel;
                                        }
                                        else
                                        {
                                                DETECT_BOX.BOX_OBSTACLE[j].h_angle = (float)((atan(((Cam_H_Center - DETECT_BOX.BOX_OBSTACLE[j].x_center) * w_pixcel) / DETECT_BOX.BOX_OBSTACLE[j].depth)) * 180 / PI);
                                                printf("Angle_H =%f deg \n", DETECT_BOX.BOX_OBSTACLE[j].h_angle);
                                                DETECT_BOX.BOX_OBSTACLE[j].h_distance = (Cam_H_Center - DETECT_BOX.BOX_OBSTACLE[j].x_center) * w_pixcel *(-1);
                                        }
                                        min_temp_h_angle = DETECT_BOX.BOX_OBSTACLE[j].h_angle;
                                        min_temp_h_distance = DETECT_BOX.BOX_OBSTACLE[j].h_distance;
                                        */
                                }
                        }
                }
                else
                {
                        /*
                        float w_pixcel = (2*DETECT_BOX.BOX_OBSTACLE[0].depth*tan((Cam_FOV_H/2)*PI/180))/Cam_H;
                        if (DETECT_BOX.BOX_OBSTACLE[0].x_center > Cam_H_Center)
                        {
                                DETECT_BOX.BOX_OBSTACLE[0].h_angle = (float)((atan(((DETECT_BOX.BOX_OBSTACLE[0].x_center - Cam_H_Center) * w_pixcel) / DETECT_BOX.BOX_OBSTACLE[0].depth)) * 180 / PI);
                                printf("Angle_H =%f deg \n", DETECT_BOX.BOX_OBSTACLE[0].h_angle);
                                DETECT_BOX.BOX_OBSTACLE[0].h_distance = (DETECT_BOX.BOX_OBSTACLE[0].x_center - Cam_H_Center) * w_pixcel;
                        }
                        else
                        {
                                DETECT_BOX.BOX_OBSTACLE[0].h_angle = (float)((atan(((Cam_H_Center - DETECT_BOX.BOX_OBSTACLE[0].x_center) * w_pixcel) / DETECT_BOX.BOX_OBSTACLE[0].depth)) * (-180) / PI);
                                printf("Angle_H =%f deg \n", DETECT_BOX.BOX_OBSTACLE[0].h_angle);
                                DETECT_BOX.BOX_OBSTACLE[0].h_distance = (Cam_H_Center - DETECT_BOX.BOX_OBSTACLE[0].x_center) * w_pixcel * (-1);
                        }
                        */
                        min_temp_depth = DETECT_BOX.BOX_OBSTACLE[0].depth;
                        min_x_center = DETECT_BOX.BOX_OBSTACLE[0].x_center;
                        //min_temp_h_angle = DETECT_BOX.BOX_OBSTACLE[0].h_angle;
                        //min_temp_h_distance = DETECT_BOX.BOX_OBSTACLE[0].h_distance;
                }
                DETECT_BOX.nearest_depth = min_temp_depth;
                //DETECT_BOX.obs_direction = min_temp_h_angle;
                DETECT_BOX.h_dist_between_obsandrobot = min_x_center; //sorry, borrow to regist
		
		//DETECT_BOX.nearest_depth = 5000;
                DETECT_BOX.flg=0;
        }
        //printf("Depth_Image DETECT_BOX.flg =%d\n",DETECT_BOX.flg);
#else
        DETECT_BOX.BOX_OBSTACLE[0].x_center=320;
        DETECT_BOX.BOX_OBSTACLE[0].y_center=240;
                for(int i=0; i<1; i++)
                {
                        unsigned int depth_pixel = (DETECT_BOX.BOX_OBSTACLE[i].x_center + DETECT_BOX.BOX_OBSTACLE[i].y_center*Cam_H)<<1;
                        DETECT_BOX.BOX_OBSTACLE[i].depth = pt[depth_pixel] + (pt[depth_pixel+1]<<8);
                        printf("object %d depth = %d \n",i,DETECT_BOX.BOX_OBSTACLE[i].depth);
                }
#endif
}

void Check_if_Reset_Rosbot_Pose(const gazebo_msgs::ModelStates::ConstPtr& msg)
{
        gazebo_msgs::ModelState model;
        geometry_msgs::Pose pose;
        model.pose = pose;
        int modelCount = msg->name.size();

        for(int modelIndex = 0; modelIndex<modelCount; ++modelIndex)
        {
                if(msg->name[modelIndex]=="turtlebot3_waffle")
                {
                        model.model_name = msg->name[modelIndex];
                        model.pose = msg->pose[modelIndex];
                        DETECT_BOX.current_abs_robot_z_pose = abs(model.pose.position.z);
                }
        }
}

void Sub_and_Dealwith_Odomdata(const nav_msgs::Odometry::ConstPtr &msg)
{
        float current_position_x = msg->pose.pose.position.x;
        float current_position_y = msg->pose.pose.position.y;
        float dist = sqrtf(((current_position_x-DETECT_BOX.x_goal)*(current_position_x-DETECT_BOX.x_goal))+((current_position_y-DETECT_BOX.y_goal)*(current_position_y-DETECT_BOX.y_goal)));
        DETECT_BOX.odom_diff = dist;
        float current_vel_linear = msg->twist.twist.linear.x;
        float current_vel_angular = msg->twist.twist.angular.z;
        DETECT_BOX.linear_vel = current_vel_linear;
        DETECT_BOX.angular_vel = current_vel_angular;
}

/*
void Find_Model_Name(const gazebo_msgs::ModelStates::ConstPtr &msg)
{
        msg.model_name = "turtlebot3_waffle";
}
*/