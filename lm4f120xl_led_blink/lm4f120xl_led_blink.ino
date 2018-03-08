/////////////////////////////////////////////////////////////////////////////////
// Date: 2018-03-08
// Target board: TI Stellaris LM4F120XL LaunchPad
// see:
//  Stellaris LM4F120 LaunchPad Evaluation Board User Manual (EK-LM4F120XL)
//  http://www.ti.com/lit/ug/spmu289c/spmu289c.pdf
/////////////////////////////////////////////////////////////////////////////////

#define SW1_PIN    PF_4
#define SW2_PIN    PF_0

#define RED_PIN    PF_1
#define GREEN_PIN  PF_2
#define BLUE_PIN   PF_3

#define NUM_LEDS       (3)
#define INTERVAL_MSEC  (500)

bool running = true;
bool forward = true;
uint32_t ts;

const int LED_PINS[ NUM_LEDS ] = { RED_PIN, GREEN_PIN, BLUE_PIN };

void setup() {
   pinMode( SW1_PIN, INPUT_PULLUP );
   pinMode( SW2_PIN, INPUT_PULLUP );
   for ( int i=0; i < NUM_LEDS; i++ ) {
      pinMode( LED_PINS[i], OUTPUT );
   }
   ts = millis();
}

void loop() {
   static int index=0;
   
   if ( digitalRead( SW1_PIN ) == LOW ) { // The push button SW1 is pressed
      while ( digitalRead( SW1_PIN ) == LOW ) { delay(10); }
      running = !running;
      ts = millis();
   }
   
   if ( digitalRead( SW2_PIN ) == LOW ) { // The push button SW2 is pressed
      while ( digitalRead( SW2_PIN ) == LOW ) { delay(10); }
      if ( running ) {
         forward = !forward; // reverse the direction
      }
      ts = millis();
   }
   
   if ( millis() - ts >= INTERVAL_MSEC ) {
      ts += INTERVAL_MSEC;
      if ( running ) {
         for ( int i=0; i < NUM_LEDS; i++ ) {
            digitalWrite( LED_PINS[i], (i==index) ? HIGH : LOW );
         } 
         // update the index of the LED in the array which will be turned on next.
         if ( forward ) { // forward direction
            index = (index+1) % NUM_LEDS; 
         } else { // reverse direction
            index = (NUM_LEDS+index-1) % NUM_LEDS; 
         }
      }
      else if ( index >= 0 ) {
         index = -1;
         for ( int i=0; i < NUM_LEDS; i++ ) {
            digitalWrite( LED_PINS[i], LOW ); // turn off all LEDs
         }     
      }
   }
}
/////////////////////////////////////////////////////////////////////////////////

