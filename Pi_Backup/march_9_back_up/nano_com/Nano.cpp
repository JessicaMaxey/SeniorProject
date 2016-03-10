#include <stdio.h>

#include <math.h>

#include <unistd.h>

#include <iostream>
using std::cout;
using std::endl;

#include <thread>
using std::this_thread::sleep_for;
#include <chrono>
using std::chrono::milliseconds;

#include <wiringPi.h>
#include <wiringPiSPI.h>

#include <cstring>
using std::strlen;

#include "Nano.h"

const int NUM_LEDS = 312;
const int NUM_OF_ROW = 8;
const int NUM_OF_COL = 13;
const float BOX_DB= 300;
const int LED_ON = 15;
const int LED_OFF = 0;
static int fd = 0;


static struct static_initializer
{

    static_initializer() {
        cout << "Static init" << endl;
        wiringPiSetupGpio();
        fd = wiringPiSPISetup(0, 2000000);

        pinMode(17, OUTPUT);
        pinMode(22, OUTPUT);
        pullUpDnControl(17, PUD_OFF);
        pullUpDnControl(22, PUD_OFF);
        digitalWrite(17, 1);
        digitalWrite(22, 1);
    }
} _si;

void Nano::LEDSync()
{
    unsigned char sync_signal = 0;

    digitalWrite(17, 0);
    digitalWrite(22, 0);
    sync_signal = 0xFD;
    wiringPiSPIDataRW(0, (unsigned char *)&sync_signal, 1);
    digitalWrite(17, 1);
    digitalWrite(22, 1);
}

void WriteToLed(short addr, char level)
{
    char led = 0;
    if(addr < 96)
    {
        led = addr;

        //writes out data for led
        digitalWrite(17, 0);
        wiringPiSPIDataRW(0, (unsigned char *)&led, 1);
        wiringPiSPIDataRW(0, (unsigned char *)&level, 1);
        digitalWrite(17, 1);
    }
    else if(addr >= 96 && addr < 192)
    {
        led = addr - 96;

        digitalWrite(22, 0);
        wiringPiSPIDataRW(0, (unsigned char *)&led, 1);
        wiringPiSPIDataRW(0, (unsigned char *)&level, 1);
        digitalWrite(22, 1);
    }
    else if (addr >= 192 && addr < 312)
    {
        //led = temp - 192;

        //writes out data for led
        //wiringPiSPIDataRW(0, (unsigned char *)&led, 1);
        //wiringPiSPIDataRW(0, (unsigned char *)&level, 1);
    }
}

Nano::Nano()
{

}

void Nano::LEDONandOFF (vector<float> & decAVG)
{
    unsigned char level = 0;
    unsigned char led = 0;

    LEDSync();

    //break it down into 13 rows
    //in each row there are 8 leds
    //each box represents decibel value
    //need to set each box to on or off depending on
    //how high the number of dec that comes in
    //color is not a factor at the moment

    for (int i = 0; i < NUM_OF_COL; i++)
    {

        float current_box_db = 0;

        for (int j = 0; j < NUM_OF_ROW; j++)
        {
            LEDSync();
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
                    int temp = (((i * NUM_OF_ROW * 3) + (j * 3)) + count);
                    level = 0;

                    level = LED_ON;

                    WriteToLed(temp, level);

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
                    int temp = (((i * NUM_OF_ROW * 3) + (j * 3)) + count);
                    level = 0;

                    level = LED_OFF;

                    WriteToLed(temp, level);

                    count++;
                }

                //increase the current box to the next level for the
                //next box.
                current_box_db += BOX_DB;
            }
        }

    }

}

