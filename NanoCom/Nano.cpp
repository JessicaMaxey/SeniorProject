#include <stdio.h>

#include <unistd.h>

#include <wiringPi/include/wiringPi.h>
#include <wiringPi/include/wiringPiSPI.h>

#include <cstring>
using std::strlen;

#include "Nano.h"

const int NUM_LEDS = 104;
const int NUM_OF_ROW = 8;
const int NUM_OF_COL = 13;
const float BOX_DB= 20;
const int LED_ON = 15;
const int LED_OFF = 0;

void Nano::LEDONandOFF (vector<float> & decAVG)
{
    char level = 0;
    char led = 0;
   
    wiringPiSetup();
    
    wiringPiSPISetup(0, 2000000);
    led = 0xFD;
    wiringPiSPIDataRW(0, (unsigned char *)&led, 1);
    


    //break it down into 13 rows
    //in each row there are 8 leds
    //each box represents 497.25 hz ((4000 "max hz" - 22 "min hz") / 8)
    //need to set each box to on or off depending on 
    //how high the number of dec that comes in
    //don't need to worry about color

    for (int i = 0; i < NUM_OF_COL; i++)
    {
        float current_box_db = 0;
        
        for (int j = 0; j < NUM_OF_ROW; j++)
        {
            //if the decibel average coming in from the API
            //is equal to or greater than the value of the hz
            //the current box represents, then turn on that box
            if (decAVG[i] >= current_box_db)
            {
                int count = 0;
                
                //sets all 3 (red, green, blue) in 1 box to on
                while (count < 3)
                {
                    //i is the num in the col, plus j will give us
                    //the box number of the led we are in, and adding
                    //the count makes sure that all 3 leds in the box
                    //get turned on
                    led = (((i * NUM_OF_ROW * 3) + (j * 3)) + count);
                    level = LED_ON;
                    
                    //writes out data for led
                    wiringPiSPIDataRW(0, (unsigned char *)&led, 1);
                    //writes out data for level
                    wiringPiSPIDataRW(0, (unsigned char *)&level, 1);
                    
                    count++;
                }
                
                //increase the current box to the next level for the
                //next box.
                current_box_db += BOX_DB;
            }
            else
            {
                int count = 0;
                
                //sets all 3 (red, green, blue) in 1 box to on
                while (count < 3)
                {
                    //i is the num in the col, plus j will give us
                    //the box number of the led we are in, and adding
                    //the count makes sure that all 3 leds in the box
                    //get turned on
                    led = (((i * NUM_OF_ROW * 3) + (j * 3)) + count);
                    level = LED_OFF;
                    
                    //writes out data for led
                    wiringPiSPIDataRW(0, (unsigned char *)&led, 1);
                    //writes out data for level
                    wiringPiSPIDataRW(0, (unsigned char *)&level, 1);
                    
                    count++;
                }
                
                //increase the current box to the next level for the
                //next box.
                current_box_db += BOX_DB;
            }
        }
        
        
        /*level = 0;
        
        //Changes how intence the 1st led (red) in the set of 3 is
        if ((i % 3) == 0)
        {
            //brightness/intensity/on-ness of led
            level = 15;
        }
        
        //Changes how intence the 2nd led (green) in the set of 3 is
        if ((i % 3) == 1)
        {
            //brightness/intensity/on-ness of led
            level = 15;
        }
        
        //Changes how intence the 3rd led (blue) in the set of 3 is
        if ((i % 3) == 2)
        {
            //brightness/intensity/on-ness of led
            level = 15;
        }*/
    }
        
}
