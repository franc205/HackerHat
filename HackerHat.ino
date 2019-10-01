#include <RCSwitch.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN    6 // Which pin on the Arduino is connected to the NeoPixels? 
#define LED_COUNT 16 // How many NeoPixels are attached to the Arduino?

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); // Declare our NeoPixel strip object

//RF Stuff
RCSwitch mySwitch = RCSwitch();
unsigned long time,sec;
static const char* bin2tristate(const char* bin);
static char * dec2binWzerofill(unsigned long Dec, unsigned int bitLength);

//Timer Stuff
unsigned long timeOne;
unsigned long timeTwo;
unsigned long timer;

void setup() {
  Serial.begin(9600);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  mySwitch.enableReceive(0); 
}

void loop() {
  theaterChase(strip.Color(242, 0, 0), 75); // Red, half brightness
  theaterChase(strip.Color(29, 6, 201), 75); // Blue, half brightness
  theaterChase(strip.Color(29,161,11), 75); // Green, half brightness
  colorWipeHalf(strip.Color(0,217,255), 50); // Light blue
  listenCode();
  theaterChase(strip.Color(0,217,255), 75); // Light blue, half brightness
  theaterChase(strip.Color(170, 0, 255), 75); // Purple, half brightness
  theaterChase(strip.Color(29,161,11), 75); // Green, half brightness
  colorWipeHalf(strip.Color(255,140,0), 50); // Yellow
  listenCode();
  theaterChase(strip.Color(255,140,0), 75); // Yellow, half brightness
  theaterChase(strip.Color(242,73,200), 75); // Pink, half brightness
  theaterChase(strip.Color(29,161,11), 75); // Green, half brightness
  colorWipeHalf(strip.Color(242, 0, 0), 50); // Red
  listenCode();
}

/*NEOPIXEL FUNCTIONS*/
/*Used in normal behaviour*/
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  
    for(int b=0; b<3; b++) {
      strip.clear(); 
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color);
      }
      strip.show(); 
      delay(wait);
    }
  }
}

/*Used in normal behaviour before ListenCode*/
void colorWipeHalf(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { 
    if ((i%2) == 0){
      strip.setPixelColor(i, color);
    }
    else{
      strip.setPixelColor(i, 0);   
    }
    strip.show();
    delay(wait);
  }
}

/*Used to change between modes*/
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}

/*Used on Demo Mode*/
void rainbow(int wait) {
    for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
      for(int i=0; i<strip.numPixels(); i++) { 
        if ((i%2) == 0){
          int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
          strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
        }
        else{
          strip.setPixelColor(i, 0);   
        }
      }
    strip.show(); 
    delay(wait);
    }
}

/*RADIO FREQUENCY FUNCTIONS*/

void listenCode(){
  timer=0;
  timeOne = millis();
  while (timer<4000){ //While have not past 4 seconds...
    if (mySwitch.available()) { //Ask if get a signal
      output(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),mySwitch.getReceivedProtocol()); //Get parameters and send it to output
      mySwitch.resetAvailable();
    }
     timeTwo = millis();
     timer = timeTwo - timeOne;
  }
 }

void output(unsigned long decimal, unsigned int length, unsigned int delay, unsigned int* raw, unsigned int protocol) {
  const char* b = dec2binWzerofill(decimal, length);
  time = millis();
  sec = time/1000;
  /* If you uncomment this section, you could print on the serial monitor the results the 433MHz module hear.*/
  /*
  Serial Print Results
  Serial.print("Time detected: ");
  Serial.print(sec);
  Serial.println(" seconds");
  Serial.print("Decimal: ");
  Serial.print(decimal);
  Serial.print(" (");
  Serial.print(length);
  Serial.print("Bit) Binary: ");
  Serial.print(b);
  */
  if (strcmp (b,"011111111110010111010010") == 0) {
    Serial.print("DemoCode Detected");
    rainbow(5); 
    }
  if (strcmp (b,"010011011101011101100101") == 0) {
    Serial.print("Code 2 Detected");
    colorWipe(strip.Color(0, 0, 0), 50);
    codeD3(); 
    }
  if (strcmp (b,"100001001001101000101101") == 0) {
    Serial.print("Code 5 Detected");
    colorWipe(strip.Color(0, 0, 0), 50);
    codeD5(); 
    }
  }
  
static const char* bin2tristate(const char* bin) {
  static char returnValue[50];
  int pos = 0;
  int pos2 = 0;
  while (bin[pos]!='\0' && bin[pos+1]!='\0') {
    if (bin[pos]=='0' && bin[pos+1]=='0') {
      returnValue[pos2] = '0';
    } 
    else if (bin[pos]=='1' && bin[pos+1]=='1') {
      returnValue[pos2] = '1';
    } 
    else if (bin[pos]=='0' && bin[pos+1]=='1') {
      returnValue[pos2] = 'F';
    }
    else {
      return "not applicable";
    }
    pos = pos+2;
    pos2++;
  }
  returnValue[pos2] = '\0';
  return returnValue;
}

