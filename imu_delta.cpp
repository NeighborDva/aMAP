#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include "std_msgs/Float64.h"/
#include "sensor_msgs/Imu.h"
#include "geometry_msgs/Vector3.h" 
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <math.h>

double roll,pitch,yaw,yaw_d;
double yaw_tmp, yaw_delta;
int init_num=0;
std_msgs::Float64 sum ;
void imuCallback(const sensor_msgs::Imu::ConstPtr& msg) {
	tf2::Quaternion q(
        msg->orientation.x,
        msg->orientation.y,
        msg->orientation.z,
        msg->orientation.w);
      tf2::Matrix3x3 m(q);
 
      m.getRPY(roll, pitch, yaw);
      
      yaw_d=yaw*180/3.14159;//+mag_north;
       ROS_INFO("yaw_d: %lf",yaw_d);
       ROS_INFO("yaw_tmp: %lf",yaw_tmp);
       
      if(yaw_d!=yaw_tmp)
      {
		  yaw_delta = yaw_d - yaw_tmp;
		  sum.data +=yaw_delta;
		  ROS_INFO("yaw_delta: %lf",yaw_delta); 
		  ROS_INFO("sum: %lf",sum.data);
	  }
	  yaw_tmp = yaw_d;
      
     
      
      /*ag=msg->angular_velocity.z;
      ag_d= (double)ag*180/3.14159;*/
      
  //ROS_INFO( "%lf",yaw_d);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "integ_sub");

	ros::NodeHandle n;
 
  
	roll = pitch = yaw = 0.0;  
	ros::Subscriber subImu = n.subscribe("/handsfree/imu", 10, &imuCallback);
	ros::Publisher pub_deltaImu = n.advertise<std_msgs::Float64>("/delta_Imu_vel", 10);
 
	ros::Rate loop_rate(10);  // 10
  
	sum.data =0;
	while (ros::ok())
	{	
		pub_deltaImu.publish(sum);
		loop_rate.sleep();
		ros::spinOnce();
   
	}
  return 0;
}

