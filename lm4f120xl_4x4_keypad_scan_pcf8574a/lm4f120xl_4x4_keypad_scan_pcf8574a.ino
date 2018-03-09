/////////////////////////////////////////////////////////////////////////////////
// Date: 2018-03-09
// Target board: TI Stellaris LM4F120XL LaunchPad
// see:
//  Stellaris LM4F120 LaunchPad Evaluation Board User Manual (EK-LM4F120XL)
//  http://www.ti.com/lit/ug/spmu289c/spmu289c.pdf
/////////////////////////////////////////////////////////////////////////////////

// 4x4 keypad pins from left to right connected to P0..P7 pins of PCF8574A
// P0-P3=Rows (output), P4-P7=Columns (input)

#include <Wire.h> // see: http://energia.nu/reference/wire/
// I2C1 : SDA = PA7, SCL = PA6

#define I2C_ADDR       (0x38)  // PCF8574A address pins: A0=A1=A2=0

const uint8_t ROWS = 4; 
const uint8_t COLS = 4; 

const char keys[ ROWS ][ COLS ] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

void setup(){
  Wire.begin( );
  Serial.begin( 115200 );
  delay(1000);
  Serial.println( "TI Stellaris LM4F120XL LaunchPad: 4x4 Keypad Scan with PCF8574A Demo\n\n" );
}

void writeOutput( uint8_t value ) {
  Wire.beginTransmission( I2C_ADDR );
  Wire.write( value );
  Wire.endTransmission();
}

// P0..P3 output, P7..P4 input
uint8_t readInput() {
  uint8_t data = 0xFF;
  // read only one byte from the I2C slave device
  Wire.requestFrom( I2C_ADDR, 1 ); 
  delayMicroseconds(4);
  if ( Wire.available() > 0 ) {
     data = Wire.read();
  }
  return data;
}

char getKey() {
  uint8_t value;
  char key_pressed = '\0';
  for ( int row=0; row < 4; row++ ) { 
     // Send 0xFE for Row 0, 0xFD for Row 1, 0xFB for Row 2, and 0xF7 for Row 3
     value = 0xF0 | ~(1 << row); 
     writeOutput( value ); // send a data byte to PCF8754A
     value = readInput();  // read a data byte from PCF8574A
     for ( int col=0; col < 4; col++ ) {
        if ( (~value >> (col+4)) & 1 ) {
          key_pressed = keys[row][col];
          break;
        }
     }
     if ( key_pressed != '\0' ) {
        while ( value == readInput() ) ; // blocking
        value = 0xFF;
        writeOutput( value ); // send a data byte to PCF8754A
        return key_pressed; 
     } 
  }
  return key_pressed; // no key press
}

void loop(){
  char key = getKey();
  if ( key != 0 ) {
     Serial.println( key );
     delay(50);
  }
  delay(10);
}
/////////////////////////////////////////////////////////////////////////////////

