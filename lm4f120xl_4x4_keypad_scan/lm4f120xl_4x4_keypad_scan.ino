/////////////////////////////////////////////////////////////////////////////////
// Date: 2018-03-08
// Target board: TI Stellaris LM4F120XL LaunchPad
// see:
//  Stellaris LM4F120 LaunchPad Evaluation Board User Manual (EK-LM4F120XL)
//  http://www.ti.com/lit/ug/spmu289c/spmu289c.pdf
/////////////////////////////////////////////////////////////////////////////////

const uint8_t ROWS = 4; 
const uint8_t COLS = 4; 

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

uint8_t rowPins[ ROWS ] = {PD_0, PD_1, PD_2, PD_3};  // R3, R2, R1, R0
uint8_t colPins[ COLS ] = {PE_1, PE_2, PE_3, PF_1};  // C3, C2, C1, C0

void setup(){
  Serial.begin( 115200 );
  delay(1000);
  Serial.println( "TI Stellaris LM4F120XL LaunchPad: 4x4 Keypad Scan Demo\n\n" );

  for ( int i=0; i < ROWS; i++ ) {
     pinMode( rowPins[i], OUTPUT );
  }
  for ( int i=0; i < COLS; i++ ) {
     pinMode( colPins[i], INPUT_PULLUP );
  }
}

char getKey( ) {
  char key_pressed = 0;
  for ( int j=0; j < ROWS; j++ ) { // scan the j-th row (j=0,1,2,3)
    for ( int i=0; i < ROWS; i++ ) { 
        // output HIGH to all rows, except the j-th row
        digitalWrite( rowPins[i], (i==j) ? LOW : HIGH );
    }
    for (int i=0; i < COLS; i++) {
       if ( digitalRead( colPins[i] ) == LOW ) { // Button at (R,C)=(j,i) is pressed
           // wait until the button is released.
           while ( digitalRead( colPins[i] ) != HIGH ) ; // blocking
           key_pressed = keys[j][i]; // get the associated key for that button
           break;
       }
    }  
    digitalWrite( rowPins[j], HIGH );
    if ( key_pressed != 0 ) { 
        return key_pressed;
    }
  }
  return 0; // no key pressed
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

