#include <vector>
using std::vector;
using std::move;

#include <thread>
using std::thread;
using std::this_thread::sleep_for;

#include <mutex>
using std::mutex;

#include <memory>
using std::unique_ptr;

struct freq_and_dec_struct
{
    string freq;
    string dec;
};

class API
{
    public:
        //returns STD::move decAVG
        auto GetLEDFrame ();
        void Start (string);
        API();
        
    private:
        void SoXShellScript ();
        void ReadMP3File ();
        void FreqAndDecAVG (vector<freq_and_dec_struct> &);
        static void ClassProxy (void *);

        bool endjob;
        bool endthread;
        mutex loading;
        pid_t pid;       
        string currentmp3file;
        string freqfilelocation;
        vector<float> decAVG;
        unique_ptr<thread> freqthread;
};