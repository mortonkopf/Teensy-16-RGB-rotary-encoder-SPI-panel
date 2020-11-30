
#include "chips.h"
#include "rotControls.h"

//----------variables for stepper leds -----need to change stepTime for BPM count or whatever
unsigned long previousMillis = 0;//temporary - for timing the stepper leds
unsigned long stepTime =150; //temporary - for timing the stepper leds

//--------------------------------setup()-----------------------------//
void setup(){
  chipsBegin();//initialise the mcp23s17 chips
  delay(400); //let things settle
  setPins(); //set mcp chip pins to input or output
  delay(400); //let things settle

//-----------do a startup led routine on the rotaries for DEBUG
  startUp();
  allKnobsOff1();//switch off all rotary led colours
  delay(400);//keep things settled

}

//--------------------------------loop()-------------------------------//
void loop(){
  
//-------------led stepper-----------------------//
 unsigned long currentMillis = millis();
       if((currentMillis - previousMillis) > stepTime) {
       previousMillis = currentMillis; 
       getStep();//change stepTime to bpm variable, using fixed value as DEBUG
       }

//-----------read ports all the time, we are not using interupts-----//
for(volatile int yy=0; yy<8; yy++){
mcpChips[yy]=mcpArray[yy].readGpioPort();//read all chips
}


//---------check to see if a button has been pushed
checkRotButtons();///DEBUG - function contains a serial print for debug. Change to somethng useful

//---------see if rotary is sending a signal
checkRot();
for(int x=0;x<16;x++){
  if(Counter[x] != oldCounter[x]){
  /////DEBUG = do something if change in rotaries. change for something useful
  Serial.print("rotary ");//debug
  Serial.print(rotaryOrder[x]);//debug
  Serial.print("...value = ");//debug
  Serial.println(Counter[x]); //debug
  oldCounter[x] =Counter[x];
    }
  }

}//end of loop
