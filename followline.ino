
#include <Wire.h>
#include <ZumoShield.h>

ZumoBuzzer buzzer;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
ZumoReflectanceSensorArray reflectanceSensors;

enum State { left, right, straight, lost_left, lost_right};

void setup()
{
  // Initialize the reflectance sensors module
  reflectanceSensors.init();

  // Wait for the user button to be pressed and released
  button.waitForButton();

  // Turn on LED to indicate we are in calibration mode
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);
  int i;
  for (i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-200, 200);
    else
      motors.setSpeeds(200, -200);
    reflectanceSensors.calibrate();

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  motors.setSpeeds(0, 0);

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);


  // Wait for the user button to be pressed and released
  button.waitForButton();

}

void motor_left(){
   motors.setSpeeds(0, 200);
}

void motor_right(){
   motors.setSpeeds(200, 100);
}

void motor_straight(){
  motors.setSpeeds(200, 200);
}

void motor_lost_left(){
  motors.setSpeeds(-200, 200);
}

void motor_lost_right(){
  motors.setSpeeds(200, -200);
}

void loop() {
  unsigned int sensors[6];

  // Get the position of the line.  Note that we *must* provide the "sensors"
  // argument to readLine() here, even though we are not interested in the
  // individual sensor readings
  int position = reflectanceSensors.readLine(sensors);

  State state;
  if (position > 2600 && position < 5000) {
    state = right;
  } else if (position < 2400 && position > 0) {
    state = left;
  } else if (position > 2350 && position < 2650) {
    state = straight;
  } else if (position == 0){
    state = lost_left;
  } else if (position == 5000){
    state = lost_right;
  }
  
  
  switch (state) {
    case left:
      motor_left();
      break;

    case right:
      motor_right();
      break;

    case straight:
      motor_straight();
      break;
      
    case lost_left:
      motor_lost_left();
      break;

     case lost_right:
       motor_lost_right();
       break;
   
  }


}
