#include <iostream>
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;

#include <string.h>
using std::string;

#include <stdio.h>

#include <unistd.h>

#include <chrono>
using std::chrono::milliseconds;

#include <signal.h>

#include "API.h"

#include <thread>
using std::this_thread::sleep_for;


const int NUM_LED_ROWS = 13;
const int MAX_SIZE_INPUT_RANGE = 4000;
const int MIN_SIZE_INPUT_RANGE = 22;
const string CUR_MP3_FILE = "/home/pi/Music/short.mp3";
const string SHELL_FILE_LOC = "";

API::API()
{
    jobavailable = 0;
    dataavailable = 0;
    endthread = 0;
    pid = 0;
    freqfilelocation = "/home/pi/Music/mp3freq.txt";
}

void API::Start (string mp3file)
{
    //cout << "Inside API::Start!" << endl;
    joblockmtx.lock();
    dataavailablemtx.lock();
    
    if (pid != 0)
        kill(pid, SIGTERM);
    
    currentmp3file = mp3file;
    //These variables will be generated based on the users
    //input, but for now we have test data.
    //currentmp3file = CUR_MP3_FILE;
    
    //Will read in the mp3 file and generate freq data
    SoXShellScript();
    
    if (freqthread == nullptr)
        freqthread = std::make_unique<thread>(ClassProxy, (void *)(this));
    
    jobavailable = true;
    joblockmtx.unlock();
    
    while (jobavailable == true)
        std::this_thread::yield();
}

void API::SoXShellScript ()
{
    //cout << "Inside API::SoXShellScript!" << endl;
    //fork and save PID
    pid_t temp = fork();
    
    //takes mp3 and generates the freqencies for it using a shell script
    //on another process

    //executes script located at /home/jess/Music/mp3freq, 
    //and passes the location of the mp3 file as a arg
    if (temp)
        pid = temp;
    else
        execl("/bin/sh", "sh", "/home/pi/Music/mp3freq.sh", currentmp3file.data(), (char*)0);
}

void API::ReadMP3File ()
{
    //std::this_thread::__sleep_for(std::chrono::seconds(10), std::chrono::nanoseconds(0));
    cout << "Inside API::ReadMP3File!" << endl;
    while (endthread == 0)
    {
        joblockmtx.lock();
        
        jobavailable = false;
        
        //buffer to store the frequency/decibel data coming in
        vector<freq_and_dec_struct> freq_and_dec_buff;
        vector<float> dec_buff;

        //opening the file that is being written and storing the data
        ifstream ifs(freqfilelocation);
        string line;
        
        
        //whille stream is still alive
        while(ifs.good())
        {         
            cout << "Inside API::ReadMP3File:while(ifs.good())!" << endl;          
            //cout << "Reading File" << endl;
            string frequency = "";
            string decibel = "";
            
            //read in from the file
            while(getline(ifs,line) && jobavailable == 0)
            {   
                cout << "Inside API::ReadMP3File:while(getline(ifs,line) && jobavailable!" << endl;
                //splits on space
                frequency = line.substr(0, line.find(' '));
                decibel = line.substr(line.find(' ') + 2);
                
                float freqvalue = atof(frequency.c_str());
                
                if(freqvalue >= MIN_SIZE_INPUT_RANGE && freqvalue <= MAX_SIZE_INPUT_RANGE)
                {
                    cout << "Inside API::ReadMP3File:if(freqvalue >= MIN_SIZE_INPUT_RANGE && freqvalue <= MAX_SIZE_INPUT_RANGE)" << endl;
                    //cout << "adding shit to buffer to get mathed" << endl;
                    //stores in temp struct    
                    freq_and_dec_struct temp = {frequency, decibel};
                    
                    //adds to the vector        buffer 
                    //so far in the file
                    freq_and_dec_buff.emplace_back(temp);
                    
                    //clears out the varibles for next time
                    frequency = "";
                    decibel = "";
                }
                
                if (freqvalue > MAX_SIZE_INPUT_RANGE && freq_and_dec_buff.size() > 0)
                {
                    cout << "Inside API::ReadMP3File:if (freqvalue > MAX_SIZE_INPUT_RANGE && freq_and_dec_buff.size() > 0)" << endl;   
                    dataavailablemtx.lock();
                    //cout << "Heading into math" << endl;
                    //call the math funtion, and it returns the 13
                    //chunks of decibel values for this buffer
                    FreqAndDecAVG(freq_and_dec_buff);
                    dataavailable = true;
                    dataavailablemtx.unlock();
                    while(dataavailable)
                        std::this_thread::yield();
                    freq_and_dec_buff.clear();
                }
            }
            
            //clears the state flags, and polls the stream
            //this way when the reading catches up with the
            //writing of the file it will contuine with it
            //instead of stopping.
            ifs.clear();            
            
            if (jobavailable)
                break;
        }    
                
        joblockmtx.unlock();
    }
}

