/*デイジーチェン接続 
Arduino11番ピン（MOSI）- 6番ピン(SDI) L6470(一台目) 3番ピン(SDO) - (SDI)L6470(二台目)(SDO) - 12番ピン（MISO) Arduino
SCK 共通
SS - CS共通
*/
#include <SPI.h>
#include <MsTimer2.h>
#include <stdio.h>
#include <ros.h>
#include <geometry_msgs/Quaternion.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Empty.h>
#include <std_msgs/String.h>

// ピン定義。
#define PIN_SPI_MOSI 11
#define PIN_SPI_MISO 12
#define PIN_SPI_SCK 13
#define PIN_SPI_SS 10
#define PIN_BUSY 9
#define PIN_BUSY2 8

ros::NodeHandle nh;

//std_msgs::String msg5;
//ros::Publisher chatter3 ("chatter3", &msg5);
geometry_msgs::Quaternion msg2;
ros::Publisher chatter("chatter", &msg2);
std_msgs::Int8 msg4;
ros::Publisher chatter2("chatter2", &msg4);
std_msgs::Empty msg3;
ros::Publisher done("done", &msg3);
void tapCallback(const std_msgs::Empty& msg) {
  digitalWrite(2, HIGH);
  delay(100);
  digitalWrite(2, LOW);
  delay(100);
}

boolean executing = false;

void arrayMessageCallback(const std_msgs::Float32MultiArray& msg) {
  if (executing) return;
  executing = true;
  int i, j;
  msg4.data = msg.data[0];
  //msg5.data = "start";
  //chatter3.publish(&msg5);
  chatter2.publish(&msg4);
  for (i=1;i<msg.data[0];i++) {
    if (i == 3) {
      digitalWrite(2, HIGH);        
    }
    if (i == msg.layout.dim[0].size-2) {
      digitalWrite(2, LOW);
    }
    int s = msg.data[i];
    msg4.data = s;
    chatter2.publish(&msg4);
    if (i & 1) {
      for (j=0;j<abs(s);j++) {
        if (s > 0) {
          L6470_move2(0, 1);
        } else {
          L6470_move2(1, 1);
        }
        delay(10);
      }
    } else {
      for (j=0;j<abs(s);j++) {
        if (s > 0) {
          L6470_move(0, 1);
        } else {
          L6470_move(1, 1);
        }
        delay(10);
      }
    }
    delay(100);
  }
  delay(1000);
  done.publish(&msg3);
  executing = false;
}

void messageCallback(const geometry_msgs::Quaternion& msg) {
 // x, y
 // z for pushing solenoid
 // w for duration
 
 if (msg.z > 0) {
   digitalWrite(2, HIGH);
 } else {
   digitalWrite(2, LOW);
 }
 if (msg.x > 0) {
   L6470_move2(0, msg.x);
 } else {
   L6470_move2(1, -msg.x);
 }
 if (msg.y > 0) {
   L6470_move(0, msg.y);
 } else {
   L6470_move(1, -msg.y);
 }
 delay(msg.w);
 msg2.x = msg.x * 2;
 msg2.y = msg.y * 2;
 msg2.z = msg.z * 2;
 msg2.w = msg.w * 2;
 //chatter.publish(&msg2);
}

ros::Subscriber<geometry_msgs::Quaternion> sub("route", messageCallback);
ros::Subscriber<std_msgs::Empty> sub2("tap", tapCallback);
ros::Subscriber<std_msgs::Float32MultiArray> sub3("route2", arrayMessageCallback);

