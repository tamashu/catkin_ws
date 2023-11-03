#include <ros/ros.h>
#include <sensor_msgs/JointState.h>

#include <string>
#include <math.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "vis_joint_publisher");
  ros::NodeHandle nh;
  ros::Publisher joint_pub = nh.advertise<sensor_msgs::JointState>("joint_states", 10);

  ros::Rate loop_rate(10);
  int count = 0;
  while (ros::ok())
  {
    sensor_msgs::JointState js0;
    js0.header.stamp = ros::Time::now();
    js0.name.resize(4);
    js0.name[0] = "joint_1_lf";
    js0.name[2] = "joint_2_lf";
    js0.name[3] = "joint_3_lf";
    js0.name[4] = "joint_4_lf";
    js0.position.resize(4);
    js0.position[0] = 1.0 * (float)count / 40.0;
    js0.position[1] = 1 * (float)count / 40.0;
    js0.position[2] = 1 * (float)count / 40.0;
    js0.position[3] = 1 * (float)count / 40.0;
    joint_pub.publish(js0);
    count++;

    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}