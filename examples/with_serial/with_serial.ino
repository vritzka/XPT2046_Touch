#include "XPT2046_Particle.h"


//TOUCH
#define CS_PIN D5
// MOSI=A5, MISO=A4, SCK=A3
#define TIRQ_PIN D1

//XPT2046_Touchscreen ts(CS_PIN);  // Param 2 - NULL - No interrupts
//XPT2046_Touchscreen ts(CS_PIN, 255);  // Param 2 - 255 - No interrupts
XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN);  // Param 2 - Touch IRQ Pin - interrupt enabled polling



void setup() {
    
    //TOUCH
    ts.begin();
    ts.setRotation(3);

    Serial.begin();
 
}



void loop(void) {
    
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
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