#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sox.h>

#include "SoXDriver.h"

using std::cout;
using std::endl;

//Static initializers
std::queue<std::vector<FreqPower> > SoxDriver::internalBuffer;
FILE * SoxDriver::soxStream;
std::mutex SoxDriver::accesslock;
bool SoxDriver::running = false;
bool SoxDriver::kill_sox = false;
uint32_t block_size_prealloc = 0;


bool SoxDriver::CheckRunning()
{
    return running || internalBuffer.size() > 0;
}

bool SoxDriver::CheckAvailable()
{
    return internalBuffer.size() > 0;
}

std::vector<FreqPower> SoxDriver::GetAvailableFrame()
{
    std::vector<FreqPower> temp;

    accesslock.lock();

    temp = internalBuffer.front();
    internalBuffer.pop();

    accesslock.unlock();

    return temp;
}

int SoxDriver::SoxCallback(sox_bool done, void * ctx)
{
  int err = 2;
  std::vector<FreqPower> temp;

  while (err == 2)
  {
    FreqPower t = {0.0, 0.0};
    err = fscanf(soxStream, "%f%f", &t.freq, &t.power);
    if (err == 2) //Found two matching strings
    {
      if (temp.capacity() == 0)
        temp.reserve(block_size_prealloc);
      if (t.freq > 20.0 && t.freq < 4000) // make sure that the frequency is in a range that matters
        temp.emplace_back(t);

      if (t.freq  > 4000.0 && temp.size() > 15)
      {
          accesslock.lock();
          internalBuffer.push(temp);
          accesslock.unlock();
          temp.clear();
      }
    }
  }

  if(temp.size() > block_size_prealloc)
    block_size_prealloc = temp.size();

  if (temp.size() > 15)
  {
      accesslock.lock();
      internalBuffer.push(std::move(temp));
      accesslock.unlock();
  }

  //Gross ugly hack to clear stream
  fseek(soxStream, 0, SEEK_SET);
  putc('\0', soxStream);
  fseek(soxStream, 0, SEEK_SET);

  if(kill_sox == true)
  {
    kill_sox = false;
    return SOX_EOF;
  }

  return SOX_SUCCESS;
}

void SoxDriver::EndJob()
{
    kill_sox = true;
}

void SoxDriver::GenerateFreqFrames(std::string fileLocation, float tempo)
{
  kill_sox = false;
  running = true;
  uint32_t error = 0;

  size_t memsize = 0;
  char * memstream = 0;
  soxStream = open_memstream(&memstream, &memsize);

  const char * fileFormat = fileLocation.data() + fileLocation.find('.') + 1;
  auto fileIn = sox_open_read(fileLocation.data(), NULL, NULL, NULL);
  auto fileOut = sox_open_write("/dev/null", &fileIn->signal, NULL, fileFormat, NULL, NULL); //Open a handle to /dev/null to delete output data

  sox_effect_t * soxStat = nullptr;
  sox_effect_t * soxTempo = nullptr;
  sox_effect_t * soxInput = nullptr;
  sox_effect_t * soxOutput = nullptr;

  sox_effects_chain_t * soxChain = nullptr;

  //Load effect "stat" (allows analysis of audio moving through chain)
  soxInput = sox_create_effect(sox_find_effect("input"));
  soxTempo = sox_create_effect(sox_find_effect("tempo"));
  soxStat = sox_create_effect(sox_find_effect("stat"));
  soxOutput = sox_create_effect(sox_find_effect("output"));

  //Options for the stat effect
  std::string tempo_string = std::to_string(tempo);
  auto effectStatOptions = "-freq";
  auto effectTempoOptions = tempo_string.data();

  cout << "Sox driver starting with tempo: " << tempo_string << endl;

  //Add options to effect
  error = sox_effect_options(soxStat, 1, (char * const *)&effectStatOptions);
  error = sox_effect_options(soxTempo, 1, (char * const *)&effectTempoOptions);
  error = sox_effect_options(soxInput, 1, (char * const *)&fileIn);
  error = sox_effect_options(soxOutput, 1, (char * const *)&fileOut); //Null output

  //Create the effect chain to use, we only want to analyze data, so we only need the in, out, and stat effect
  soxChain = sox_create_effects_chain(&fileIn->encoding, soxOutput->out_encoding);

  error = sox_add_effect(soxChain, soxInput, &fileIn->signal, &fileIn->signal);
  error = sox_add_effect(soxChain, soxTempo, &fileIn->signal, &fileIn->signal);
  error = sox_add_effect(soxChain, soxStat, &fileIn->signal, &fileIn->signal);
  error = sox_add_effect(soxChain, soxOutput, &fileIn->signal, &fileIn->signal);

  //For some reason SoX dumps the DFT to stderr, this redirects stderr to another buffer
  auto errorStream = stderr;
  stderr = soxStream;

  //Remove effect handles since they've been taken care of in the effects chain
  free(soxStat);
  free(soxTempo);
  free(soxInput);
  free(soxOutput);

  //Sox flow: chain, callback for status, context for effect callbacks
  sox_flow_effects(soxChain, SoxCallback, NULL);

  free(soxChain);
  sox_close(fileIn);
  sox_close(fileOut);

  stderr = errorStream; //Return to the STDERR stream

  cout << "Size of freq and power array: " << internalBuffer.size() << endl;
  cout << "Bytes consumed: " << internalBuffer.size() * 4 << endl;
  cout << "Prealloc block size: " << block_size_prealloc << endl;
  fclose(soxStream);
  running = false;
}
