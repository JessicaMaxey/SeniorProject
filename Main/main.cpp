#include <stdio.h>

#include <iostream>
using std::cout;
using std::endl;

#include <thread>
using std::this_thread::sleep_for;

//#include "SoX_API/API.h";
//#include "NanoCom/Nano.h";

#include "/home/jess/Documents/Repo/SeniorProject/NanoCom/Nano.h"
#include "/home/jess/Documents/Repo/SeniorProject/SoX_API/API.h"

int main(int argc, char **argv)
{
    //cout << "Got to main!" << endl;
    
    API api;
    Nano nano;
    
    api.Start("/home/pi/Music/short.mp3");
    
    cout << "Got passed api.Start!" << endl;
    
    while(api.CheckDone() != true)
    {
        cout << "Got passed api.CheckDone!" << endl;

        vector<float> decAVG = api.GetLEDFrame();
        
        nano.LEDONandOFF(decAVG);
        cout << "Got passed nano.LEDONandOFF!" << endl;
    }
    
    /*vector<float> fucks = {60, 1, 20, 80};
    vector<float> too = {1, 22, 33, 44};
    vector<float> much {33, 60, 15, 1};
    
    while (1)
    {
        nano.LEDONandOFF(fucks);
        std::this_thread::__sleep_for(std::chrono::seconds(1), std::chrono::nanoseconds(0));
        
        nano.LEDONandOFF(too);
        std::this_thread::__sleep_for(std::chrono::seconds(1), std::chrono::nanoseconds(0));
        
        nano.LEDONandOFF(much);
        std::this_thread::__sleep_for(std::chrono::seconds(1), std::chrono::nanoseconds(0));
    }*/
    
    //cout << "Finished running" << endl;
}
