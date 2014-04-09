/*
*  demo sketch to read pixel from a LF1401 line sensor
*
*  Thanks to Guy van den Berg for a great article about setting up ADC
*  http://www.microsmart.co.za/technical/2014/03/01/advanced-arduino-adc/
*
*
*  written by Ingo Randolf - 2013
*/

// Define various ADC prescaler
const unsigned char PS_2 = (1 << ADPS0);
const unsigned char PS_4 = (1 << ADPS1);
const unsigned char PS_8 = (1 << ADPS1) | (1 << ADPS0);
const unsigned char PS_16 = (1 << ADPS2);
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);



//--------------------------------
// variables
//--------------------------------

// pin definition
// take care to change these values
// in order to be fast pins are set directly
byte pixelValue_pin = A0; // analog output of LF1401
byte si_pin = 7;          // SI pin of LF1401
byte clk_pin = 8;         // CLK Pin of LF1401

// out pixel array
uint8_t pixels[128];

int value;

//--------------------------------
// setup
//--------------------------------
void setup()
{
  //set pins to output
  pinMode(si_pin, OUTPUT);
  pinMode(clk_pin, OUTPUT);
  
  //set analog pins as input
//  DDRC = B00000000;
  
  // init pins
  digitalWrite(clk_pin, LOW);
  digitalWrite(si_pin, HIGH);


  // set up ADC
  // choose the right prescaler for analog read.  
  ADCSRA &= ~PS_128;  // remove bits set by Arduino library

  // we want to be fast, choose low sampling resolution
  ADCSRA |= PS_4; // 4 prescaler
  
  // start Serial
  Serial.begin(115200);  
}



//--------------------------------
// loop
//--------------------------------
void loop()
{  
  //--------------------------------
  //read pixels
  
  // we want to be fast, use PORTS directly instead of digitalWrite
  
  byte i;
  for (i = 0; i < 128; i++) {
    
    // set clk to high
    PORTB |= B00000001; // digitalWrite(clk_pin, HIGH);

    if (i == 0) {
      //set si to low
      PORTD ^= B10000000; // digitalWrite(si_pin, LOW);
    } else if (i == 127) {
      //set si to high (see datasheet)
      PORTD |= B10000000; // digitalWrite(si_pin, HIGH);
    }


    // tick clock low
    PORTB ^= B00000001; // digitalWrite(clk_pin, LOW);
    
    // get pixel value
    value = analogRead(pixelValue_pin);
    // scale value (0 - 1024) and constrain
    pixels[i] = constrain((uint8_t)(value / 4), 0, 255);
  }
  

  // send pixelarray over serial 
  Serial.write((uint8_t*)pixels, (size_t)128);
  Serial.println();
}



