/////////////////////////////////////////////////////////////////////////////////
// Date: 2018-03-08
// Target board: TI Stellaris LM4F120XL LaunchPad
// see:
//  Stellaris LM4F120 LaunchPad Evaluation Board User Manual (EK-LM4F120XL)
//  http://www.ti.com/lit/ug/spmu289c/spmu289c.pdf
/////////////////////////////////////////////////////////////////////////////////
#include <Wire.h> // see: http://energia.nu/reference/wire/
// I2C1 : SDA = PA7, SCL = PA6

#define INTERVAL_MSEC  (200)
#define I2C_ADDR       (0x38)

uint32_t ts;
char sbuf[32];

void setup() {
   Serial.begin( 115200 );
   Wire.begin( );
   Serial.println( "TI Stellaris LM4F120XL LaunchPad: PCF8574A I2C Demo..." );
   ts = millis();
}

void loop() {
   static uint8_t value = 0xFE;
 
   if ( millis() - ts >=  INTERVAL_MSEC ) {
       bool timeout = false;
       uint8_t cnt=0;  
       ts += INTERVAL_MSEC;

       // send one byte to the PCF8574A
       Wire.beginTransmission( I2C_ADDR );
       Wire.write( value ); 
       value ^= 0x03;
       Wire.endTransmission();

       // read one byte to the PCF8574A
       Wire.requestFrom( I2C_ADDR, 1 );
       while( Wire.available() < 1 ) { 
          if ( cnt++ > 10 ) {
             timeout = true;
             break; 
          }
          delay(1); 
       }
       if ( !timeout ) {
          uint8_t data = Wire.read();
          sprintf( sbuf, "Read: 0x%02X", data );
       } else {
          sprintf( sbuf, "Read: timeout!!!" );
       }
       Serial.println( sbuf );
   }
}
/////////////////////////////////////////////////////////////////////////////////

