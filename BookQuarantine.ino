#include <FastLED.h>
#include <EEPROM.h>
#include <Wire.h>
#include <ds3231.h>
#define NUM_LEDS 2
#define DATA_PIN 10


struct button1Data{
  int clean = 0; //clean variable add to struct. This is the how clean is it variable. 
  bool buttonPressed = false; //if the button has been pressed. This triggers the timer. Add to Struct.
  int timerCurrent= 0; //The time when the button is Pressed add to struct  
  bool button1Counter = 0;
};
int clean = 0; //clean variable add to struct. This is the how clean is it variable. 
  bool buttonPressed = false; //if the button has been pressed. This triggers the timer. Add to Struct.
  int timerCurrent= 0; //The time when the button is Pressed add to struct  
  bool button1Counter = 0;
//int led1 = 6;
//int led2 = 5;
int button = 3; //button pin

int button1MemoryAddress = 0; //Where we store the struct
//Some debounce logic from RandomNerdTutorials
int buttonState;
int lastButtonState = LOW;
// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 800;    // the debounce time; increase if the output flickers
//For the RTC
struct ts t; 
//Initialize all LEDs
CRGB leds[NUM_LEDS];
int timeTriggered = 7;

void setup() {
  // put your setup code here, to run once:
 
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  Serial.begin(9600);
  lastButtonState = LOW;
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
   // brightness = 50;
   FastLED.setBrightness(2);
  checkLEDState();
  //Initialize the RTC
   Wire.begin();
  DS3231_init(DS3231_INTCN);
  DS3231_get(&t);
  //t.hour = t.hour - 12;

}

void loop() {
  DS3231_get(&t);
  int currentSeconds = ((t.min*60)+(t.sec));
  
  // put your main code here, to run repeatedly:
  int button1Value = digitalRead(button);
  //Serial.println(lastButtonState);
  //build in the debouncer...
  if( button1Value != lastButtonState) {
    // reset the debouncing timer
    lastButtonState = button1Value;
    
    Serial.println(millis()-lastDebounceTime);
  
    if((millis() - lastDebounceTime) > debounceDelay) {
      // whatever the reading is currently, it's been there for longer
      // than the debounce delay, so take it as the actual current state:
      //lastDebounceTime = 0;
      lastDebounceTime = millis();
  
      // if the button state has changed:
      lastButtonState = button1Value;

        if (buttonState == LOW){
          clean ++;
          //delay(500);
          
          if (clean > 2){
            clean = 0;
            }
            Serial.println(clean);
            //TODO: Re-Enable EEPROM.
            //EEPROM.update(button1Address, clean);
            timerCurrent = ((t.min*60)+(t.sec));
            Serial.println (timerCurrent);
            buttonPressed = true; 
            timeTriggered = 1;   
          }
      }     
  }
  if ((currentSeconds >=(timerCurrent+10)) && (buttonPressed == true) && (button1Counter<1)){
    
    //timerCurrent = currentSeconds;
    
        clean++;
        Serial.println("Auto-changing");
        Serial.println(clean);
        button1Counter++;
      }
     if ((currentSeconds >=(timerCurrent+20)) && (buttonPressed == true) && (button1Counter<2)){
    
        clean++;
        Serial.println("Auto-changing");
        Serial.println(clean);
        button1Counter++;
      }
      if ((currentSeconds >=(timerCurrent+30)) && (buttonPressed == true) && (button1Counter<3)){
  
  //timerCurrent = currentSeconds;
  
      clean = 0;
      Serial.println("Auto-changing");
      Serial.println(clean);
      button1Counter = 0;
      buttonPressed = false;
     }
  switch (clean){
    case 0:
        FastLED.clear();
        leds[0] = CRGB::Green;
        leds[1] = CRGB::Green;
        FastLED.show();
          break;    
    case 1 :
      //digitalWrite(led1, HIGH);
      FastLED.clear(); 
      leds[0] = CRGB::Red;
      leds[1] = CRGB::Red;
      FastLED.show();
         break;
   case 2 :
     FastLED.clear();
     leds[0] = CRGB::Yellow;
     leds[1] = CRGB::Red;
     FastLED.show();
        break;
                 
   default:
     leds[0] = CRGB::Blue;
     leds[1] = CRGB::Red;
     FastLED.show();
        break;
  }
}


void checkLEDState(){
  Serial.println("LED status after restart: ");
  clean = EEPROM.read(button1MemoryAddress);
  
  //We should consistently use address 0 as our test address for one-bit opperations).
  Serial.println("The first LED status is " );
  Serial.println (clean);
  Serial.println(" I think.");
}
