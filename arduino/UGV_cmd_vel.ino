#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int8.h>

#define steering_PWM_RR  8
#define steering_PWM_RL  9
#define steering_PWM_LR  4
#define steering_PWM_LL  5

#define motor_FR_R 10
#define motor_FR_L 11

#define motor_FL_R 45
#define motor_FL_L 46

#define motor_BR_R 6
#define motor_BR_L 7

#define motor_BL_R 12
#define motor_BL_L 13

#define MX_lvl 1000
#define MN_lvl -1000
#define pot1 A0

#define encoderPin1 2
#define encoderPin2 3
#define encoderPin3 18
#define encoderPin4 19

char button1;
char button2;
int angular;
static int last_angle;
int angle = 0 ;
int angle_new;
int angle_aimed= 0;


volatile int lastEncoded_left = 0;
volatile int lastEncoded_right = 0;
volatile long encoderValue = 0;
int encoder_left  = 0;
int encoder_right = 0;

ros::NodeHandle  nh;
ros::Subscriber <geometry_msgs::Twist> sub("/cmd_vel",&roverCallBack);
ros::Subscriber<std_msgs::Int8> sub2("/buttons", &steering_calibration);

void setup()
{
//  Serial1.begin(9600);
  pinMode(steering_PWM_RR, OUTPUT);
  pinMode(steering_PWM_RL, OUTPUT);
  pinMode(steering_PWM_LR, OUTPUT);
  pinMode(steering_PWM_LL, OUTPUT);

  pinMode(motor_FR_R, OUTPUT);
  pinMode(motor_FR_L, OUTPUT);
  pinMode(motor_FL_R, OUTPUT);
  pinMode(motor_FL_L, OUTPUT);
  pinMode(motor_BR_R, OUTPUT);
  pinMode(motor_BR_L, OUTPUT);
  pinMode(motor_BL_R, OUTPUT);
  pinMode(motor_BL_L, OUTPUT); 

  pinMode(encoderPin1, INPUT_PULLUP);
  pinMode(encoderPin2, INPUT_PULLUP);
  pinMode(encoderPin3, INPUT_PULLUP);
  pinMode(encoderPin4, INPUT_PULLUP);

  digitalWrite(encoderPin1, HIGH);
  digitalWrite(encoderPin2, HIGH);
  digitalWrite(encoderPin3, HIGH);
  digitalWrite(encoderPin4, HIGH);

  analogWrite(steering_PWM_RR, 0);
  analogWrite(steering_PWM_RL, 0);
  analogWrite(steering_PWM_RR, 0);
  analogWrite(steering_PWM_RL, 0);

//  attachInterrupt(0 , updateEncoder_left , CHANGE);
//  attachInterrupt(1 , updateEncoder_left , CHANGE);
//  attachInterrupt(18, updateEncoder_right, CHANGE);
//  attachInterrupt(19, updateEncoder_right, CHANGE);
  pinMode(LED_BUILTIN, OUTPUT);

  nh.initNode();

  nh.subscribe(sub);
  nh.subscribe(sub2);

  right1_test();delay(1000);
}

void loop() {

//  Serial1.print("angular = ");
//  Serial1.println(angular);
  encoder_left = constrain(encoder_left, -4800, 4800);
  encoder_right = constrain(encoder_right, -4800, 4800);
  if(encoder_left==4800 || encoder_left==-4800 || encoder_right==-4800 || encoder_right==-4800 )
  {
    encoder_left =0 ;
    encoder_right =0 ;
  }
 
  if(encoder_left >= angle_new-5 && encoder_left <= angle_new+5 && encoder_right >= angle_new-5 && encoder_right <= angle_new+5  ) 
  {
    Stopsteering();
    last_angle = angle_new;
  }

  //if(encoder_left <= angle_new-5 && encoder_right <= angle_new-5 )
      if(angular > 20) right();
  //else if(encoder_left >= angle_new+5 && encoder_right <= angle_new-5 )
      else if(angular < -20) left(); 
      else Stopsteering();


  if(button1 && angular<0) left1_test();
  if(button1 && angular>0) right1_test();
  if(button2 && angular<0) left2_test();
  if(button2 && angular>0) right2_test();
   nh.spinOnce();
}



