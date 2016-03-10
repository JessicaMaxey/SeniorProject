#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <algorithm>

#include "AudioAnalyzer.h"

AudioAnalyzer::AudioAnalyzer(std::vector<FreqPower> && dftFrames, float timeSlice) : mDftFrames(dftFrames), mTimeSlice(timeSlice)
{
}

AudioAnalyzer::~AudioAnalyzer()
{
}

std::vector<FreqPower> && AudioAnalyzer::FrameAnalysis(std::vector<FreqPower> && frame)
{
  auto comp = [](FreqPower & i, FreqPower & j) -> bool {
    return i.power > j.power;
  };
  
  std::vector<FreqPower> temp = frame;
  std::sort(temp.begin(), temp.end(), comp);
  
  //Grab only the 3 most notable nodes of the frame
  temp.erase(temp.begin() + 3, temp.end());
  
  return std::move(temp);
}

bool AudioAnalyzer::FreqToStepFile(std::string filename, int difficulty)
{
  bool error = false;
  //StepFile to output into
  auto fout = fopen(filename.data(), "w+");
  
  if (fout == nullptr)
    return true;
  
  uint32_t frameSize = 0;
  float max = 0;
  
  //Find the maximum freq out of all frames and their frame size
  for (frameSize = 0; frameSize < mDftFrames.size(); frameSize++)
  {
    if (mDftFrames[frameSize].freq >= max)
      max = mDftFrames[frameSize].freq;
    else
      break; //Found the max frequency and size of each frame
  }
  
  frameSize++; //Need to increment frameSize an extra time to get an accurate count
  
  uint32_t numFrames = mDftFrames.size() / frameSize;
  
  for (uint32_t i = 0; i < numFrames; i++)
  {
    std::vector<FreqPower> frame(mDftFrames.cbegin()+(i*frameSize), (mDftFrames.cbegin()+(i*frameSize + frameSize)));
    
    frame = FrameAnalysis(std::move(frame));
  }
  
  fclose(fout);
  
  return error;
}
