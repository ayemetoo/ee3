///Sensors
#define LEFT_IR A0
#define RIGHT_IR A1

///Turn Indicators
#define LEFT_LED 11
#define RIGHT_LED 2

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LEFT_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);
  pinMode(LEFT_LED, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);
}

int leftValue, rightValue;

void loop() {
  digitalWrite(LEFT_LED, LOW);
  digitalWrite(RIGHT_LED, LOW);
  leftValue = analogRead(LEFT_IR) - 200;
  rightValue = analogRead(RIGHT_IR);
  
  Serial.print("Left: ");
  Serial.println(leftValue);
  Serial.print("Right: ");
  Serial.println(rightValue);


  //if leftValue is high, then turn on the left LED
  //if rightValue is high, then turn on the right LED
  //acquiring distances from the walls
  if (leftValue > 500) { //dummy values
    digitalWrite(LEFT_LED, HIGH);
  }
  if (rightValue > 500) { //dummy values
    digitalWrite(RIGHT_LED, HIGH);  
  }

  
  delay(500);
}
