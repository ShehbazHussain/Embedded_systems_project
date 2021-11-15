#include <SPI.h>
#include <RFID.h>
#define SDA_DIO 53
#define RESET_DIO 48
RFID RC522(SDA_DIO, RESET_DIO);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  /* Enable the SPI interface */
  SPI.begin(); 
  /* Initialise the RFID reader */
  RC522.init();

}

void loop() {
  // put your main code here, to run repeatedly:
  /* Has a card been detected? */
  if (RC522.isCard())
  {
    /* If so then get its serial number */
    RC522.readCardSerial();
    Serial.println("Card detected:");
    for(int i=0;i<5;i++)
    {
    Serial.print(RC522.serNum[i],DEC);
    //Serial.print(RC522.serNum[i],HEX); //to print card detail in Hexa Decimal format
    }
    Serial.println();
    Serial.println();
  }
  delay(1000);

}
