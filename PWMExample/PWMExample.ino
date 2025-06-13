#include <ESP32Servo.h>
#include <Adafruit_PWMServoDriver.h>

#include "controller.h"


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);      //  16 I/O channel PWM board


float L1 = 27.5;            // Length of the first part of the arm
float L2 = 28.0;            // Length of the second part of the arm

int timer;

int SERVO_SHOULDER = 250;  
int SERVO_ELBOW = 350;



void moveXY(float x, float y)
{
  float distance = sqrt(x*x + y*y);

  if(distance >= (L1 + L2))
  {
    Serial.println("Out of Range!");
    return;
  }


  float phie = acos((-L1*L1 + distance*distance - L2*L2) / (-2 * L1 * L2)) * 180.0 / PI;                        // Angle for the shoulder servo (angle for the first part of the arm)
  float theta = (atan2(y , x) + acos((L2*L2 - L1*L1 - distance*distance)/(-2*distance*L1))) * 180.0 / PI;       // Angle for the elbow servo (angle for the second part of the arm)


  int Servo_Shoulder = map(theta, 0, 180, 132, 392);      // mapping angle for SHOULDER SERVO to pulses for 16 I/O channel
  int Servo_Elbow    = map(phie, 0, 180, 132, 392);       // mapping angle for ELBOW SERVO to pulses for 16 I/O channel

  
  pwm.setPWM(0, 0, Servo_Shoulder);
  pwm.setPWM(1, 0, Servo_Elbow);

  Serial.print("Shoulder Servo = ");
  Serial.print(Servo_Shoulder);
  Serial.print(" | ");
  Serial.print("Elbow Servo = ");
  Serial.print(Servo_Elbow);
  Serial.print(" |  ");
  Serial.print("Shoulder Angle = ");
  Serial.print(theta);  
  Serial.print(" |  ");
  Serial.print("Elbow Angle = ");
  Serial.print(phie);

}



void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);                         // SDA, SCL
  pwm.begin();                                // for pwm 16 io board
  pwm.setOscillatorFrequency(27000000);       // Frequency of pwm 16 I/O board
  pwm.setPWMFreq(50);                         // Analog servos run at ~50 Hz updates
  Controllersetup();                          // Setup from the controller page

  delay(10);

	// this is not needed due to the use of the alternative constructor above
  Serial.println("Setup is completed!");

  timer = millis();
}

/*
notifications for the position of the servo's

Servo_Shoulder:
  - top = 
  - bottom = 
  - max = 410
  - min = 135
Servo_elbow: 
  - top = 525
  - bottom = 


*/

void loop() {

  if (timer + 30 > millis()){
  Controllerloop();

  if(ARM_UP > 0){
    Serial.println("omhoog");
    SERVO_SHOULDER += 5;   
    pwm.setPWM(0, 0, SERVO_SHOULDER);
    }
    else if(ARM_DOWN > 0){
      Serial.println("omlaag");
      SERVO_SHOULDER -= 5; 
      pwm.setPWM(0, 0, SERVO_SHOULDER);       
    }
    else if(ARM_RIGHT > 0){
      Serial.println("RECHTS");
      SERVO_ELBOW  += 5;
      pwm.setPWM(1, 0, SERVO_ELBOW);
    }
    else if(ARM_LEFT > 0){
        
      Serial.println("LINKS");
       SERVO_ELBOW -= 5;
       pwm.setPWM(1, 0, SERVO_ELBOW);
    }

    moveXY(55,1);
    delay(3000);
    moveXY(30, 30);
    delay(3000);
    moveXY(1, 49);
    delay(3000);

    Serial.println("Hoii");
    
    timer = millis();

    }
}
  
  

  
  /*for (uint16_t pulselen = 135; pulselen < 410; pulselen++) {           
    //pwm.setPWM(1, 0, pulselen);
    pwm.setPWM(0, 0, pulselen);    
    Serial.println("puls plus = ");
    Serial.println(pulselen);
    delay(50);
  }
  
  for (uint16_t pulselen = 410; pulselen > 135; pulselen--) {             
    //pwm.setPWM(1, 0, pulselen);
    pwm.setPWM(0, 0, pulselen);
    Serial.print("puls min = ");
    Serial.println(pulselen);
    delay(50);
  }*/












