#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RST_PIN         48          // Configurable, see typical pin layout above
#define SS_PIN          53         // Configurable, see typical pin layout above
Servo myservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position
Servo servo1;

boolean garage = false;

int i=0;

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
	Serial.begin(9600);		// Initialize serial communications with the PC
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
  servo1.attach(4);
  servo1.write(0);
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
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
    servo1.write(0);  
    }
    if ( (i % 2) == 0) {
    servo1.write(180);
    }

  }
  else 
  {
    Serial.println(" Access denied");
    Serial.println();
  }
 
 
}