void Nano::LEDRed ()
{
    unsigned char level = 0;
    unsigned char led = 0;

    LEDSync();

    //break it down into 13 rows
    //in each row there are 8 leds
    //each box represents decibel value
    //need to set each box to on or off depending on
    //how high the number of dec that comes in
    //color is not a factor at the moment

    for (int i = 0; i < NUM_OF_COL; i++)
    {

        float current_box_db = 0;

        for (int j = 0; j < NUM_OF_ROW; j++)
        {
            LEDSync();
            //if the decibel average coming in from the API
            //is equal to or greater than the value of the hz
            //the current box represents, then turn on that box

                int count = 0;

                //sets all 3 (red, green, blue) in 1 box to on
                while (count < 3)
                {

                    //i is the num in the col, plus j will give us
                    //the box number of the led we are in, and adding
                    //the count makes sure that all 3 leds in the box
                    //get turned on
                    int temp = (((i * NUM_OF_ROW * 3) + (j * 3)) + count);
                    level = 0;

                    if (temp % 3 == 0)
                        level = LED_ON;

                    WriteToLed(temp, level);

                    count++;
                }

        }
    }
}

void Nano::LEDPurple()
{
    unsigned char level = 0;
    unsigned char led = 0;

    LEDSync();

    //break it down into 13 rows
    //in each row there are 8 leds
    //each box represents decibel value
    //need to set each box to on or off depending on
    //how high the number of dec that comes in
    //color is not a factor at the moment

    for (int i = 0; i < NUM_OF_COL; i++)
    {

        float current_box_db = 0;

        for (int j = 0; j < NUM_OF_ROW; j++)
        {
            LEDSync();
            //if the decibel average coming in from the API
            //is equal to or greater than the value of the hz
            //the current box represents, then turn on that box
                int count = 0;

                //sets all 3 (red, green, blue) in 1 box to on
                while (count < 3)
                {

                    //i is the num in the col, plus j will give us
                    //the box number of the led we are in, and adding
                    //the count makes sure that all 3 leds in the box
                    //get turned on
                    int temp = (((i * NUM_OF_ROW * 3) + (j * 3)) + count);
                    level = 0;

                    if (temp % 3 == 0)
                        level = LED_ON;
                    if (temp % 3 == 2)
                        level = LED_ON;

                    WriteToLed(temp, level);

                    count++;
                }
        }
    }
}

void Nano::LEDBlue()
{
    unsigned char level = 0;
    unsigned char led = 0;

    LEDSync();

    //break it down into 13 rows
    //in each row there are 8 leds
    //each box represents decibel value
    //need to set each box to on or off depending on
    //how high the number of dec that comes in
    //color is not a factor at the moment

    for (int i = 0; i < NUM_OF_COL; i++)
    {

        float current_box_db = 0;

        for (int j = 0; j < NUM_OF_ROW; j++)
        {
            LEDSync();
            //if the decibel average coming in from the API
            //is equal to or greater than the value of the hz
            //the current box represents, then turn on that box
                int count = 0;

                //sets all 3 (red, green, blue) in 1 box to on
                while (count < 3)
                {

                    //i is the num in the col, plus j will give us
                    //the box number of the led we are in, and adding
                    //the count makes sure that all 3 leds in the box
                    //get turned on
                    int temp = (((i * NUM_OF_ROW * 3) + (j * 3)) + count);
                    level = 0;

                    if (temp % 3 == 2)
                        level = LED_ON;

                    WriteToLed(temp, level);

                    count++;
                }

        }
    }
}

void Nano::LEDTeal()
{
    unsigned char level = 0;
    unsigned char led = 0;

    LEDSync();

    //break it down into 13 rows
    //in each row there are 8 leds
    //each box represents decibel value
    //need to set each box to on or off depending on
    //how high the number of dec that comes in
    //color is not a factor at the moment

    for (int i = 0; i < NUM_OF_COL; i++)
    {

        float current_box_db = 0;

        for (int j = 0; j < NUM_OF_ROW; j++)
        {
            LEDSync();
            //if the decibel average coming in from the API
            //is equal to or greater than the value of the hz
            //the current box represents, then turn on that box

                int count = 0;

                //sets all 3 (red, green, blue) in 1 box to on
                while (count < 3)
                {

                    //i is the num in the col, plus j will give us
                    //the box number of the led we are in, and adding
                    //the count makes sure that all 3 leds in the box
                    //get turned on
                    int temp = (((i * NUM_OF_ROW * 3) + (j * 3)) + count);
                    level = 0;

                    if (temp % 3 == 1)
                        level = LED_ON;
                    if (temp % 3 == 2)
                        level = LED_ON;

                    WriteToLed(temp, level);

                    count++;
                }

        }
    }
}

