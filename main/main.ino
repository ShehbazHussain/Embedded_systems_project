#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Keypad.h>
#include <NewPing.h>

// servo motor stuff
int close_pos=180;
int open_pos=90 ;

// IR pins

#define Analog_Pin_IR_Sensor A0 // IR Sensor
#define GPIOL 45
int IR_Sensor_Value;
int IR_Sensor_Distance;

// RGB_1 pins
#define red_pin_rgb_1 12
#define green_pin_rgb_1 11
#define blue_pin_rgb_1 10

// RGB_2 pins
#define red_pin_rgb_2 9
#define green_pin_rgb_2 8
#define blue_pin_rgb_2 7

// Ultrasonic pins 
#define TRIGGER_PIN 40
#define ECHO_PIN 41
#define MAX_DISTANCE 200
int ultrasonic_distance;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance


// Buzzer Pins
#define buzzer 2 

// Button Pin
#define buttonPin 3
boolean buttonState = false;         // variable for reading the pushbutton status
int button_counter = 0;

// RFID Pins
#define RST_PIN 48        // Configurable, see typical pin layout above
#define SS_PIN 53         // Configurable, see typical pin layout above
int pos = 0;    // variable to store the servo position
boolean garage = false;
int i=0;
Servo servo1;
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// Keybad Pins
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {22, 24, 26, 28}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {30, 32, 34, 36}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

const String password = "1234"; // change your password here
String input_password;

void setup()
{
  Serial.begin(9600);   // Initialize serial communications with the PC
  pinMode(red_pin_rgb_1, OUTPUT);
  pinMode(green_pin_rgb_1, OUTPUT);
  pinMode(blue_pin_rgb_1, OUTPUT);  
  pinMode(red_pin_rgb_2, OUTPUT);
  pinMode(green_pin_rgb_2, OUTPUT);
  pinMode(blue_pin_rgb_2, OUTPUT);  
  pinMode(buzzer,OUTPUT);
  pinMode(buttonPin, INPUT);

  // RFID Stuff
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  servo1.attach(4);
  servo1.write(0);
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}
 
void loop()
{
  ultrasonic_reading();
  IR_Sensor();
  RFID();
  keypad_dial();
  button();
}
void buzzer_activate(){
  digitalWrite(buzzer, HIGH); // produce sound
}

void buzzer_deactivate(){
  digitalWrite(buzzer, LOW); // produce sound
}

void IR_Sensor(){
  IR_Sensor_Value = analogRead(Analog_Pin_IR_Sensor);
  IR_Sensor_Distance = -0.1005*(IR_Sensor_Value) + 50;// read the input pin
  Serial.println(IR_Sensor_Distance);
  if (IR_Sensor_Distance < 7){
    servo1.write(close_pos);
    buzzer_activate();
    }
  else{
    buzzer_deactivate();    
    }  
}
 
void setColor_rgb1(int red, int green, int blue)
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

void setColor_rgb2(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(red_pin_rgb_2, red);
  analogWrite(green_pin_rgb_2, green);
  analogWrite(blue_pin_rgb_2, blue);  
}


void button(){
  buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);
  if (buttonState == true){
    button_counter++;
    if ( (button_counter % 2) != 0) {
    servo1.write(close_pos);  
    }
    if ( (button_counter % 2) == 0) {
    servo1.write(open_pos);
    }
    }
}

void ultrasonic_reading(){
  //delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings. (we proservo_pulse_milisecondsdon't need this anymore)
  ultrasonic_distance = sonar.ping_cm(); 
  Serial.println("Ping: " + String(ultrasonic_distance)); // Send ping, get distance in cm and print result (0 = outside set distance range)
  if (ultrasonic_distance<5){
    
    setColor_rgb1(255,0,0);
    setColor_rgb2(255,0,0);
    }
   else{
    setColor_rgb1(0,255,0);
    setColor_rgb2(0,255,0);
    }

}

void keypad_dial(){
  char key = customKeypad.getKey();
  
    if (key){
    Serial.println(key);

    if(key == '*') {
      input_password = ""; // clear input password
    } else if(key == '#') {
      if(password == input_password) {
        Serial.println("password is correct");
        servo1.write(close_pos);
        // DO YOUR WORK HERE
        
      } else {
        Serial.println("password is incorrect, try again");
      }

      input_password = ""; // clear input password
    } else {
      input_password += key; // append new character to input password string
    }
}
}

void RFID(){
  
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

//Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "C4 72 67 A3" || content.substring(1) == "E2 A5 F7 1B") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(1000);
    i++;
    if ( (i % 2) != 0) {
    servo1.write(close_pos);  
    }
    if ( (i % 2) == 0) {
    servo1.write(open_pos);
    }

  }
  else 
  {
    Serial.println(" Access denied");
    Serial.println();
  }
  }
