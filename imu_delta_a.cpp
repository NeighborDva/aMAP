#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include "std_msgs/Float64.h"/
#include "sensor_msgs/Imu.h"
#include "geometry_msgs/Vector3.h" 
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <math.h>

double linear_acceleration_x_tmp = 0.0;
double linear_acceleration_x;
double linear_acceleration_x_delta;

int init_num=0;
std_msgs::Float64 sum ;
void imuCallback(const sensor_msgs::Imu::ConstPtr& msg) {



		
       linear_acceleration_x = msg-> linear_acceleration.x;
            //msg-> linear_acceleration.y,
		//msg-> linear_acceleration.z
 
       if(linear_acceleration_x != linear_acceleration_x_tmp)
       {
       linear_acceleration_x_delta = linear_acceleration_x - linear_acceleration_x_tmp;
       
       sum.data += linear_acceleration_x_delta;
        
     //  ROS_INFO("linear_acceleration_x: %lf",linear_acceleration_x);
	 //  ROS_INFO("linear_acceleration_x_tmp: %lf",linear_acceleration_x_tmp);
		//ROS_INFO("linear_acceleration_x_delta: %lf",linear_acceleration_x_delta);
       }
       linear_acceleration_x_tmp = linear_acceleration_x;
       
      /*ag=msg->angular_velocity.z;
      ag_d= (double)ag*180/3.14159;*/
      
  //ROS_INFO( "%lf",yaw_d);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "integ_sub");

	ros::NodeHandle n;
 
  
	
	ros::Subscriber subImu = n.subscribe("/handsfree/imu", 10, &imuCallback);
	ros::Publisher pub_deltaImu = n.advertise<std_msgs::Float64>("/delta_Imu_vel", 10);
 
	ros::Rate loop_rate(200);  // 10
  
	sum.data =0;
	while (ros::ok())
	{	
		pub_deltaImu.publish(sum);
		loop_rate.sleep();
		ros::spinOnce();
   
	}
  return 0;
}