void Nano::LEDGreen()
{
    unsigned char level = 0;
    unsigned char led = 0;

    LEDSync();

    //break it down into 13 rows
    //in each row there are 8 leds
    //each box represents decibel value
    //need to set each box to on or off depending on
    //how high the number of dec that comes in
    //color is not a factor at the moment

    for (int i = 0; i < NUM_OF_COL; i++)
    {

        float current_box_db = 0;

        for (int j = 0; j < NUM_OF_ROW; j++)
        {
            LEDSync();
            //if the decibel average coming in from the API
            //is equal to or greater than the value of the hz
            //the current box represents, then turn on that box

                int count = 0;

                //sets all 3 (red, green, blue) in 1 box to on
                while (count < 3)
                {

                    //i is the num in the col, plus j will give us
                    //the box number of the led we are in, and adding
                    //the count makes sure that all 3 leds in the box
                    //get turned on
                    int temp = (((i * NUM_OF_ROW * 3) + (j * 3)) + count);
                    level = 0;

                    if (temp % 3 == 1)
                        level = LED_ON;

                    WriteToLed(temp, level);

                    count++;
                }

        }
    }
}

void Nano::LEDYellow()
{
    unsigned char level = 0;
    unsigned char led = 0;

    LEDSync();

    //break it down into 13 rows
    //in each row there are 8 leds
    //each box represents decibel value
    //need to set each box to on or off depending on
    //how high the number of dec that comes in
    //color is not a factor at the moment

    for (int i = 0; i < NUM_OF_COL; i++)
    {

        float current_box_db = 0;

        for (int j = 0; j < NUM_OF_ROW; j++)
        {
            LEDSync();
            //if the decibel average coming in from the API
            //is equal to or greater than the value of the hz
            //the current box represents, then turn on that box

                int count = 0;

                //sets all 3 (red, green, blue) in 1 box to on
                while (count < 3)
                {

                    //i is the num in the col, plus j will give us
                    //the box number of the led we are in, and adding
                    //the count makes sure that all 3 leds in the box
                    //get turned on
                    int temp = (((i * NUM_OF_ROW * 3) + (j * 3)) + count);
                    level = 0;

                    if (temp % 3 == 1)
                        level = LED_ON;
                    if (temp % 3 == 0)
                        level = LED_ON;

                    WriteToLed(temp, level);

                    count++;
                }

        }
    }
}
//Cycles through 6 colors, goes from completely off
//to completely on and back off, then goes to the next
//color
void Nano::LEDFadeONandOFF(int speed)
{
    char level[3] = {0};
    char led = 0;

    LEDSync();

    int count = 0;
    int m_count = 6;
    while(m_count > 0)
    {
        cout << m_count << '\n';
        m_count--;
        LEDSync();

        for (int i = 0; i < 32; i++)
        {

            if(count == 0)
            {
                //for red led levels
                level[0] = ((sin(((M_PI * i) / 15) + (M_PI * 1.5)) / 2) + .5) * 15;
                level[1] = 0;
                level[2] = 0;
            }
            else if(count == 1)
            {
                //for red led levels
                level[0] = ((sin(((M_PI * i) / 15) + (M_PI * 1.5)) / 2) + .5) * 15;
                //for green led levels
                level[1] = ((sin(((M_PI * i) / 15) + (M_PI * 1.5)) / 2) + .5) * 15;
                level[2] = 0;
            }
            else if(count == 2)
            {
                level[0] = 0;
                //for green led levels
                level[1] = ((sin(((M_PI * i) / 15) + (M_PI * 1.5)) / 2) + .5) * 15;
                level[2] = 0;
            }
            else if(count == 3)
            {
                level[0] = 0;
                //for green led levels
                level[1] = ((sin(((M_PI * i) / 15) + (M_PI * 1.5)) / 2) + .5) * 15;
                //for blue led levels
                level[2] = ((sin(((M_PI * i) / 15) + (M_PI * 1.5)) / 2) + .5) * 15;
            }
            else if(count == 4)
            {
                level[0] = 0;
                level[1] = 0;
                //for blue led levels
                level[2] = ((sin(((M_PI * i) / 15) + (M_PI * 1.5)) / 2) + .5) * 15;
            }
            else if(count == 5)
            {
                //for red led levels
                level[0] = ((sin(((M_PI * i) / 15) + (M_PI * 1.5)) / 2) + .5) * 15;
                level[1] = 0;
                //for blue led levels
                level[2] = ((sin(((M_PI * i) / 15) + (M_PI * 1.5)) / 2) + .5) * 15;

            }

            if (i == 31)
            {
                sleep_for(milliseconds(100));
                if(count < 5)
                {
                    count++;
                }
                else
                    count = 0;
            }

            for(int j = 0; j < 312; j++)
            {
                unsigned char temp = level[j % 3];

                WriteToLed(j, temp);
            }
            sleep_for(milliseconds(10 * speed));

        }




    }
}

