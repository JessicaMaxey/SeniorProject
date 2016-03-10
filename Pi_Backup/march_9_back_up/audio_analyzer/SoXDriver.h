#ifndef SOXDRIVER_H
#define SOXDRIVER_H

#include <memory>
#include <vector>
#include <queue>
#include <mutex>

#include <sox.h>

struct FreqPower {
  float freq, power;
};

class SoxDriver
{
public:
    static void init() {sox_init(); sox_format_init();}
    static void end() {sox_format_quit(); sox_quit();}

    static bool CheckRunning();
    static bool CheckAvailable();
    static std::vector<FreqPower> GetAvailableFrame();
    static void GenerateFreqFrames(std::string fileLocation, float tempo = 1.0);
    static void EndJob();

protected:
    static int SoxCallback(sox_bool done, void * ctx);

    static bool kill_sox;
    static bool running;
    static std::queue<std::vector<FreqPower> > internalBuffer;
    static FILE * soxStream;
    static std::mutex accesslock;
};

#endif // SOXDRIVER_H
