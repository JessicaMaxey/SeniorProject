#ifndef AUDIOANALYZER_H
#define AUDIOANALYZER_H

#include <vector>
#include "SoXDriver.h"

class AudioAnalyzer
{
public:
  bool FreqToStepFile(std::string filename, int difficulty);
  AudioAnalyzer(std::vector<FreqPower> && dftFrames, float timeSlice);
  ~AudioAnalyzer();

protected:

  std::vector<FreqPower> && FrameAnalysis(std::vector<FreqPower> && frame);

  std::vector<FreqPower> mDftFrames;
  float mTimeSlice;
};

#endif // AUDIOANALYZER_H
