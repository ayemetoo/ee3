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
  // put your setup code here, to run once:
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
  delay(1000);

  int leftSum = 0, rightSum = 0;
  for (int i = 0; i < 20; i++) {
    leftSum += analogRead(LEFT_IR);
    rightSum += analogRead(RIGHT_IR);
  }
  leftS = leftSum / 20;
  rightS = rightSum / 20;
}

double getCorrection(int err);
int leftValue, rightValue, centerValue;
double KD=0.9,KP=0.9; //changed from int to double to fine tune
int dt=10;
int speed1 = 98; 
int speed2 = 125; 
int pError=0;
//int integral=0;
int correction = 0, pCorrection = 0;
int counter[2] = {0,0};
//MAGNET
int magnet = 0;

int s1, s2;

void loop() {
  //turn off the turn LEDs
  digitalWrite(LEFT_LED, LOW);
  digitalWrite(RIGHT_LED, LOW);

  //MAGNET
  magnet = digitalRead(magnetSen);
  
  //get the values for how far the bot is off the line
  leftValue = analogRead(LEFT_IR) - leftS;
  rightValue = analogRead(RIGHT_IR) - rightS;
  leftValue = map(leftValue, 0, 1023, 1023, 0);
  rightValue = map(rightValue, 0, 1023, 1023, 0);

  //get the values for if the bot is on the line
  centerValue = analogRead(CENTER_IR);
  rightValue = map(centerValue, 0, 1023, 1023, 0);

  //print the error for testing purposes
  Serial.print("Left: ");
  Serial.println(leftValue);
  Serial.print("Right: ");
  Serial.println(rightValue);

  //MAGNET
  if (magnet == 1){
    Serial.print("Magnet Detected\n");
    digitalWrite(magnetLED, HIGH);
  }else{
    Serial.print("Magnet Waiting\n");
    digitalWrite(magnetLED, LOW);
  }
  
  //acquiring distances from the line looollll  -NEED TO COLLECT APPROPRIATE VALUES FOR leftS and rightS
  int lerr = -leftValue + leftS;
  int rerr = rightValue - rightS;
  int err = (lerr+rerr)/2;

  //getting and applying the correction - check signs of correction?
  int correction = (int) getCorrection(err);
  if (centerValue > 5) { 
    s1 = speed1+((correction*3)/2); // left motor
    s2 = speed2-((correction*1)/2); // right motor
  } else if (correction > 0) { // right turn
    s1 = speed1+((correction*4)/2); // left motor gets more power to turn right
    s2 = speed2-((correction*1)/2); // right motor
  } else if (correction < 0) { // left turn
    s1 = speed1+((correction*3)/2); // left motor
    s2 = speed2-(correction); // right motor gets more power to turn left
  }
  //make sure the speed change isn't too drastic
  if(s1<0)
    s1 = 98;
  else if(s1>250)
    s1 = 200;
  if(s2<0)
    s2 = 125;
  else if(s2>250)
    s2 = 180;

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
