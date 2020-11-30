#include <SPI.h>
#include <mcp23s17.h>   // import library

/*
 * pinout for Teensy SPI
 * CS - 10
 * MOSI - 11
 * MISO - 12
 * SCK - 13
 */

int SPISpeed =10000000;// 30000000


//to use HAEN, address should be 0x20 to 0x27
//instance - (chip select, address, SPI speed)
//*****************instances of each MCP23S17 Port expander chip****************//
mcp23s17 mcp0(10, 0x20,SPISpeed);//using transactions?);//all three ground
mcp23s17 mcp1(10, 0x21,SPISpeed);//using transactions?);//A0 +ve, A1 gnd, A2 gnd
mcp23s17 mcp2(10, 0x22,SPISpeed);//using transactions?);//A0 gnd, A1 +ve, A2 gnd
mcp23s17 mcp3(10, 0x23,SPISpeed);//using transactions?);//A0 +ve, A1 +ve, A2 gnd
mcp23s17 mcp4(10, 0x24,SPISpeed);//using transactions?);//A0 gnd, A1 gnd, A2 +ve
mcp23s17 mcp5(10, 0x25,SPISpeed);//using transactions?);//A0 +ve, A1 gnd, A2 +ve
mcp23s17 mcp6(10, 0x26,SPISpeed);//using transactions?);//A0 gnd, A1 +ve, A2 +ve
mcp23s17 mcp7(10, 0x27,SPISpeed);//using transactions?);//all three positive

mcp23s17 mcpArray[8] = {mcp7,mcp6,mcp5,mcp4,mcp3,mcp2,mcp1,mcp0};
int chip0, chip1, chip2, chip3,chip4, chip5, chip6, chip7;
int mcpChips[8] {chip0, chip1,chip2,chip3,chip4,chip5,chip6,chip7};//array to hold read values


void chipsBegin(){
  mcp0.begin();
  mcp1.begin();
  mcp2.begin();
  mcp3.begin();
  mcp4.begin();
  mcp5.begin();
  mcp6.begin();
  mcp7.begin();  
}



//for this lib pin numbers are:
//IOB0 = pin0, IOB2 = pin1, etc
//IOA0 = pin8, IOA1 = pin9, etc

void setPins(){
//setup the gpio!
//portB
for(int x=0;x<8;x++){
mcpArray[x].gpioPinMode(0,OUTPUT); // red
mcpArray[x].gpioPinMode(1,OUTPUT);//green
mcpArray[x].gpioPinMode(2,INPUT);//switch
mcpArray[x].gpioPinMode(3,OUTPUT);//blue
mcpArray[x].gpioPinMode(4,INPUT);//rotA
mcpArray[x].gpioPinMode(5,INPUT);//rotB
mcpArray[x].gpioPinMode(6,OUTPUT);//led1
mcpArray[x].gpioPinMode(7,OUTPUT);//led2
//portA 
mcpArray[x].gpioPinMode(8,INPUT);//rotA
mcpArray[x].gpioPinMode(9,INPUT);//rotB
mcpArray[x].gpioPinMode(10,OUTPUT);//red
mcpArray[x].gpioPinMode(11,OUTPUT);//green
mcpArray[x].gpioPinMode(12,INPUT);//switch
mcpArray[x].gpioPinMode(13,OUTPUT);//blue
mcpArray[x].gpioPinMode(14,OUTPUT);//led1
mcpArray[x].gpioPinMode(15,OUTPUT);//led2
  }
}

int stepLeds[4] = {6,7,14,15};
