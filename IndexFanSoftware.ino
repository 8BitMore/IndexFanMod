// Settings you can change -------------------------------------------

#define FAN_SPEED 35                                                // 
#define PWM_PIN   9                                                 // Pin for sending PWM signal to fan, D9 should be used
#define POT_PIN   A0                                                // Pin for reading potentiometer, A0 - A7 can be used

// Setup -------------------------------------------------------------

bool debug = false;                                                 // Set debug mode

#include <FanController.h>                                          // Import fan library, https://github.com/GiorgioAresu/FanController
FanController fan(2, 1000, PWM_PIN);                                // Initialize fan library, tachometer pin (unused), update interval, PWM pin

void setup() {

  if(debug){ Serial.begin(9600); }                                  // Start Serial communication via USB, only enabled when debugging
  
  fan.begin();                                                      // Start up fan
  fan.setDutyCycle(0);                                              // Immediately turn off fan

  pinMode(POT_PIN, INPUT);                                          // Set the potentiometer pin as input
  digitalWrite(POT_PIN, LOW);                                       // Set the potentiometer pin low (theoretically not necessary)

}

// Loop --------------------------------------------------------------

void loop() {

  byte         potSamples = 100;                                    // Number of samples for reading the potentiometer, max = 255
  unsigned int potValue   = 0;                                      // Set "potValue" to 0
  
  for(byte i = 0; i < potSamples; i++){                             // Do this "potSamples" amount of times
    potValue += round( (100.0 / 1024.0) * analogRead(POT_PIN) );    // Calculate a 0-100 value from the potentiometer and add it to "potValue"
  }
  
  potValue = potValue / potSamples;                                 // Calculate the average of "potValue" to smooth out potentiometer readings

  if(debug){ Serial.print( "POT:" + String(potValue) + ","); }      // Print potentiometer value, only enabled when debugging  



  byte fanSpeed = 0;                                                // Set "fanSpeed" to 0
  byte idleVal  = 25;                                               // Value of Potentiometer until it stops idleing at "FAN_SPEED" speed

  if(potValue <= 5                       ){ fanSpeed = 0;  }        // If potentiometer is 0% - 5% Turn fan completly off
  if(potValue >  5 && potValue <= idleVal){ fanSpeed = FAN_SPEED; } // If potentiometer is 6% - "idleVal"% run fan at "FAN_SPEED" speed
  if(potValue > idleVal                  ){                         // If potentiometer is "idleVal"% - 100% 
    
    float fanRange = (100.0 - FAN_SPEED) / (100.0 - idleVal);       // Calculate remaining speed range of fan, "FAN_SPEED" - 100%
    float potRange = potValue - idleVal;                            // Calculate remaining turn range of potentiometer, "idleVal" - 100%;
    fanSpeed = round((fanRange * potRange) + FAN_SPEED);            // Calculate "fanSpeed"
    if(fanSpeed >= 99){ fanSpeed = 100; }
    
  }

  fan.setDutyCycle(fanSpeed);                                       // Set PWM Dutycyle to "fanSpeed"

  if(debug){Serial.print("FAN:"+String(fanSpeed)+",");}             // Print fan speed, only enabled when debugging
  if(debug){Serial.println("MIN:0,MAX:110");}                       // Print min, max, to make plot look neater, only enabled when debugging
}
