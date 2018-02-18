/*
Arduino uno reading eeprom 24c16
pins    Arduino     24c16
        5V          VCC
        GND         GND
        SCL         SCL
        SDA         SDA
Dont use analog pin A0,or raplace it in code to free analog pin        
*/
#include <Wire.h>
const byte DEVADDR = 0x50;
int changeaddress, randnumber;
byte msgf[16] = {
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
   };
   
void setup() 
{
   Wire.begin();
   Serial.begin(115200);
   Serial.println("press c to clear data");
   Serial.println("press s to show data");
   Serial.println("press r to change a random number for address");
   Serial.println("press h to help");
}

void loop() 
{   
    if (Serial.available()) 
    {
        byte incomingByte = Serial.read();
        //Serial.println(incomingByte);
        //////////////////////////////////////////////////////////////////////////////
        if(incomingByte == 99)
        {
          for(int i = 15; i <= 255; i+=16)
            {
            eeprom_write_page(DEVADDR, i, msgf, 16);
            }
            Serial.println("Data cleared");
            Serial.println();
        }
        //////////////////////////////////////////////////////////////////////////////
        if(incomingByte == 115)
        {
          for (int i = 0; i <= 255; i++) 
            {
          byte b = eeprom_read_byte(DEVADDR, i);
          Serial.print(b, DEC);
          Serial.print(' ');
          if((i+1)%16 == 0){
          //Serial.print("  //");
          //Serial.print(i + 1);
            Serial.println();
            }
          }
          Serial.println();
        }
        //////////////////////////////////////////////////////////////////////////////
        if(incomingByte == 104)
        {
          Serial.println("press c to clear data");
          Serial.println("press s to show data");
          Serial.println("press h to help");
        }
        //////////////////////////////////////////////////////////////////////////////
        if(incomingByte == 114)
        {
          randnumber = random(analogRead(0));
          Wire.beginTransmission(80);
          Wire.write(randnumber - 1);
          Wire.write(randnumber);
          Wire.endTransmission();
          delay(10);
          Serial.println("number of address changed");
        }
    }
}
void eeprom_write_page(byte deviceaddress, unsigned eeaddr,
                      const byte * data, byte length)
{
   // Three lsb of Device address byte are bits 8-10 of eeaddress
   byte devaddr = deviceaddress | ((eeaddr >> 8) & 0x07);
   byte addr    = eeaddr;
   Wire.beginTransmission(devaddr);
   Wire.write(int(addr));
   for (int i = 0; i < length; i++) {
       Wire.write(data[i]);
   }
   Wire.endTransmission();
   delay(10);
}
int eeprom_read_byte(byte deviceaddress, unsigned eeaddr)
{
   byte rdata = -1;

   // Three lsb of Device address byte are bits 8-10 of eeaddress
   byte devaddr = deviceaddress | ((eeaddr >> 8) & 0x07);
   byte addr    = eeaddr;

   Wire.beginTransmission(devaddr);
   Wire.write(int(addr));
   Wire.endTransmission();
   Wire.requestFrom(int(devaddr), 1);
   if (Wire.available()) {
       rdata = Wire.read();
   }
   return rdata;
}
