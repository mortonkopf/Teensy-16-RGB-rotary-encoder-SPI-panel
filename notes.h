
/*
using the library from SUMOTOY at https://github.com/sumotoy/gpio_expander

Connections for this set up are (--> signifies OUTPUT) 

      red led    IOB-0 [|  U  |] IOA-7 led 
      green led  IOB-1 [|     |] IOA-6  push button
       push gnd  IOB-2 [|     |] IOA-5 led
      blue led   IOB-3 [|     |] IOA-4  led
         rotA    IOB-4 [|     |] IOA-3 rotB
         rotB    IOB-5 [|     |] IOA-2 rotA
        stepled1 IOB-6 [|     |] IOA-1 
      stepled2   IOB-7 [|     |] IOA-0 
                 +VE   [|     |] INT-A --> out to T3 interrupt pin2 (one for each 8bit port)
                 GND   [|     |] INT-B --> out to T3 interrupt pin2
    t35 10  --> CS/SS  [|     |] RST (connect to +)
  t35 13/14 -->  SCK   [|     |] A2 Address bit -connect to either -ve or +ve
     t35 11 -->  MOSI  [|     |] A1 address bit -ditto
     t35 12 <--  MISO  [|_____|] A0 address bit -ditto

Basic Address:  00100 A2 A1 A0 (from 0x20 to 0x27)
A2,A1,A0 tied to ground = 0x20

for this lib pin numbers are:

IOB0 = pin0, IOB2 = pin1, etc
IOA0 =pin8, IOA1 = pin9, etc
*/
/*
 * 0x20 = 100000
 * 0x27 = 100111
 * address bits
 * A0-gnd A1-gnd A2-gnd
 * A0-1 A1-gnd A2-gnd
 * 
 */





/*
 * 
 * The following is the typical sequence of code on the output when
moving from one step to the next:

  Position   Bit1   Bit2
  ----------------------
    Step1     0      0
     1/4      1      0
     1/2      1      1
     3/4      0      1
    Step2     0      0
 * 
 * 
 * 
 * const unsigned char ttable[7][4] = {
  // R_START
  {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
  // R_CW_FINAL
  {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},
  // R_CW_BEGIN
  {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
  // R_CW_NEXT
  {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
  // R_CCW_BEGIN
  {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
  // R_CCW_FINAL
  {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},
  // R_CCW_NEXT
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};*/
