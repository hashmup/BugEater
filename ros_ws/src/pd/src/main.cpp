#include <ros/ros.h>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/MultiArrayDimension.h>
#include <geometry_msgs/Quaternion.h>
#include "search.h"
#include "control.h"
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;

bool executing = false;

class SubscribeAndPublish {
public:
  SubscribeAndPublish() {
    pub1 = nh.advertise<geometry_msgs::Quaternion>("route", 2.5);
    pub2 = nh.advertise<geometry_msgs::Quaternion>("route", 0.3);
    pub3 = nh.advertise<std_msgs::Float32MultiArray>("route2", 0.1);
    pub4 = nh.advertise<std_msgs::Float32MultiArray>("setup", 1.0);
    sub = nh.subscribe("board", 10, &SubscribeAndPublish::boardCallback2, this);
    executing = true;
  /*  ros::Rate loop_rate(1.0);
    std_msgs::Float32MultiArray msgs = ctl.setup();
    for(int i=0;i<10;i++) {
      pub4.publish(msgs);
      loop_rate.sleep();
    }
    sleep(60);
    */
    executing = false;
  }
  void boardCallback(const std_msgs::Float32MultiArray::ConstPtr& msg) {
    if (!executing) {
      executing = true;
      ros::Rate loop_rate(2.5);
      ros::Rate loop_rate2(0.3);
      int* board = (int*)calloc(sizeof(int), 30);
      for (int i=0;i<30;i++) {
        board[i] = msg->data[i];
        cout << " " << board[i];
      }
      cout << endl;
      Search* search = new Search(board, 0, 0);
      Board* best = search->beam_search(6);

      best->disp_path(board);
      cout << best->score << endl;
      cout << best->combo_num << endl;
      vector<geometry_msgs::Quaternion> msgs = ctl.getMsgs(best->path);
      for (int i=0;i<msgs.size();i++) {
        if (i <= 1 || i >= msgs.size()-2) {
          loop_rate2.sleep();
          pub2.publish(msgs[i]);
          loop_rate2.sleep();
        } else {
          loop_rate.sleep();
          pub1.publish(msgs[i]);
        }
        loop_rate.sleep();
        cout << "step:" << i << endl;
      }
      sleep(20);
      executing = false;
    }
  }

  void boardCallback2(const std_msgs::Float32MultiArray::ConstPtr& msg) {
    if (!executing) {
      executing = true;
      ros::Rate loop_rate(0.1);
      int* board = (int*)calloc(sizeof(int), 30);
      for (int i=0;i<30;i++) {
        board[i] = msg->data[i];
        cout << " " << board[i];
      }
      cout << endl;
      Search* search = new Search(board, 0, 0);
      Board* best = search->beam_search(6);

      best->disp_path(board);
      cout << best->score << endl;
      cout << best->combo_num << endl;
      std_msgs::Float32MultiArray msgs = ctl.getMsgs2(best->path);
      for(int i=0;i<100;i++) {
        pub3.publish(msgs);
        loop_rate.sleep();
      }
      sleep(20);
      executing = false;
    }
  }
private:
  Control ctl;
  ros::NodeHandle nh;
  ros::Subscriber sub;
  ros::Publisher pub1;
  ros::Publisher pub2;
  ros::Publisher pub3;
  ros::Publisher pub4;
};

int main(int argc, char** argv) {
  ros::init(argc, argv, "mainNode");
  SubscribeAndPublish SAPObject;
  ros::spin();
  /*
  int bd[30] = {1, 2, 3, 4 ,5, 6, 1, 2, 3, 4 ,5, 6, 6, 1, 2, 3, 4 ,5, 1, 2, 3, 4 ,5, 6, 1, 2, 3, 4 ,5, 6};
  // Search* search = new Search(bd, 0, 0);
  Search* search = new Search("sample.dat", 0, 0);
  clock_t start = clock();    // スタート時間
  Board* best = search->beam_search(10);
  clock_t end = clock();     // 終了時間
  cout << "duration = " << (double)(end - start) / CLOCKS_PER_SEC << "sec.\n";
  // best->print_board();
  // best->print_simulate_board();
  // best->print_path();
  cout << best->score << endl;
  cout << best->combo_num << endl;
  // best->disp(false, 5000);
  best->disp_path(search->load_from_file("sample.dat"));
  int cnt = 1;
  while(cnt) {
    best->_mark_combo();
    best->disp(true, 1000);
    cnt = best->_delete_drop();
    best->disp(true, 1000);
    best->_fill_drop();
    best->disp(true, 1000);
  }
  */
}
