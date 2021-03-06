//SD Card Reader
#include <SPI.h>
#include <SD.h>

const int chipSelect = 7;

///Sensors
#define LEFT_IR A0
#define CENTER_IR A2
#define RIGHT_IR A1

//MAGNET
#define magnetSen 8
//Magnet LED
#define magnetLED 6

///Turn Indicators
#define LEFT_LED 4
#define RIGHT_LED 2

//motors
#define LEFT_MOTOR 5
#define RIGHT_MOTOR 3

int leftS = 250, rightS = 40;

void setup() {
  Serial.begin(9600);
  pinMode(LEFT_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);
  pinMode(CENTER_IR, INPUT);
  pinMode(LEFT_LED, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);
  pinMode(LEFT_MOTOR, OUTPUT);
  pinMode(RIGHT_MOTOR, OUTPUT);
  //MAGNET
  pinMode(magnetSen, INPUT);
  pinMode(magnetLED, OUTPUT);
  //SD Card Reader
  pinMode(chipSelect, OUTPUT);
  Serial.print("Initilize Card...");
  if (SD.begin())
    Serial.println("Card Works");
  else
    Serial.println("Card Failed");

  int leftSum = 0, rightSum = 0;
  for (int i = 0; i < 20; i++) {
    leftSum += analogRead(LEFT_IR);
    rightSum += analogRead(RIGHT_IR);
  }
  leftS = (leftSum / 20) + 100;
  rightS = (rightSum / 20) + 100;
  leftS = map(leftS, 0, 1023, 1023, 0);
  rightS = map(rightS, 0, 1023, 1023, 0);

  delay(1000);
  analogWrite(LEFT_MOTOR, 155); //what speed 1 and speed 2 used to bw
  analogWrite(RIGHT_MOTOR, 120);
  delay(50);
}

double getCorrection(int err);
int leftValue, rightValue, centerValue;
double KD=1.4,KP=0.9; //changed from int to double to fine tune
int dt=10;
int pError=0;
int correction = 0, pCorrection = 0;
int counter[2] = {0,0};
//MAGNET
int magnet = 0;

int s1, s2;

//bring motor speeds down
int speed1 = 100; 
int speed2 = 77; //give the right s bit more life : prev 77

//SD Card Reader
String LeftString = "", RightString = "", MagnetString = "";
int analogPin = 23, analogPin1 = 24, analogPin2 = 8;

void loop() {
  //turn off the turn LEDs
  digitalWrite(LEFT_LED, LOW);
  digitalWrite(RIGHT_LED, LOW);

  //MAGNET
  magnet = digitalRead(magnetSen);
  
  //get the values for how far the bot is off the line
  leftValue = analogRead(LEFT_IR); //- leftS
  rightValue = analogRead(RIGHT_IR); //- rightS
  leftValue = map(leftValue, 0, 1023, 1023, 0);
  rightValue = map(rightValue, 0, 1023, 1023, 0);

  //get the values for if the bot is on the line
  centerValue = analogRead(CENTER_IR);
  centerValue = map(centerValue, 0, 1023, 1023, 0);

  //MAGNET
  if (magnet == 1){
    Serial.print("Magnet Detected\n");
    digitalWrite(magnetLED, HIGH);
    int sensor3 = analogRead(analogPin2);
    MagnetString = String(sensor3);
  }else{
    Serial.print("Magnet Waiting\n");
    digitalWrite(magnetLED, LOW);
  }
  
  //acquiring distances from the line looollll  -NEED TO COLLECT APPROPRIATE VALUES FOR leftS and rightS
  int lerr = -leftValue + leftS; // opposite negatives
  int rerr = rightValue - rightS;
  int err = (lerr+rerr)/2;
  
//  Serial.println(centerValue);

  //getting and applying the correction - check signs of correction?
  int correction = (int) getCorrection(err);
  if (centerValue > 55) { 
    s1 = speed1+((correction*1)/5); // left motor 2/3
    s2 = speed2-((correction*1)/4); // right motor
  } else if (centerValue > 0) { // right turn
    s1 = speed1+((correction*1)/2); // left motor gets more power to turn right
    s2 = speed2-((correction*1)/2); // right motor
  } else if (centerValue < 0) { // left turn
    s1 = speed1+((correction*1)/2); // left motor
    s2 = speed2-((correction*1)/2); // right motor gets more power to turn left
  }
 
  //make sure the speed change isn't too drastic
  if(s1<0)
    s1 = 105;
  else if(s1>220)
    s1 = 220;
  if(s2<0)
    s2 = 85;
  else if(s2>200)
    s2 = 200;

  //onwards we go

  //apply the speed change
  analogWrite(LEFT_MOTOR, s1);
  analogWrite(RIGHT_MOTOR, s2);

  //if leftValue is high, then turn on the left LED
  //if rightValue is high, then turn on the right LED
  if (err < -1) { //dummy values
    digitalWrite(LEFT_LED, HIGH);
    int sensor = analogRead(analogPin); //left sensor
    LeftString += String(sensor);
  }
  if (err > 1) { //dummy values
    digitalWrite(RIGHT_LED, HIGH);  
    int sensor1 = analogRead(analogPin1); //right sensor
    RightString += String(sensor1);
  }

  //SD Card Reader
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  
  if (dataFile){
    dataFile.println(LeftString);
    dataFile.println(RightString);
    dataFile.println(MagnetString);
    dataFile.close();
  }else{
    Serial.println("error opening datalog.txt");
  }
}

double getCorrection(int err){ //used to be int
  int derivative = (pError-err)/dt;
  pError = err;
  double cor = KP* (double) err + KD*(double) derivative;
  //return KP* err + KD*derivative;
  return cor;
}
