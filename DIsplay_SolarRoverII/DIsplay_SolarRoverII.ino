// This is the code for the display to read
// the energy sensors for thet supercapacity
// for project Solar Rover I +II

// Made by Jimmy Aben 
// Student Electrictal engineering Zuyd Hogeschool


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <INA228.h>

const char* BatteryValue = "";


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
  Serial.println(F("SSD1306 allocation failed"));
  for(;;); // Don't proceed, loop forever
  }


  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.display();
  delay(2000); 
  display.clearDisplay();
  display.display();
  delay(2000);


}

void loop() {
  // put your main code here, to run repeatedly:
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("VOLTAGE IN: ");  
  //display.print(voltage_in);
  display.println(" V");

  display.print("POWER IN: ");  
  //display.print(power_in);
  display.println(" mW");

  display.print("VOLTAGE OUT: ");
  //display.print(voltage_out);
  display.println(" V");

  display.print("POWER OUT: ");  
  //display.print(power_out);
  display.println(" mW");

  display.setCursor(110, 0);
  display.println("75%");

  display.setCursor(110, 8);
  //display.println(BatteryValue);

  display.display();  
  delay(1000);

  
  
  
  }
