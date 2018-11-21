#include "XPT2046_Touchscreen.h"
#include <Adafruit_mfGFX.h>
#include "DFRobot_SHT20.h"
DFRobot_SHT20 sht20;

/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "Adafruit_ILI9341.h"
#define TFT_DC A1
#define TFT_CS A2
#define TFT_BRIGHTNESS_PIN WKP
#define TFT_RESET_PIN D6

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RESET_PIN);


//TOUCH

#define CS_PIN D5
// MOSI=A5, MISO=A4, SCK=A3
#define TIRQ_PIN D1

//XPT2046_Touchscreen ts(CS_PIN);  // Param 2 - NULL - No interrupts
//XPT2046_Touchscreen ts(CS_PIN, 255);  // Param 2 - 255 - No interrupts
XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN);  // Param 2 - Touch IRQ Pin - interrupt enabled polling

void showTouchCoordinates() {
    
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
      
    TS_Point p = ts.getPoint();

    tft.fillRect(120,7,120,100,ILI9341_BLACK);
    
    tft.setCursor(120, 7);
    tft.print("z: ");
    tft.print(p.z);
    
    
    tft.setCursor(120, 30);
    tft.print("x: ");
    tft.print(p.x);
    
    tft.setCursor(120, 50);
    tft.print("y: ");
    tft.print(p.y); 

}

// END TOUCH


void setup() {
    
    //TFT Display
    tft.begin();
    tft.setRotation(3);
    //DISPLAY BRIGHNESS
    pinMode(TFT_BRIGHTNESS_PIN, OUTPUT);
    analogWrite(TFT_BRIGHTNESS_PIN, 255);  //0 (dark) to 255 (max brightness)
    //END TFT DISPLAY
    
    //TOUCH
    ts.begin();
    ts.setRotation(3);
    
    tft.fillScreen(ILI9341_BLACK);

    Serial.begin();
 
}



void loop(void) {
    
    if(ts.touched()) {

      tft.setTextColor(ILI9341_WHITE);
      tft.setTextSize(2);

      TS_Point p = ts.getPoint();

      tft.fillRect(120,7,120,100,ILI9341_BLACK);

      tft.setCursor(120, 7);
      tft.print("z: ");
      tft.print(p.z);


      tft.setCursor(120, 30);
      tft.print("x: ");
      tft.print(p.x);

      tft.setCursor(120, 50);
      tft.print("y: ");
      tft.print(p.y); 

      delay(30);

    }

}


