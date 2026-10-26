 /* 130 DC Motor
     by DFRobot <https:www.dfrobot.com>

    */

    int motorPin = 3;   //Motor drive pin D3
    int motorSpeed;     //Define motor speed

    void setup()
    {
      Serial.begin(9600);
    }

    void loop()
    {

      for(motorSpeed = 0 ; motorSpeed <= 255; motorSpeed+=5)
      {
        analogWrite(motorPin, motorSpeed);   //PWM speed control
        delay(30);
      }
      for(motorSpeed = 255 ; motorSpeed >= 0; motorSpeed-=5)
      {
        analogWrite(motorPin, motorSpeed);   //PWM speed control
        delay(30);
      }
    }