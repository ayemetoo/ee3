const int chipSelect = 7;

void setup() 
{
  Serial.begin(9600);

  // This constantly waits for serial port to connect
  while(!Serial)
  {
  }
  Serial.print("Initialize Card");

  if(!SD.begin(chipSelect))
  {
    Serial.printla("Card Failed:);
    return;
  }
  Serial.println("Card Works");
}

void loop() 
{
  // makes a string to put data together
  String dataString = "";
  String dataString1 = "";
  int analogPin = 23; // left black tape sensor
  int analogPin1 = 24; // right black tape sensor
  
  int sensor = analogRead(analogPin);
  dataString += String(sensor);
  int sensor = analogRead(analogPin1);
  dataString1 += String(sensor);
}

File dataFile = SD.open("datalog.txt", FILE_WIRTE);

  if (dataFile)
  {
    dataFile.println(dataString);
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
    Serial.println(dataString1);
  }
  else
  {
    Serial.println("error Opening datalog.txt");
  }
}