static char * dec2binWzerofill(unsigned long Dec, unsigned int bitLength) {
  static char bin[64]; 
  unsigned int i=0;
  while (Dec > 0) {
    bin[32+i++] = ((Dec & 1) > 0) ? '1' : '0';
    Dec = Dec >> 1;
  }
  for (unsigned int j = 0; j< bitLength; j++) {
    if (j >= bitLength - i) {
      bin[j] = bin[ 31 + i - (j - (bitLength - i)) ];
    } 
    else {
      bin[j] = '0';
    }
  }
  bin[bitLength] = '\0';
  return bin;
}

/*CODE FUNCTIONS*/
/*
With the code's functions you could get the Keys in Hexadecimal, that are written down below:
Challenge 3 - 71 70 . 72 63 . 75 6e . 6f 6c
Challenge 5 - 71 70 . 6f 6c
*/

/*CODE Challenge 3 FUNCION*/

void codeD3(){

//Part 1
  strip.setPixelColor(15,255,0,0); //Number 7
  strip.show();
  delay(1000);
  strip.setPixelColor(15,0,0,0); 
  strip.show();

  strip.setPixelColor(9,255,0,0); //Number 1
  strip.show();
  delay(1000);
  strip.setPixelColor(9,0,0,0); 
  strip.show(); 

  strip.setPixelColor(15,255,0,0); //Number 7
  strip.show();
  delay(1000);
  strip.setPixelColor(15,0,0,0); 
  strip.show();

  strip.setPixelColor(8,255,0,0); //Number 0
  strip.show();
  delay(1000);
  strip.setPixelColor(8,0,0,0);
  strip.show();

//Part 2

  strip.setPixelColor(15,219,191,33); //Number 7
  strip.show();
  delay(1000);
  strip.setPixelColor(15,0,0,0); 
  strip.show();

  strip.setPixelColor(10,219,191,33); //Number 2
  strip.show();
  delay(1000);
  strip.setPixelColor(10,0,0,0); 
  strip.show();

  strip.setPixelColor(14,219,191,33); //Number 6
  strip.show();
  delay(1000);
  strip.setPixelColor(14,0,0,0); 
  strip.show();

  strip.setPixelColor(11,219,191,33); //Number 3
  strip.show();
  delay(1000);
  strip.setPixelColor(11,0,0,0); 
  strip.show();

//Part 3

  strip.setPixelColor(15,0,0,255); //Number 7
  strip.show();
  delay(1000);
  strip.setPixelColor(15,0,0,0); 
  strip.show();

  strip.setPixelColor(13,0,0,255); //Number 5
  strip.show();
  delay(1000);
  strip.setPixelColor(13,0,0,0); 
  strip.show();

  strip.setPixelColor(14,0,0,255); //Number 6
  strip.show();
  delay(1000);
  strip.setPixelColor(14,0,0,0); 
  strip.show();

  strip.setPixelColor(6,0,0,255); //Number E
  strip.show();
  delay(1000);
  strip.setPixelColor(6,0,0,0); 
  strip.show();

//Part 4

  strip.setPixelColor(14,0,255,0); //Number 6
  strip.show();
  delay(1000);
  strip.setPixelColor(14,0,0,0); 
  strip.show();

  strip.setPixelColor(7,0,255,0); //Number F
  strip.show();
  delay(1000);
  strip.setPixelColor(7,0,0,0); 
  strip.show();

  strip.setPixelColor(14,0,255,0); //Number 6
  strip.show();
  delay(1000);
  strip.setPixelColor(14,0,0,0); 
  strip.show();

  strip.setPixelColor(4,0,255,0); //Number C
  strip.show();
  delay(1000);
  strip.setPixelColor(4,0,0,0);
  strip.show(); 
  
}

/*CODE Challenge 5 FUNCION*/

void codeD5(){
  strip.setPixelColor(15,255,0,0); //Number 7
  strip.show();
  delay(1000);
  strip.setPixelColor(15,0,0,0); 
  strip.show();

  strip.setPixelColor(9,255,0,0); //Number 1
  strip.show();
  delay(1000);
  strip.setPixelColor(9,0,0,0); 
  strip.show(); 

  strip.setPixelColor(15,255,0,0); //Number 7
  strip.show();
  delay(1000);
  strip.setPixelColor(15,0,0,0); 
  strip.show();

  strip.setPixelColor(8,255,0,0); //Number 0
  strip.show();
  delay(1000);
  strip.setPixelColor(8,0,0,0);
  strip.show();   

  strip.setPixelColor(14,219,191,33); //Number 6
  strip.show();
  delay(1000);
  strip.setPixelColor(14,0,0,0);
  strip.show(); 

  strip.setPixelColor(7,219,191,33); //Number F
  strip.show();
  delay(1000);
  strip.setPixelColor(7,0,0,0);
  strip.show(); 

  strip.setPixelColor(14,219,191,33); //Number 6
  strip.show();
  delay(1000);
  strip.setPixelColor(14,0,0,0);
  strip.show(); 

  strip.setPixelColor(4,219,191,33); //Number C
  strip.show();
  delay(1000);
  strip.setPixelColor(4,0,0,0);
  strip.show(); 
}
