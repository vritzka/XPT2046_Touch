# XPT2046 / ADS7843 Touch sensor chip

A Particle library for XPT2046.

Ported from Paul Stoffregen's Arduino version.


## Usage

Connect the chip like this

DO --- MISO A4
DI --- MOSI A5
CLK --- SCK A3
SS --  (you will chose CS this pin in the application)

You could also use SPI1, the other SPI bus.

```
#include "XPT2046_touch.h"
#define CS_PIN D5    //ChipSelect or SlaveSelect Pin
#define TIRQ_PIN D1 // interrupt pin

XPT2046_Touchscreen ts(SPI, CS_PIN, TIRQ_PIN);

void setup() {
  ts.begin();
  ts.setRotation(3); 
  
  Serial.begin();
}

void loop() {
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
```

See the [examples](examples) folder for more details.

Licensed under the MIT license
