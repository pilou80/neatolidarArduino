// the setup routine runs once when you press reset:
void setup()
{
  Serial.begin(500000);
  Serial1.begin(115200);
  pinMode(9, OUTPUT);
  analogWrite(9, 255);
}

uint8_t power = 255;

uint8_t incomingByte = 0;
int8_t currentByte = -1;
uint8_t currentIndex = -1;
float speedValue = 0;
uint8_t value[4];
uint16_t CRC = 0;
void loop()
{
  while(Serial1.available())
  {
    incomingByte = Serial1.read();    
    if((currentByte == -1) &&
    (incomingByte == 0xFA))
    {
      currentByte++;
      speedValue = 0;
      currentIndex = -1;
    }
    else if(currentByte == 0)
    {
      currentIndex = incomingByte;
      //Serial.println(currentIndex);
      //TODO check currentIndex from A0 to F9
      currentByte++;
    }
    else if(currentByte == 1)
    {
      speedValue = incomingByte;
      currentByte++;
    }
    else if(currentByte == 2)
    {
      speedValue += incomingByte<<8;
      speedValue = speedValue/64.0;
      currentByte++;
      if(speedValue > 200)
      {
        if(power!=0)
          power--;
      }
      else
      {
        if(power!=255)
          power++;
      }
      analogWrite(9, power);
        
      Serial.println(speedValue);
    }
    else if((currentByte > 2) &&
            (currentByte < 8))
    {
      value[currentByte-3] = incomingByte<<8;
      currentByte++;
    }
    else if(currentByte == 8)
    {
      CRC = incomingByte;
      currentByte++;
    }
    else if(currentByte == 9)
    {
      CRC += incomingByte<<8;
      currentByte=-1;
    }
    
    
    
  }
}
