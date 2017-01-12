#include <ros/ros.h>
#include <iostream>
#include <geometry_msgs/Quaternion.h>
// #include <std_msgs/MultiArrayLayout.h>
// #include <std_msgs/MultiArrayDimension.h>
// #include <std_msgs/Float64MultiArray.h>
using namespace std;
int main(int argc, char **argv) {
  ros::init(argc, argv, "talker");
  ros::NodeHandle nh;

  ros::Publisher chatter_pub = nh.advertise<geometry_msgs::Quaternion>("route", 1000);
  int i=0;
  ros::Rate loop_rate(1);
  while(ros::ok()) {
    cout << "test" << endl;
      geometry_msgs::Quaternion msg;
      msg.x = 100.0;
      msg.y = 10.0;
      msg.z = 1.0;
      msg.w = 100.0;
      chatter_pub.publish(msg);

    ros::spinOnce();
    loop_rate.sleep();
    i++;
  }
  return 0;
}
