#include <vector>
#include <wiringPi.h>
#include <wiringPiSPI.h>

using std::vector;

class Nano
{
public:
    Nano();
    void LEDONandOFF (vector<float>&);
    void LEDRed();
    void LEDPurple();
    void LEDBlue();
    void LEDTeal();
    void LEDGreen();
    void LEDYellow();
    void LEDallOFF ();
    void LEDFadeRainbow(int);
    void LEDColorShift(int);
    void LEDFadeONandOFF(int);
    void LEDSync();

private:

};
