//made by Jimmy Aben 
//16-6-2024
//v1


#include <ESP32Servo.h>                               //   be sure to have these libraries
#include <Adafruit_PWMServoDriver.h>

#include "controller.h"
//#include "joystick.h"



Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);      //  16 I/O channel PWM board


float L1 = 27.5;            // Length of the first part of the arm
float L2 = 28.0;            // Length of the second part of the arm

int timer;
short n = 0;

int SERVO_SHOULDER = 250;  
int SERVO_ELBOW = 350;

uint16_t shoulderAngle;
uint16_t elbowAngle;

int x = 30;
int y = 30;



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

  /*Serial.print("Shoulder Servo = ");
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
  */
}

void joystick()
{
  
  shoulderAngle = map(Shoulder, -508, 512, 8, -8);
  elbowAngle    = map(Elbow, -508, 512, 8, -8);


  if(shoulderAngle > 1)
  {
    SERVO_SHOULDER += shoulderAngle;
    pwm.setPWM(0, 0, SERVO_SHOULDER);
    Serial.println(SERVO_SHOULDER);
    delay(10);
  }
  else if(shoulderAngle < -1)
  {
    SERVO_SHOULDER += shoulderAngle;
    pwm.setPWM(0, 0, SERVO_SHOULDER);
    delay(10);
  }
  
  if(elbowAngle > 1)
  {
    SERVO_ELBOW += elbowAngle;
    pwm.setPWM(1, 0, SERVO_ELBOW);
    Serial.println(SERVO_ELBOW);
    delay(10);
  }
  else if(elbowAngle < -1 )
  {
    SERVO_ELBOW += elbowAngle;
    pwm.setPWM(1, 0, SERVO_ELBOW);
    Serial.println(SERVO_ELBOW);
    delay(10);
  }

  /*for (uint16_t pulselen = 135; pulselen < 410; pulselen += (shoulderAngle*2)) {           
    //pwm.setPWM(1, 0, pulselen);
    pwm.setPWM(0, 0, pulselen);    
    Serial.println("puls plus = ");
    Serial.println(pulselen);
    delay(50);
  }*/
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

  pwm.setPWM(0, 0, 300);
  pwm.setPWM(1, 0, 300);

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

    if(NextState && n < 2)                            //   Press CIRCLE (ctl->b();)  on PS4 controller to go up one case
    {
      n++;
    }

    else if(NextState && n >= 2)
    {
      n = 0;
    }

    
    switch(n)
      { 
        case 0:                                     // control robot arm manually with DPAD
          Serial.println("Case 0");

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
        break;


        case 1:                                     // control robot arm with funtion moxeXY
          Serial.println("Case 1");
         
            if(ARM_UP)
            {
              x++;
            }
            else if(ARM_DOWN)
            {
              x--;
            }

            if(ARM_RIGHT)
            {
              y++;
            }
            else if(ARM_LEFT)
            {
              y--;
            }

          moveXY(x, y);
          
          
          /*moveXY(55,1);
          delay(3000);
          moveXY(30, 30);
          delay(3000);
          moveXY(1, 49);
          delay(3000);*/
        break;
        

        case 2:
        Serial.println("Case 2");
        joystick();

        break;
      }
  
    
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