/////////////////////// *** MOVING FUNCTIONS *** ////
void right () {
  
  analogWrite(steering_PWM_RR, 255);
  analogWrite(steering_PWM_RL, 0);
  analogWrite(steering_PWM_LR, 0);
  analogWrite(steering_PWM_LL, 255);

}

void left () {
  analogWrite(steering_PWM_RR, 0);
  analogWrite(steering_PWM_RL, 255);
  analogWrite(steering_PWM_LR, 255);
  analogWrite(steering_PWM_LL, 0);

}

void left1_test(){
  analogWrite(steering_PWM_RR, 0);
  analogWrite(steering_PWM_RL, 255);
}

void right1_test(){
  analogWrite(steering_PWM_RR, 255);
  analogWrite(steering_PWM_RL, 0);
}
void left2_test(){
  analogWrite(steering_PWM_LR, 0);
  analogWrite(steering_PWM_LL, 255);
}

void right2_test(){
  analogWrite(steering_PWM_LR, 255);
  analogWrite(steering_PWM_LL, 0);
}
void Stopsteering () {
  analogWrite(steering_PWM_RR, LOW);
  analogWrite(steering_PWM_RL, LOW);
  analogWrite(steering_PWM_LR, LOW);
  analogWrite(steering_PWM_LL, LOW);
}

void back( char rate){
  analogWrite(motor_FR_R, LOW);
  analogWrite(motor_FR_L, rate);
  analogWrite(motor_FL_R, LOW);
  analogWrite(motor_FL_L, rate);
  analogWrite(motor_BR_R, rate);
  analogWrite(motor_BR_L, LOW);
  analogWrite(motor_BL_R, rate);
  analogWrite(motor_BL_L, LOW);
}

void forward(char rate){
  analogWrite(motor_FR_R, rate);
  analogWrite(motor_FR_L, LOW);
  analogWrite(motor_FL_R, rate);
  analogWrite(motor_FL_L, LOW);
  analogWrite(motor_BR_R, LOW);
  analogWrite(motor_BR_L, rate);
  analogWrite(motor_BL_R, LOW);
  analogWrite(motor_BL_L, rate);
}

void Stop(){
  analogWrite(motor_FR_R, LOW);
  analogWrite(motor_FR_L, LOW);
  analogWrite(motor_FL_R, LOW);
  analogWrite(motor_FL_L, LOW);
  analogWrite(motor_BR_R, LOW);
  analogWrite(motor_BR_L, LOW);
  analogWrite(motor_BL_R, LOW);
  analogWrite(motor_BL_L, LOW);
}


void updateEncoder_left() {
  int MSB = digitalRead(encoderPin1);
  int LSB = digitalRead(encoderPin2);

  int encoded = (MSB << 1) | LSB;
  int sum  = (lastEncoded_left << 2) | encoded;

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoder_left ++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoder_left --;
  lastEncoded_left = encoded;
}

void updateEncoder_right() {
  int MSB = digitalRead(encoderPin1);
  int LSB = digitalRead(encoderPin2);

  int encoded = (MSB << 1) | LSB;
  int sum  = (lastEncoded_right << 2) | encoded;

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoder_right ++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoder_right --;
  lastEncoded_right = encoded;
}

//ros call back function
void roverCallBack( const geometry_msgs::Twist &msg){
   angular = msg.angular.z;
   angle = map(msg.angular.z, -90, 90, -4800 , 4800);
   angle = constrain(angle, -1000, 1000);

   if(msg.linear.x > 0)
   {
//    Serial1.print("forward = ");
//    Serial1.println(msg.linear.x);
      forward(msg.linear.x);
   }
   else if (msg.linear.x < 0){
      back(msg.linear.x*(-1));
//      Serial1.print("back = ");
//      Serial1.println(msg.linear.x);
   }   
   else{
    Stop();
//    Serial1.println("stop linear");
   }

}

void steering_calibration(const std_msgs::Int8 &msg){
unsigned char recived_msg = msg.data; 
button1 = (recived_msg & 0x01);
button2 = ((recived_msg & 0x02) >> 1);
digitalWrite(LED_BUILTIN,button2);
//if(button1){
////  Serial1.println("b1");
//}
//if(button2){
////  Serial1.println("b2");
//}
}
