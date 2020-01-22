#ifndef _XPT2046_Touch_h_
#define _XPT2046_Touch_h_

#include "Particle.h"
#include "SPI.h"

class TS_Point {
public:
	TS_Point(void) : x(0), y(0), z(0) {}
	TS_Point(int16_t x, int16_t y, int16_t z) : x(x), y(y), z(z) {}
	bool operator==(TS_Point p) { return ((p.x == x) && (p.y == y) && (p.z == z)); }
	bool operator!=(TS_Point p) { return ((p.x != x) || (p.y != y) || (p.z != z)); }
	int16_t x, y, z;
};

class XPT2046_Touchscreen {
  
public:
 
  	constexpr XPT2046_Touchscreen(SPIClass &spi, int displaywidth, int displayheight, uint8_t cspin) 
	: xptSPI(spi), displayWidth(displaywidth), displayHeight(displayheight), csPin(cspin), tirqPin(255) { };
  
	constexpr XPT2046_Touchscreen(SPIClass &spi, int displaywidth, int displayheight, uint8_t cspin, uint8_t tirq) 
	: xptSPI(spi), displayWidth(displaywidth), displayHeight(displayheight), csPin(cspin), tirqPin(tirq) { };
  
	bool begin();
	TS_Point getPoint();
  TS_Point getPosition();
	bool tirqTouched();
	bool touched();
	void readData(uint16_t *x, uint16_t *y, uint8_t *z);
	bool bufferEmpty();
	uint8_t bufferSize() { return 1; }
	void setRotation(uint8_t n) { rotation = n % 4; }
	int16_t SPItransfer16(int8_t a,int8_t b);
	int16_t Flip(int16_t c);
// protected:
	volatile bool isrWake=true;

private:
	void update();
  SPIClass &xptSPI;
  int displayWidth = 0;
int displayHeight = 0;
	uint8_t csPin, tirqPin, rotation=1;
	int16_t xraw=0, yraw=0, zraw=0;
	uint32_t msraw=0x80000000;
};

#endif
