#include "SPI.h"
#include "MsTimer2.h"
#include <ros.h>
#include <geometry_msgs/Quaternion.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Int8.h>

// ピン定義。
#undef PIN_SPI_MOSI
#undef PIN_SPI_MISO
#undef PIN_SPI_SCK
#undef PIN_SPI_SS
#undef PIN_BUSY
#define PIN_SPI_MOSI 11 // 7
#define PIN_SPI_MISO 12 // 5
#define PIN_SPI_SCK 13 // 6
#define PIN_SPI_SS 10 // 8
#define PIN_BUSY 9

//#define PIN_SPI_MOSI 5 // 7
//#define PIN_SPI_MISO 6 // 5
//#define PIN_SPI_SCK 7 // 6
//#define PIN_SPI_SS 4 // 8
//#define PIN_BUSY 3

ros::NodeHandle nh;

void messageCb(const std_msgs::Empty& toggle_msg) {
  digitalWrite(13, HIGH-digitalRead(13));
}

geometry_msgs::Quaternion msg2;
ros::Publisher chatter2("chatter2", &msg2);

void messageCallback(const geometry_msgs::Quaternion& msg) {
  // x 
  // y 
  // z for pushing solenoid
  // w for duration
  
  if (msg.z > 0) {
   digitalWrite(2, HIGH);
   } else {
   digitalWrite(2, LOW);
   }
   if (msg.x > 0) {
   L6470_move(0, 1000*msg.x);
   } else {
   L6470_move(1, -1000*msg.x);
   }
   delay(msg.w);
   L6470_softstop();

  msg2.x = msg.x * 2;
  msg2.y = msg.y * 2;
  msg2.z = msg.z * 2;
  msg2.w = msg.w * 2;
  chatter2.publish(&msg2);
}

void pushSolenoid(const std_msgs::Int8& msg) {

}

ros::Subscriber<geometry_msgs::Quaternion> sub2("route", messageCallback);
ros::Subscriber<std_msgs::Empty> sub("toggle_led", messageCb);
std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);
char hello[13] = "hello world!";
void setup()  
{

  //delay(1000);
  pinMode(2, OUTPUT);
  pinMode(PIN_SPI_MOSI, OUTPUT);
  pinMode(PIN_SPI_MISO, INPUT);
  pinMode(PIN_SPI_SCK, OUTPUT);
  pinMode(PIN_SPI_SS, OUTPUT);
  pinMode(PIN_BUSY, INPUT);

  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);

  Serial.begin(9600);
  digitalWrite(PIN_SPI_SS, HIGH);

  pinMode(13, OUTPUT);
  L6470_resetdevice(); //残留コマンドの削除とリセット
  L6470_setup(); //L6470を設定

  //MsTimer2::set(100, fulash);
  //MsTimer2::start();
  //delay(2000);
  nh.initNode();
  nh.advertise(chatter);
  nh.advertise(chatter2);
  nh.subscribe(sub);
  nh.subscribe(sub2);
}

void loop(){
  //testDrive();
  //rotateTest();
  str_msg.data = hello;
  chatter.publish(&str_msg);
  nh.spinOnce();
  delay(500);
  //rotate();
  //digitalWrite(2, LOW);
  //delay(100);
  //digitalWrite(2, HIGH);
  //delay(100);
}

void rotate() {
  L6470_move(1, 50000);
  delay(1000);
  L6470_softstop();
  L6470_move(0, 50000);
  delay(1000);
  L6470_softstop();

}

void rotateTest(){
  L6470_move(1,16000);
  L6470_hardhiz(); //回転急停止、保持トルクなし
  L6470_move(0,16000);
  L6470_hardhiz(); //回転急停止、保持トルクなし
}

void testDrive(){
  L6470_run(0,50000);
  delay(10000);
  L6470_softstop();
  L6470_busydelay(500);
  L6470_run(1,16000);
  delay(1);
  L6470_softstop();
  L6470_busydelay(500);
}

void L6470_setup(){
  //L6470_setparam_acc(0x40); //default 0x08A (12bit) (14.55*val+14.55[step/s^2])
  L6470_setparam_acc(0x08A); //default 0x08A (12bit) (14.55*val+14.55[step/s^2])
  //L6470_setparam_dec(0x40); //default 0x08A (12bit) (14.55*val+14.55[step/s^2])
  L6470_setparam_acc(0x08A); //default 0x08A (12bit) (14.55*val+14.55[step/s^2])
  //L6470_setparam_maxspeed(0x40); //default 0x041 (10bit) (15.25*val+15.25[step/s])
  //L6470_setparam_maxspeed(0xFFFFFF); //default 0x041 (10bit) (15.25*val+15.25[step/s])
  // When half step // L6470_setparam_maxspeed(0x5C); //default 0x041 (10bit) (15.25*val+15.25[step/s])
  L6470_setparam_maxspeed(0xFFFFFF); //default 0x041 (10bit) (15.25*val+15.25[step/s])
  L6470_setparam_minspeed(0x00); //default 0x000 (1+12bit) (0.238*val[step/s])
  L6470_setparam_fsspd(0xff); //default 0x027 (10bit) (15.25*val+7.63[step/s])
  //L6470_setparam_fsspd(0x3ff); //default 0x027 (10bit) (15.25*val+7.63[step/s])
  //L6470_setparam_fsspd(0x027); //default 0x027 (10bit) (15.25*val+7.63[step/s])
  L6470_setparam_kvalhold(0x80); //default 0x29 (8bit) (Vs[V]*val/256)
  L6470_setparam_kvalrun(0xd0); //default 0x29 (8bit) (Vs[V]*val/256)
  L6470_setparam_kvalacc(0x80); //default 0x29 (8bit) (Vs[V]*val/256)
  L6470_setparam_kvaldec(0x60); //default 0x29 (8bit) (Vs[V]*val/256)

  //L6470_setparam_stepmood(0x03); //default 0x07 (1+3+1+3bit)
  L6470_setparam_stepmood(0x07); //default 0x07 (1+3+1+3bit)
}

void fulash(){
  Serial.print("0x");
  Serial.print( L6470_getparam_abspos(),HEX);
  Serial.print(" ");
  Serial.print("0x");
  Serial.println( L6470_getparam_speed(),HEX);
}

