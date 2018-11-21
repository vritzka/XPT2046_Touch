#include "XPT2046_Touchscreen.h"

#define Z_THRESHOLD     400
#define Z_THRESHOLD_INT	75
#define MSEC_THRESHOLD  3
#define SPI_SETTING     SPISettings(2000000, MSBFIRST, SPI_MODE0)

volatile static XPT2046_Touchscreen *isrPinptr;
void isrPin(void);

static int16_t rx_buffer[1];
static int8_t tx_buffer[2];

bool XPT2046_Touchscreen::begin()
{
    //starting the SPI interface with the Photon as master, display as slaved. Slave select pin is A2 (default anyway)
	SPI1.begin(SPI_MODE_MASTER);
	pinMode(csPin, OUTPUT);
	digitalWrite(csPin, HIGH);
	if (255 != tirqPin) {
		pinMode( tirqPin, INPUT );
		attachInterrupt(tirqPin, isrPin, FALLING);
		isrPinptr = this;
	}
	return true;
}


void isrPin( void )
{
	volatile XPT2046_Touchscreen *o = isrPinptr;
	o->isrWake = true;
}

TS_Point XPT2046_Touchscreen::getPoint()
{
	update();
	return TS_Point(xraw, yraw, zraw);
}

bool XPT2046_Touchscreen::tirqTouched()
{
	return (isrWake);
}

bool XPT2046_Touchscreen::touched()
{
	update();
	return (zraw >= Z_THRESHOLD);
}

void XPT2046_Touchscreen::readData(uint16_t *x, uint16_t *y, uint8_t *z)
{
	update();
	*x = xraw;
	*y = yraw;
	*z = zraw;
}

bool XPT2046_Touchscreen::bufferEmpty()
{
	return ((millis() - msraw) < MSEC_THRESHOLD);
}

static int16_t besttwoavg( int16_t x , int16_t y , int16_t z ) {
  int16_t da, db, dc;
  int16_t reta = 0;
  if ( x > y ) da = x - y; else da = y - x;
  if ( x > z ) db = x - z; else db = z - x;
  if ( z > y ) dc = z - y; else dc = y - z;

  if ( da <= db && da <= dc ) reta = (x + y) >> 1;
  else if ( db <= da && db <= dc ) reta = (x + z) >> 1;
  else reta = (y + z) >> 1;   //    else if ( dc <= da && dc <= db ) reta = (x + y) >> 1;

  return (reta);
}

int16_t XPT2046_Touchscreen::SPItransfer16(int8_t a,int8_t b)
//this function added to library because photon SPI library does not have a transfer16 function as used in the Arduino lib
//two args used to make operation explicit but could be reduced to 1 arg once operation proven
{
    int16_t c;
    
    c = SPI1.transfer(b) << 8;
    c |= SPI1.transfer(a);
    
    return (c);
}

int16_t XPT2046_Touchscreen::Flip(int16_t c)
{
    
  int16_t r=0;
  for(byte i = 0; i < 16; i++){
    r <<= 1;
    r |= c & 1;
    c >>= 1;
  }
  return r;
}

void XPT2046_Touchscreen::update()
{
    
	int16_t data[6];

    //just guards against reading the touch screen too often.
	if (!isrWake) return;
	uint32_t now = millis();
	if (now - msraw < MSEC_THRESHOLD) return;
	
	SPI1.beginTransaction(SPI_SETTING);
	digitalWrite(csPin, LOW);
	
	SPI1.transfer(0xB1 /* Z1 */);
	int16_t z1 = XPT2046_Touchscreen::SPItransfer16(0xC1,0 /* Z2 */) >> 3;
	int16_t z = z1 + 4095;
	int16_t z2 = XPT2046_Touchscreen::SPItransfer16(0x91,0 /* X */) >> 3;
	z -= z2;

	//if pressure is greater than a certain amount, then get xy measurements
	if (z >= Z_THRESHOLD) {
		XPT2046_Touchscreen::SPItransfer16(0x91,0 /* X */);  // dummy X measure, 1st is always noisy
		data[0] = XPT2046_Touchscreen::SPItransfer16(0xD1,0 /* Y */) >> 3;
		data[1] = XPT2046_Touchscreen::SPItransfer16(0x91,0 /* X */) >> 3; // make 3 x-y measurements
		data[2] = XPT2046_Touchscreen::SPItransfer16(0xD1,0 /* Y */) >> 3;
		data[3] = XPT2046_Touchscreen::SPItransfer16(0x91,0 /* X */) >> 3;
	}
	else data[0] = data[1] = data[2] = data[3] = 0;	// Compiler warns these values may be used unset on early exit.
	//Serial.println("Y pow down");
	data[4] = XPT2046_Touchscreen::SPItransfer16(0xD0,0 /* Y */) >> 3;	// Last Y touch power down
	//Serial.println("nul");
	data[5] = XPT2046_Touchscreen::SPItransfer16(0,0) >> 3;
	digitalWrite(csPin, HIGH);
	SPI1.endTransaction();
	
	//Serial.printf("z=%d  ::  z1=%d,  z2=%d  ", z, z1, z2);
	if (z < 0) z = 0;
	if (z < Z_THRESHOLD) { //	if ( !touched ) {
		// Serial.println();
		zraw = 0;
		if (z < Z_THRESHOLD_INT) { //	if ( !touched ) {
			if (255 != tirqPin) isrWake = false;
		}
		return;
	}
	zraw = z;
	
	// Average pair with least distance between each measured x then y
	//Serial.printf("    z1=%d,z2=%d  ", z1, z2);
	//Serial.printf("p=%d,  %d,%d  %d,%d  %d,%d", zraw,
		//data[0], data[1], data[2], data[3], data[4], data[5]);
	int16_t x = besttwoavg( data[0], data[2], data[4] );
	int16_t y = besttwoavg( data[1], data[3], data[5] );
	
	//Serial.printf("    %d,%d", x, y);
	//Serial.println();
	if (z >= Z_THRESHOLD) {
		msraw = now;	// good read completed, set wait
		switch (rotation) {
		  case 0:
			xraw = 4095 - y;
			yraw = x;
			break;
		  case 1:
			xraw = x;
			yraw = y;
			break;
		  case 2:
			xraw = y;
			yraw = 4095 - x;
			break;
		  default: // 3
			xraw = 4095 - x;
			yraw = 4095 - y;
		}
	}
}