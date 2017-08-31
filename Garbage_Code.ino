///Sensors
#define LEFT_IR A0
#define RIGHT_IR A1

///Turn Indicators
#define LEFT_LED 4
#define RIGHT_LED 2

//motors
#define LEFT_MOTOR 5
#define RIGHT_MOTOR 3


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LEFT_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);
  pinMode(LEFT_LED, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);
  pinMode(LEFT_MOTOR, OUTPUT);
  pinMode(RIGHT_MOTOR, OUTPUT);
  delay(1000);
}

double getCorrection(int err);
int leftValue, rightValue;
int leftS = 245, rightS = 45;
double KD=0.9,KP=0.8; //changed from int to double to fine tune
int dt=10;
int speed1 = 98; 
int speed2 = 125; 
int pError=0;
//int integral=0;
int correction = 0;

void loop() {
  //turn off the turn LEDs
  digitalWrite(LEFT_LED, LOW);
  digitalWrite(RIGHT_LED, LOW);
  
  //get the values for how far the bot is off the line
  leftValue = analogRead(LEFT_IR) - 200;
  rightValue = analogRead(RIGHT_IR);
  leftValue = map(leftValue, 0, 1023, 1023, 0);
  rightValue = map(rightValue, 0, 1023, 1023, 0);

  //print the error for testing purposes
  Serial.print("Left: ");
  Serial.println(leftValue);
  Serial.print("Right: ");
  Serial.println(rightValue);
  
  //acquiring distances from the line looollll  -NEED TO COLLECT APPROPRIATE VALUES FOR leftS and rightS
  int lerr = -leftValue + leftS;
  int rerr = rightValue - rightS;
  int err = (lerr+rerr)/2;

  //getting and applying the correction - check signs of correction?
  int correction = (int) getCorrection(err);
  int s1 = speed1+correction;
  int s2 = speed2-correction; 

  //make sure the speed change isn't too drastic
  if(s1<0)
    s1 = 98;
  else if(s1>250)
    s1 = 250;
  if(s2<0)
    s2 = 125;
  else if(s2>250)
    s2 = 250;

  //Serial.println(err);
  //onwards we go

  //apply the speed change
  analogWrite(LEFT_MOTOR, s1);
  analogWrite(RIGHT_MOTOR, s2);


  //if leftValue is high, then turn on the left LED
  //if rightValue is high, then turn on the right LED
  if (err < -1) { //dummy values
    digitalWrite(LEFT_LED, HIGH);
  }
  if (err > 1) { //dummy values
    digitalWrite(RIGHT_LED, HIGH);  
  }
  
  delay(dt);
}

double getCorrection(int err){ //used to be int
  int derivative = (pError-err)/dt;
  pError = err;
  //integral += err*dt;
  double cor = KP* (double) err + KD*(double) derivative;
  //return KP* err + KD*derivative;
  return cor;
}
