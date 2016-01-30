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

#include <string>
using std::string;

struct freq_and_dec_struct
{
    string freq;
    string dec;
};

class API
{
    public:
        //returns STD::move decAVG
        vector<float> &&  GetLEDFrame ();
        void Start (string);
        API();
        bool CheckDone();

    private:
        void SoXShellScript ();
        void ReadMP3File ();
        void FreqAndDecAVG (vector<freq_and_dec_struct> &);
        static void ClassProxy (void *);

        
        volatile bool endthread;
        mutex dataavailablemtx;
        volatile bool dataavailable;
        mutex joblockmtx;
        volatile bool jobavailable;
        
        pid_t pid;       
        string currentmp3file;
        string freqfilelocation;
        vector<float> decAVG;
        unique_ptr<thread> freqthread;
};