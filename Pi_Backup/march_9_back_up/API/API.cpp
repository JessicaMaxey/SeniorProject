#include <iostream>
using std::cout;
using std::endl;

#include <stdio.h>

#include <unistd.h>

#include <thread>
using std::this_thread::sleep_for;

#include "API.h"

const int NUM_LED_ROWS = 13;
const int MAX_SIZE_INPUT_RANGE = 4000;
const int MIN_SIZE_INPUT_RANGE = 22;
const string CUR_MP3_FILE = "/home/pi/Music/short.mp3";
const string SHELL_FILE_LOC = "";

API::API()
{ }

API::~API()
{ }

vector<float> API::ReadBuff(vector<FreqPower> buffer)
{
    float freqvalue = 0;
    vector<FreqPower> temp;

    return FreqAndDecAVG(buffer);
}

vector<float> API::FreqAndDecAVG (vector<FreqPower> buffer)
{
    vector<float> tempAVG;
    int sizeofbuffer = buffer.size();

    //set numbers to be able to divide both groups by 13 to
    //get the number of chunks to do math on
    int numinDECchunks = (sizeofbuffer / NUM_LED_ROWS);


    //overflow of freq and dec that do not fit into the division of 13
    //that still needs to be taken care of
    int remaining = sizeofbuffer % NUM_LED_ROWS;
    float tempdec = 0;

    //adds up all the dec
    for (int i = 0; i < sizeofbuffer; i++)
    {
        if(tempdec < buffer[i].power)
        {
            tempdec = (buffer[i].power);
        }


        if ((i + 1) % numinDECchunks == 0)
        {
            if (remaining > 0)
            {
                i++;
                remaining--;

                if(tempdec < buffer[i].power)
                {
                    tempdec = (buffer[i].power);
                }
            }

            //Find the average

            //tempdec = (tempdec / numinDECchunks);

            //cout << "value in tempdec: " << tempdec << endl;

            //Add it to the vector
            tempAVG.emplace_back(tempdec);

            tempdec = 0;
        }
    }

    return tempAVG;
}
