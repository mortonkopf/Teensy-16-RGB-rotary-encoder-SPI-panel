
//-----------------------------------------------------------------------------------------------------//
unsigned char rotTable[8][4]; //first = each chip, 2nd = one place for each bit read 0/1
unsigned char pinState0[16];
unsigned char pinState1[16];
unsigned char stateRot0[16];
unsigned char stateRot1[16];
int Counter[16]; //this is the goto rotary value
int oldCounter[16]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


//------------location of green leds on the mcp chips - used for step led
int maxStep = 15; //this is the number of steps to count up to (0 to 15)

//----------number is mcp pin location for either green led or red led on bicolour 
//through hole led. 6=top row green, 15=bottom row green, 7=red, 14=red
int location[2][16] ={
  {6,6,6,6,6,6,6,6,15,15,15,15,15,15,15,15},
  {7,7,7,7,7,7,7,7,14,14,14,14,14,14,14,14}
}; 

int row = 0;//for led stepper
int oldRow =0;//for led stepper
int stepNo =0;  
int oldStep=0;
int mcpStep[16] ={0,1,2,3,4,5,6,7,  0,1,2,3,4,5,6,7};//holder for steps locations
int buttonNumber[16] ={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};//just to print out 1-16 not 0-15

//----rotary pins read table----for CW or CCW rotation catching//
const unsigned char ttable[7][4] = {
{0x0, 0x2, 0x4, 0x0}, 
{0x3, 0x0, 0x1, 0x10},
{0x3, 0x2, 0x0, 0x0}, 
{0x3, 0x2, 0x1, 0x0},
{0x6, 0x0, 0x4, 0x0}, 
{0x6, 0x5, 0x0, 0x20},
{0x6, 0x5, 0x4, 0x0},
};

//-------rotary order, change to two rows of eight
int rotaryOrder[16]= {1,9,2,10,3,11,4,12,  5,13,6,14,7,15,8,16};

//**************rotary polling bit numbers*************//
//bitread 2 = switch right
//bitread 12 = switch left
//bitread 9 =rot on left
//bitread 8 =rot on left
//bitread 4 =rot on right
//bitread 5 =rot on right


void checkRot(){

for (int x=0;x<8;x++){
rotTable[x][0] = bitRead(mcpChips[x], 4); //are the mcp pins for the rotary connections
rotTable[x][1] = bitRead(mcpChips[x], 5);
rotTable[x][2] = bitRead(mcpChips[x], 8);
rotTable[x][3] = bitRead(mcpChips[x], 9);
  
pinState0[x] = (rotTable[x][0] << 1) | rotTable[x][1];
pinState1[x] = (rotTable[x][2] << 1) | rotTable[x][3];
stateRot0[x] = ttable[stateRot0[x] & 0xf][pinState0[x]];
stateRot1[x] = ttable[stateRot1[x] & 0xf][pinState1[x]];

if (stateRot0[x] ==16)
{Counter[x*2]=Counter[x*2]+1;}
if (stateRot0[x] ==32)
{Counter[x*2]=Counter[x*2]-1;}

if (stateRot1[x] ==16)
{Counter[(x*2)+1]=Counter[(x*2)+1]+1;}

if (stateRot1[x] ==32)
{Counter[(x*2)+1]=Counter[(x*2)+1]-1;}
  }
}



//----------------------for updating the rotary button states----------//
int bstate1[16] = {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0};
unsigned long bcount1[16] = {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0}; // button debounce timer.  Replicate as necessary.

//----rotary button states for on/off
int rotButtonState[16]={0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0};

//====================functions for button push stability on Rotaries===========//
// Have we completed the specified interval since last confirmed event?
// "marker" chooses which counter to check
// Routine by Paul__B on Arduino Forum

// Deal with a button read; true if button pressed and debounced is a new event
// Uses reading of button input, debounce store, state store and debounce interval.
// Routines by Paul__B on Arduino Forum

