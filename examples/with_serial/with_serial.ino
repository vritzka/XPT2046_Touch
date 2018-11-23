//this example will be helpful only if you connect the device to USB. 
//That means you need to install the CLI first. The CLI will also install the USB driver: https://docs.particle.io/tutorials/developer-tools/cli/

// define your screen dimensions
#include "XPT2046_Touch.h"

//TOUCH


#define CS_PIN D5    //ChipSelect or SlaveSelect Pin
#define TIRQ_PIN D1 // interrupt pin

//The Photon has 2 SPI busses; SPI and SPI1. You can chose which one to use in the initialization underneth
//Pins for SPI: MOSI=A5, MISO=A4, SCK=A3
//Pins for SPI1: MOSI=D2, MISO=D3, SCK=D4


XPT2046_Touchscreen ts(SPI, 320,240,CS_PIN);  // No interrupts
//XPT2046_Touchscreen ts(SPI, 320, 240, CS_PIN, TIRQ_PIN);  // Using SPI, interrupt enabled, polls only when disply is touched



void setup() {
    
    //TOUCH
    ts.begin();
    ts.setRotation(3);

    Serial.begin();
 
}



void loop(void) {
    
  if (ts.touched()) {
    TS_Point p = ts.getPosition();
    Serial.print("Pressure = ");
    Serial.print(p.z);
    Serial.print(", x = ");
    Serial.print(p.x);
    Serial.print(", y = ");
    Serial.print(p.y);
    delay(30);
    Serial.println();
 }

}