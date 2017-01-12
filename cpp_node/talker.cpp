#include <ros/ros.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/Float64MultiArray.h>

int main(int argc, char **argv) {
  ros::init(argc, argv, "talker");
  ros::NodeHandle nh;

  ros::Publisher chatter_pub = nh.advertise<std_msgs::Float64MultiArray>("talker", 1000);
  ros::Rate loop_rate(10);

  std_msgs::Float64MultiArray msg;

  msg.layout.dim.push_back(std_msgs::MultiArrayDimension());
  msg.layout.dim[0].size = 4;
  msg.layout.dim[0].stride = 1;
  msg.layout.dim[0].label = "x";
  msg.data.push_back(10);
  msg.data.push_back(0);
  msg.data.push_back(-1);
  msg.data.push_back(100);
  msg.data.push_back(10);
  msg.data.push_back(0);
  msg.data.push_back(1);
  msg.data.push_back(200);
  msg.data.push_back(-20);
  msg.data.push_back(0);
  msg.data.push_back(1);
  msg.data.push_back(300);
  msg.data.push_back(30);
  msg.data.push_back(0);
  msg.data.push_back(-1);
  msg.data.push_back(200);
  chatter_pub.publish(msg);
  ros::spinOnce();
  loop_rate.sleep();
}
