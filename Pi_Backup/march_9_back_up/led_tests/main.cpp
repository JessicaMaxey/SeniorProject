#include <iostream>
#include <cstdio>
#include <ctime>
using std::clock_t;
using std::clock;

#include <thread>
using std::this_thread::sleep_for;
#include <chrono>
using std::chrono::milliseconds;

#include <unistd.h>
#include <stdio.h>
#include <signal.h>


#include <wiringPi.h>
#include <AudioAnalyzer.h>
#include <SoXDriver.h>
#include <Nano.h>
#include <API.h>

void DoStuff()
{
    API api;
    Nano n;

    auto start = std::chrono::high_resolution_clock::now();
    sleep(1);

    //While the job has not finished
    while (SoxDriver::CheckRunning())
    {
        if (SoxDriver::CheckAvailable())
        {
                auto buffer = SoxDriver::GetAvailableFrame();
                auto ledVals = api.ReadBuff(buffer);
                n.LEDONandOFF(ledVals);
        }
        else
            sleep_for(std::chrono::milliseconds(5));
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    std::cout << "seconds to finish: " << duration.count() << std::endl;

    n.LEDallOFF();
}

int main()
{
    SoxDriver::init();

    std::thread doStuff(DoStuff);

    SoxDriver::GenerateFreqFrames("/home/pi/Music/Aint No Rest For The Wicked.mp3");

    doStuff.join();


    //Uncomment for led color tests
    //Nano n;
    //n.LEDFadeRainbow();
    //n.LEDColorShift();
    //n.LEDFadeONandOFF();

    return 0;
}