void API::FreqAndDecAVG (vector<freq_and_dec_struct> & freq_and_dec_buff)
{
    /*
    //cout << "Inside API::FreqAndDecAVG" << endl;
    int sizeofbuffer = freq_and_dec_buff.size();
    
    //set numbers to be able to divide both groups by 13 to 
    //get the number of chunks to do math on    
    int numinDECchunks = (sizeofbuffer / NUM_LED_ROWS);
    
    
    //overflow of freq and dec that do not fit into the division of 13 
    //that still needs to be taken care of
    int remaining = sizeofbuffer % NUM_LED_ROWS;

    int count = 0;
    
    float tempdec = 0;
    
    //adds up all the dec
    for (int i = 0; i < sizeofbuffer; i++)
    {
        tempdec += (atof(freq_and_dec_buff[i].dec.c_str()));
                
        if ((i + 1) % numinDECchunks == 0)
        {
            if (remaining > 0)
            {
                i++;
                remaining--;
                tempdec += (atof(freq_and_dec_buff[i].dec.c_str()));
            }
        
            //Find the average
            tempdec = (tempdec / numinDECchunks);
    
            //Add it to the vector 
            decAVG.emplace_back(tempdec);    
        }
    }
     */
     
     
    cout << "Inside API::FreqAndDecAVG" << endl;
    int sizeofbuffer = freq_and_dec_buff.size();
    
    //set numbers to be able to divide both groups by 13 to 
    //get the number of chunks to do math on    
    int numinDECchunks = (sizeofbuffer / NUM_LED_ROWS);
    
    
    //overflow of freq and dec that do not fit into the division of 13 
    //that still needs to be taken care of
    int remaining = sizeofbuffer % NUM_LED_ROWS;

    int count = 0;
    
    float tempdec = 0;
    
    //adds up all the dec
    for (int i = 0; i < sizeofbuffer; i++)
    {
        tempdec += (atof(freq_and_dec_buff[i].dec.c_str()));
                
        if ((i + 1) % numinDECchunks == 0)
        {
            if (remaining > 0)
            {
                i++;
                remaining--;
                tempdec += (atof(freq_and_dec_buff[i].dec.c_str()));
            }
        
            //Find the average
            tempdec = (tempdec / numinDECchunks);
                
            //cout << "value in tempdec: " << tempdec << endl;
            
            //Add it to the vector 
            decAVG.emplace_back(tempdec);    
        }
    }
}

vector<float> && API::GetLEDFrame ()
{
    dataavailablemtx.lock();
    auto temp = std::move(decAVG);
    dataavailable = false;
    dataavailablemtx.unlock();
    return std::move(temp);
}

void API::ClassProxy(void * context)
{
    cout << "Inside API::ClassProxy" << endl;
    API * api = (API *)context;
    
    api->ReadMP3File(); 
}

bool API::CheckDone()
{
    cout << "Inside API::CheckDone" << endl;
    return jobavailable;
}