boolean butndown(char button, unsigned long *marker, int *butnstate, unsigned long interval) {
  switch (*butnstate) {               // Odd states if was pressed, >= 2 if debounce in progress
  case 0: // Button up so far, 
    if (button == HIGH) return false; // Nothing happening!
    else { 
      *butnstate = 2;                 // record that is now pressed
      *marker = millis();             // note when was pressed
      return false;                   // and move on
    }

  case 1: // Button down so far, 
    if (button == LOW) return false; // Nothing happening!
    else { 
      *butnstate = 3;                 // record that is now released
      *marker = millis();             // note when was released
      return false;                   // and move on
    }

  case 2: // Button was up, now down.
    if (button == HIGH) {
      *butnstate = 0;                 // no, not debounced; revert the state
      return false;                   // False alarm!
    }
    else { 
      if (millis() - *marker >= interval) {
        *butnstate = 1;               // jackpot!  update the state
        return true;                  // because we have the desired event!
      }
      else 
        return false;                 // not done yet; just move on
    }

  case 3: // Button was down, now up.
    if (button == LOW) {
      *butnstate = 1;                 // no, not debounced; revert the state
      return false;                   // False alarm!
    }
    else { 
      if (millis() - *marker >= interval) {
        *butnstate = 0;               // Debounced; update the state
        return false;                 // but it is not the event we want
      }
      else 
        return false;                 // not done yet; just move on
    }
  default:                            // Error; recover anyway
    {  
      *butnstate = 0;
      return false;                   // Definitely false!
    }
  }
}


//-----------------------polling--check for rotary button push----//
void checkRotButtons(){
 for(int x=0;x<8;x++){
   if (butndown(bitRead(mcpChips[x],2),&bcount1[x], &bstate1[x], 15UL )) //function from Paul_B
    {
      if(rotButtonState[x]==0){
        mcpArray[x].gpioDigitalWrite(0,HIGH);
        rotButtonState[x] =1;
        //DEBUG--------change print to something useful
        Serial.print("button pressed OFF...");
        Serial.println(buttonNumber[x]);
        }
       else{ mcpArray[x].gpioDigitalWrite(0,LOW);
        rotButtonState[x] =0;
                //DEBUG--------change print to something useful
        Serial.print("button pressed ON...");
        Serial.println(buttonNumber[x]);
        }
      }
      ////////////second row buttons////////////////
   if (butndown(bitRead(mcpChips[x],12),&bcount1[x+8], &bstate1[x+8], 15UL )) //function from Paul_B
    {
      if(rotButtonState[x+8]==0){
        mcpArray[x].gpioDigitalWrite(10,HIGH); //11 = green, 10 = red
        rotButtonState[x+8] =1;
                //DEBUG--------change print to something useful
        Serial.print("button pressed OFF...");
        Serial.println(buttonNumber[x+8]);
        }
       else{ mcpArray[x].gpioDigitalWrite(10,LOW);
        rotButtonState[x+8] =0;
                //DEBUG--------change print to something useful
        Serial.print("button pressed ON...");
        Serial.println(buttonNumber[x+8]);
        }
      }
   
   }
    
}


//**************set knobs All Off at startup****************//
void allKnobsOff1(){
for(int allChips=0;allChips<8;allChips++){
  mcpArray[allChips].gpioDigitalWrite(0,HIGH);
  mcpArray[allChips].gpioDigitalWrite(1,HIGH);
  mcpArray[allChips].gpioDigitalWrite(3,HIGH);
  mcpArray[allChips].gpioDigitalWrite(10,HIGH);
  mcpArray[allChips].gpioDigitalWrite(11,HIGH);
  mcpArray[allChips].gpioDigitalWrite(13,HIGH);  
}

}

//-----------------for rotary leds colours
boolean colours[8][3] = { {1, 1, 1},  // off (black)
                          {1, 1, 0},  // blue
                          {1, 0, 1},  // green
                          {1, 0, 0},  
                          {0, 1, 1},  // red
                          {0, 1, 0},  
                          {0, 0, 1},  
                          {0, 0, 0}}; // white    


//***********add startup colour sequence****??*********//
void startUp(){
//go through rotary knob rgb permutations and show colours at startup
for(int x =0; x<8; x++){
for (int y=0;y<8;y++){
  mcpArray[y].gpioDigitalWrite(0,colours[x][0]);//blue
  mcpArray[y].gpioDigitalWrite(1,colours[x][1]);//green
  mcpArray[y].gpioDigitalWrite(3,colours[x][2]);//light blue
  mcpArray[y].gpioDigitalWrite(10, colours[x][0]);//red
  mcpArray[y].gpioDigitalWrite(11, colours[x][1]);//purple
  mcpArray[y].gpioDigitalWrite(13, colours[x][2]);//yellow
} 
delay(450);
}
}


//--------step the through hole led at teh stepTime pace--------//
void getStep(){

     mcpArray[mcpStep[oldStep]].gpioDigitalWrite(location[oldRow][oldStep],LOW);//switch off previous
     mcpArray[mcpStep[stepNo]].gpioDigitalWrite(location[row][stepNo],HIGH);//switch on next
     
     oldStep = stepNo; //remember the step number to switch off
     oldRow=row;
     
     stepNo++;
      if(stepNo >maxStep)
      {
      stepNo=0;
          // change led row for example
    if (row == 0)
      row = 1;
    else
      row = 0;
      }
}
