#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S i2s1;          //xy=139,91
AudioMixer4 mixer1;          //xy=312,134
AudioOutputI2S i2s2;         //xy=392,32
AudioAnalyzeFFT1024 fft1024; //xy=467,147
AudioConnection patchCord1(i2s1, 0, mixer1, 0);
AudioConnection patchCord2(i2s1, 0, i2s2, 0);
AudioConnection patchCord3(i2s1, 1, mixer1, 1);
AudioConnection patchCord4(i2s1, 1, i2s2, 1);
AudioConnection patchCord5(mixer1, fft1024);
AudioControlSGTL5000 audioShield; //xy=366,225
// GUItool: end automatically generated code

const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;

// An array to hold the 16 frequency bands
float level[16];

#define N 20
int dataCollect[N];
int curIndex = 0;
float curMean = 0.0f;
float oldMean = 0.0f;

#define WINDOW 100
int indxWindw = 0;
float minWindw = 1024;
float maxWindw = -1;
float curMinWindow = 0;
float curMaxWindow = 100;
float curMeanWindow = 50;
// float oldRangeWindow = 512;

float testIntegral = 0;

#define Ntempo 4
float tempoCollect[Ntempo];
int curIndexTempo = 0;
float curMeanTempoSec = 1000;

float timerTempo0 = -1;

void setup()
{
  setupSerial1();

  // Audio requires memory to work.
  AudioMemory(12);

  // Enable the audio shield and set the output volume.
  audioShield.enable();
  audioShield.inputSelect(myInput);
  audioShield.volume(0.5);
  // configure the mixer to equally add left & right
  mixer1.gain(0, 0.5);
  mixer1.gain(1, 0.5);

  // init data collection
  for (int i = 0; i < N; i++)
  {
    dataCollect[i] = 0;
  }

  for (int i = 0; i < Ntempo; i++)
  {
    tempoCollect[i] = -1;
  }
}

void loop()
{

  float curSec_ = millis() / 1000.;
  float curVal_ = 0;
  float maxLevel = -1;
  float minLevel = 6000;

  if (fft1024.available())
  {
    // read the 512 FFT frequencies into 16 levels
    // music is heard in octaves, but the FFT data
    // is linear, so for the higher octaves, read
    // many FFT bins together.
    level[0] = fft1024.read(0);
    level[1] = fft1024.read(1);
    level[2] = fft1024.read(2, 3);
    // curVal_ = level[2];

    level[3] = fft1024.read(4, 6);
    level[4] = fft1024.read(7, 10);
    level[5] = fft1024.read(11, 15);
    level[6] = fft1024.read(16, 22);
    level[7] = fft1024.read(23, 32);
    level[8] = fft1024.read(33, 46);
    level[9] = fft1024.read(47, 66);
    level[10] = fft1024.read(67, 93);
    level[11] = fft1024.read(94, 131);
    level[12] = fft1024.read(132, 184);
    level[13] = fft1024.read(185, 257);
    level[14] = fft1024.read(258, 359);
    level[15] = fft1024.read(360, 511);
    // See this conversation to change this to more or less than 16 log-scaled bands?
    // https://forum.pjrc.com/threads/32677-Is-there-a-logarithmic-function-for-FFT-bin-selection-for-any-given-of-bands

    for (int i = 0; i < 16; i++)
    {
      curVal_ += level[i];
    }

    curVal_ *= 100;

    // update index
    if (curIndex < N && curIndex >= 0)
    {
      curIndex++;
    }
    else
    {
      curIndex = 0;
    }
    dataCollect[curIndex] = curVal_;

    // get moving mean
    curMean = 0.0f;
    for (int i = 0; i < N; i++)
    {
      curMean += dataCollect[i];
    }
    curMean /= N;

    // update window
    if (indxWindw < WINDOW)
    {
      indxWindw++;
      if (curMean < minWindw)
      {
        minWindw = curMean;
      }
      if (curMean > maxWindw)
      {
        maxWindw = curMean;
      }
    }
    else
    {
      // update new thresholds
      curMinWindow = minWindw;
      curMaxWindow = maxWindw;
      curMeanWindow = (curMaxWindow - curMinWindow) * 0.75 + curMinWindow;

      // reset
      indxWindw = 0;
      minWindw = 1024;
      maxWindw = -1;
    }

    bool isTrigTemp_ = (curMean > curMeanWindow && oldMean < curMeanWindow);

    if (isTrigTemp_)
    {
      if (tempoCollect[Ntempo - 2] != -1)
      {
        isTrigTemp_ = false; // reset
        if (curSec_ - tempoCollect[Ntempo - 2] > 1) // 0.25)
        {
          isTrigTemp_ = true; // reset true after conditions checked
        }
      }
    }

    if (isTrigTemp_)
    {
      tempoCollect[Ntempo - 1] = curSec_;

      // compute tempo mean
      float meanTemp_ = 0.0f;
      int ntemp_ = 0;
      for (int i = 1; i < Ntempo; i++)
      {
        if (tempoCollect[i] != -1)
        {
          meanTemp_ += tempoCollect[i] - tempoCollect[i - 1];
          ntemp_++;

          // shift array
          tempoCollect[i - 1] = tempoCollect[i];
        }
      }

      if (ntemp_ > 0)
      {
        curMeanTempoSec = meanTemp_ / (float)ntemp_;
      }
    }

    // Serial.print(curVal_);
    // Serial.print("\t");
    // Serial.print(curMean);
    // Serial.print("\t");
    // Serial.print(curMinWindow);
    // Serial.print("\t");
    // Serial.print(curMeanWindow);
    // Serial.print("\t");
    // Serial.print(curMaxWindow);
    // Serial.print("\t");

    // if (isTrigTemp_)
    // {
    //   Serial.print(5);
    // }
    // else
    // {
    //   Serial.print(0);
    // }
    
    // Serial.print("\t");
    // Serial.print(curMeanTempoSec);

    // Serial.print("\t");
    if (curSec_ - timerTempo0 > curMeanTempoSec)
    {
      Serial.print(3);
      timerTempo0 = curSec_;

      testIntegral = 0;
    }
    else
    {
      Serial.print(0);
    }

    // Serial.print("\t");
    // Serial.print(maxWindw);
    
    Serial.print("\t");
    if(maxWindw != -1 && curMaxWindow != curMinWindow) {
      
      float val_ = 100 * (maxWindw - curMeanWindow) / (curMaxWindow - curMinWindow);
      if(val_ > 0) {
        Serial.print(val_);
      } else {
        Serial.print(0);
      }
      

      testIntegral += (maxWindw - curMeanWindow) / (curMaxWindow - curMinWindow);
      if(testIntegral < 0) {
        testIntegral = 0;
      }
      if(testIntegral > 100) {
        testIntegral = 0;
      }
      Serial.print("\t");
      Serial.print(100 * pow(testIntegral/100., 0.5));
      Serial.print("\t");
      Serial.print(100);
    }
    else {
      Serial.print(0);
    }
    

    // Display range
    //    Serial.print(0);
    //    Serial.print("\t");
    //    Serial.print(15);
    //    Serial.print("\t");
    Serial.println("");

    oldMean = curMean;
  }
}
