// #include <Servo.h>

// RGB_1 pins
#define red_pin_rgb_1 30
#define green_pin_rgb_1 32
#define blue_pin_rgb_1 34

// RGB_2 pins
#define red_pin_rgb_1 30
#define green_pin_rgb_1 32
#define blue_pin_rgb_1 34

// Buzzer Pins
#define buzzer 2 

// Button Pin
#define buttonPin = 3

// RFID Pins


// RFID Pins


void setup()
{
  pinMode(red_pin_rgb_1, OUTPUT);
  pinMode(green_pin_rgb_1, OUTPUT);
  pinMode(blue_pin_rgb_1, OUTPUT);  
}
 
void loop()
{
  setColor(255, 0, 0);  // red
  delay(1000);
  setColor(0, 255, 0);  // green
  delay(1000);
  setColor(0, 0, 255);  // blue
  delay(1000);
  setColor(255, 255, 0);  // yellow
  delay(1000);  
  setColor(80, 0, 80);  // purple
  delay(1000);
  setColor(0, 255, 255);  // aqua
  delay(1000);
}
 
void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(red_pin_rgb_1, red);
  analogWrite(green_pin_rgb_1, green);
  analogWrite(blue_pin_rgb_1, blue);  
}
