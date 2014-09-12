

/*
  DMX_Slave.ino - Example code for using the Conceptinetics DMX library
 Copyright (c) 2013 W.A. van der Meeren <danny@illogic.nl>.  All right reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


#include <Conceptinetics.h>
#include <Wire.h>
#include <inttypes.h>
//
// CTC-DRA-13-1 ISOLATED DMX-RDM SHIELD JUMPER INSTRUCTIONS
//
// If you are using the above mentioned shield you should 
// place the RXEN jumper towards G (Ground), This will turn
// the shield into read mode without using up an IO pin
//
// The !EN Jumper should be either placed in the G (GROUND) 
// position to enable the shield circuitry 
//   OR
// if one of the pins is selected the selected pin should be
// set to OUTPUT mode and set to LOGIC LOW in order for the 
// shield to work
//

//
// The slave device will use a block of 10 channels counting from
// its start address.
//
// If the start address is for example 56, then the channels kept
// by the dmx_slave object is channel 56-66
//
#define DMX_SLAVE_CHANNELS   9 

//
// Pin number to change read or write mode on the shield
// Uncomment the following line if you choose to control 
// read and write via a pin
//
// On the CTC-DRA-13-1 shield this will always be pin 2,
// if you are using other shields you should look it up 
// yourself
//
#define RXEN_PIN                2


// Configure a DMX slave controller
//DMX_Slave dmx_slave ( DMX_SLAVE_CHANNELS );

// If you are using an IO pin to control the shields RXEN
// the use the following line instead
DMX_Slave dmx_slave ( DMX_SLAVE_CHANNELS , RXEN_PIN );

const int ledPin = 13;
const int LONGSIZE = sizeof(long int);
unsigned long tmp;
uint8_t command, oldCommand;
// the setup routine runs once when you press reset:
void setup() {             

  // Enable DMX slave interface and start recording
  // DMX data
  //'
  dmx_slave.enable ();  

  // Set start address to 1, this is also the default setting
  // You can change this address at any time during the program
  dmx_slave.setStartAddress (500);

  // Set led pin as output pin
  pinMode ( ledPin, OUTPUT );
  Wire.begin(); // join i2c bus (address optional for master)
  oldCommand = 0;

}


// the loop routine runs over and over again forever:
void loop() 
{
  //
  // EXAMPLE DESCRIPTION
  //
  // If the first channel comes above 50% the led will switch on
  // and below 50% the led will be turned off

  // NOTE:
  // getChannelValue is relative to the configured startaddress

   command=dmx_slave.getChannelValue (1);
//   command++;

  if(command != oldCommand) {
    oldCommand=command;
    switch(command){
    case 10:
      sendCommand(1,2,1);
      break;
    case 20:
      sendCommand(2,2,1);
      break;
    case 100:
      sendCommand(3,2,1);
      break;
    case 200:
      sendCommand(4,2,2);
      break;
    case 210:
      sendCommand(5,4,2);
      break;
    case 220:
      sendCommand(6,6,2);
      break;
    case 230:
      sendCommand(7,8,2);
      break;
    }
  }

  delay(60);
}

void sendCommand(uint8_t _command, uint8_t start, uint8_t len) {
  digitalWrite(ledPin, HIGH);
  int i;
  unsigned char buf[LONGSIZE];
  for(i=0; i < LONGSIZE; i++)
    buf[i]=0;
  for (i=0; i < len; i++)
    buf[1-i]=dmx_slave.getChannelValue (start+i);
  if(_command < 4){
      tmp=1234567;
      memcpy(buf, &tmp, 4);
  }
  Wire.beginTransmission(2); 
  for(i=0; i < LONGSIZE; i++)
      Wire.write(buf[i]);        // sends byte DMX1
  Wire.write(_command);        // sends byte DMX1
       // sends byte DMX1
  Wire.endTransmission();    // stop transmitting

  digitalWrite(ledPin, LOW);
}