void Nano::LEDColorShift(int speed)
{
    cout << "Beginning color shift" << endl;
    char level[3] = {0};

    unsigned long long x = 0;
    unsigned long long count = 200;

    LEDSync();

    int m_count = 180;
    while(m_count > 0)
    {
        LEDSync();
        count = 0;
        m_count--;

        x++;

        for(int i = 0; i < 312; i++)
        {
            if((i % 24) == 0)
            {
                //for red led levels
                level[0] = ((sin((M_PI * (x + count)) / 45) / 2) + .5) * 15;
                //for green led levels
                level[1] = ((sin((M_PI * (x + count)) / 60) / 2) + .5) * 15;
                //for blue led levels
                level[2] = ((sin((M_PI * (x + count)) / 90) / 2) + .5) * 15;

                count++;
            }

            unsigned char temp = level[i % 3];

            WriteToLed(i, temp);
        }

        sleep_for(milliseconds(100 * speed));
    }


}

void Nano::LEDFadeRainbow(int speed)
{
    char level[3] = {0};
    char led = 0;

    LEDSync();

    int x = 0;
    int m_count = 90;
    while(m_count > 0)
    {
        LEDSync();

        //for red led levels
        level[0] = ((sin((M_PI * x) / 15) / 2) + .5) * 15;
        //for green led levels
        level[1] = ((sin((M_PI * x) / 30) / 2) + .5) * 15;
        //for blue led levels
        level[2] = ((sin((M_PI * x) / 60) / 2) + .5) * 15;

        x++;

        for(int i = 0; i < 312; i++)
        {
            unsigned char temp = level[i % 3];

            WriteToLed(i, temp);
        }

        sleep_for(milliseconds(10 * speed));

        m_count--;
    }

}


void Nano::LEDallOFF()
{
    unsigned char level = 0;
    unsigned char led = 0;

    LEDSync();



    //break it down into 13 rows
    //in each row there are 8 leds
    //each box represents decibel value
    //need to set each box to on or off depending on
    //how high the number of dec that comes in
    //color is not a factor at the moment

    for (int i = 0; i < NUM_OF_COL; i++)
    {

        for (int j = 0; j < NUM_OF_ROW; j++)
        {
            LEDSync();
            //if the decibel average coming in from the API
            //is equal to or greater than the value of the hz
            //the current box represents, then turn on that box
            int count = 0;

            //sets all 3 (red, green, blue) in 1 box to on
            while (count < 3)
            {
                //i is the num in the col, plus j will give us
                //the box number of the led we are in, and adding
                //the count makes sure that all 3 leds in the box
                //get turned on
                int temp = (((i * NUM_OF_ROW * 3) + (j * 3)) + count);
                level = 0;

                level = LED_OFF;

                WriteToLed(temp, level);

                count++;

            }
        }

    }



}