void setup()
{
  pinMode(2, OUTPUT);
  pinMode(PIN_SPI_MOSI, OUTPUT);
  pinMode(PIN_SPI_MISO, INPUT);
  pinMode(PIN_SPI_SCK, OUTPUT);
  pinMode(PIN_SPI_SS, OUTPUT);
  pinMode(PIN_BUSY, INPUT_PULLUP);
  pinMode(PIN_BUSY2, INPUT_PULLUP);
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
  Serial.begin(38400);
  digitalWrite(PIN_SPI_SS, HIGH);
  
  L6470_resetdevice(); //1台目のL6470リセット
  L6470_resetdevice2(); //2台目のL6470リセット
  L6470_setup();  //1台目のL6470を設定 
  L6470_setup2();  //2台目のL6470を設定 
  L6470_getstatus(); //1台目のフラグ解放
  L6470_getstatus2();//2台目のフラグ解放
     
  //MsTimer2::set(25, fulash);//シリアルモニター用のタイマー割り込み
  //MsTimer2::start();
  nh.initNode();
 // nh.advertise(chatter);
  nh.advertise(chatter2);
  //nh.advertise(chatter3);
  nh.advertise(done);
  nh.subscribe(sub);
  nh.subscribe(sub2);
  nh.subscribe(sub3);
/*  
//1台目正転
L6470_move(1,800);//1台目を正転方向に800ステップ　(現在1/4マイクロステップ設定なので、フルステップの200ステップ分になる)
L6470_busydelay(1000);//1台目の動作終了から１秒待つ

//２台目正転
L6470_move2(1,800);//2台目を正転方向に800ステップ　()
L6470_busydelay2(1000);//2台目の動作終了から１秒待つ

//同時正転
for(int i=0;i<3;i++){
L6470_move(1,800);
L6470_move2(1,800);
L6470_busydelay(300);
}
delay(2000);

//同時逆転
for(int i=0;i<3;i++){
L6470_move(0,800);
L6470_move2(0,800);
L6470_busydelay(300);
}
delay(3000);

//同時　順次　正逆転
L6470_move(1,800);
L6470_move2(1,800);
L6470_move(0,800);
L6470_move2(0,800);
L6470_move(1,800);
L6470_move2(1,800);
delay(3000);

//交互に順次動作
for(int i=0;i<4;i++){
L6470_move(0,200);
L6470_busydelay(0);
L6470_move2(0,200);
L6470_busydelay2(0);
}
delay(3000);

//１テンポ遅れて順次動作
L6470_move(0,200);
L6470_busydelay(0);
for(int i=0;i<3;i++){
L6470_move2(0,200);
L6470_move(0,200);
}
L6470_move2(0,200);
L6470_busydelay2(0);
delay(3000);

//連続回転
L6470_run(1,0x1234);//69step/sの速度で正転
delay(5000);
L6470_run2(1,0x5678);//332step/sの速度で正転
delay(6000);

//回転停止
 L6470_softstop();//1台目回転停止
 L6470_softstop2();//2台目回転停止
delay(5000);

//指定座標に移動
 L6470_goto(0x4321);//座標0x004321に最短でいける回転方向で移動[B]
 L6470_goto2(0x8765);//座標0x008765に最短でいける回転方向で移動[B]
 L6470_busydelay(0);//　　　　　　↓
 L6470_busydelay2(3000);//２台とも動作が完了してから３秒待つ
 
 //原点座標に移動
 L6470_gohome();//座標原点0x000000に移動[B]
 L6470_gohome2();
 
 //原点につき次第　消磁
 L6470_hardhiz();
 L6470_hardhiz2();
 
 */
}

void loop(){
  nh.spinOnce();
  delay(1);
}

void L6470_setup(){
L6470_setparam_acc(0x30); //[R, WS] 加速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
L6470_setparam_dec(0x30); //[R, WS] 減速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
L6470_setparam_maxspeed(0x2a); //[R, WR]最大速度default 0x041 (10bit) (15.25*val+15.25[step/s])
L6470_setparam_minspeed(0x1200); //[R, WS]最小速度default 0x000 (1+12bit) (0.238*val[step/s])
L6470_setparam_fsspd(0x027); //[R, WR]μステップからフルステップへの切替点速度default 0x027 (10bit) (15.25*val+7.63[step/s])
L6470_setparam_kvalhold(0x28); //[R, WR]停止時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6470_setparam_kvalrun(0x28); //[R, WR]定速回転時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6470_setparam_kvalacc(0x28); //[R, WR]加速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6470_setparam_kvaldec(0x28); //[R, WR]減速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)

L6470_setparam_stepmood(0x02); //ステップモードdefault 0x07 (1+3+1+3bit)
}

void L6470_setup2(){
L6470_setparam_acc2(0x30); //[R, WS] 加速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
L6470_setparam_dec2(0x30); //[R, WS] 減速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
L6470_setparam_maxspeed2(0x2a); //[R, WR]最大速度default 0x041 (10bit) (15.25*val+15.25[step/s])
L6470_setparam_minspeed2(0x1200); //[R, WS]最小速度default 0x000 (1+12bit) (0.238*val[step/s])
L6470_setparam_fsspd2(0x027); //[R, WR]μステップからフルステップへの切替点速度default 0x027 (10bit) (15.25*val+7.63[step/s])
L6470_setparam_kvalhold2(0x28); //[R, WR]停止時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6470_setparam_kvalrun2(0x28); //[R, WR]定速回転時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6470_setparam_kvalacc2(0x28); //[R, WR]加速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6470_setparam_kvaldec2(0x28); //[R, WR]減速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)

L6470_setparam_stepmood2(0x02); //ステップモードdefault 0x07 (1+3+1+3bit)
}


void fulash(){
  

long a=L6470_getparam_abspos();
long b=L6470_getparam_speed();
long c=L6470_getparam_abspos2();
long d=L6470_getparam_speed2();
char str[15];
snprintf(str,sizeof(str),"1pos=0x%6.6X ",a);
Serial.print(str);
snprintf(str,sizeof(str),"1spd=0x%5.5X ",b);
Serial.print(str);
snprintf(str,sizeof(str),"2pos=0x%6.6X ",c);
Serial.print(str);
snprintf(str,sizeof(str),"2spd=0x%5.5X ",d);
Serial.println(str);
  
 /* Serial.print("0x");
  Serial.print( L6470_getparam_abspos(),HEX);
  Serial.print(" 0x");
  Serial.print( L6470_getparam_speed(),HEX);
  Serial.print(" 0x");
  Serial.print( L6470_getparam_abspos2(),HEX);
  Serial.print(" 0x");
  Serial.println( L6470_getparam_speed2(),HEX);
  */
}


