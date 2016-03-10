#include <vector>
using std::vector;
using std::move;

#include <thread>
using std::thread;

#include <mutex>
using std::mutex;

#include <memory>
using std::unique_ptr;

#include <string>
using std::string;

#include <SoXDriver.h>

class API
{
    public:
        API();
        ~API();
        vector<float> ReadBuff (vector<FreqPower>);
        vector<float> FreqAndDecAVG (vector<FreqPower>);
};
