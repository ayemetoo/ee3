///Sensors
#define LEFT_IR A0
#define RIGHT_IR A1

///Turn Indicators
#define LEFT_LED 4
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
int leftS = 245, rightS = 45;
void loop() {
  digitalWrite(LEFT_LED, LOW);
  digitalWrite(RIGHT_LED, LOW);
  leftValue = analogRead(LEFT_IR) - 200;
  rightValue = analogRead(RIGHT_IR);
  leftValue = map(leftValue, 0, 1023, 1023, 0);
  rightValue = map(rightValue, 0, 1023, 1023, 0);
  
  Serial.print("Left: ");
  Serial.println(leftValue);
  Serial.print("Right: ");
  Serial.println(rightValue);

  int lerr = -leftValue + leftS;
  lerr /= 2;
  int rerr = rightValue - rightS;
  rerr /=2;
  int err = (lerr+rerr)/2;

  Serial.print("Left error: ");
  Serial.println(lerr);
  Serial.print("Right error: ");
  Serial.println(rerr);
  Serial.print("Err: ");
  Serial.println(err);


  //if leftValue is high, then turn on the left LED
  //if rightValue is high, then turn on the right LED
  //acquiring distances from the walls
  if (err < -1) { //dummy values
    digitalWrite(LEFT_LED, HIGH);
  }
  if (err > 1) { //dummy values
    digitalWrite(RIGHT_LED, HIGH);  
  }

  
  delay(500);
}
