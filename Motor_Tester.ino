#define leftMotor 5
#define rightMotor 3

void setup() {
  // put your setup code here, to run once:
  pinMode(leftMotor, OUTPUT);
  pinMode(rightMotor, OUTPUT);
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  analogWrite(rightMotor, 110);
  analogWrite(leftMotor, 130);

}
