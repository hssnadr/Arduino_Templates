#include <Audio.h>

#define RANGEFFT 40 // ??
const int myLineInput = AUDIO_INPUT_LINEIN;          // define line-in as audio input
AudioInputI2S          lineInput;                    // define audio input for line-in (or mic but here it's line-in)
AudioAnalyzeFFT1024    myFFT;                        // define FFT function
AudioConnection patchCord1(lineInput, 0, myFFT, 0);  // connect audio input to the FFT function (if I'm correct)
AudioControlSGTL5000 audioShield;                    // define audio shield

const int lowerFFTBins[] = {0,1,2,3,4,5,6,8,10,12,15,18,22,27,32,38,45,53,63,74,87,102,119,138,160,186,216,250,289,334,385,444};
const int upperFFTBins[] = {0,1,2,3,4,5,7,9,11,14,17,21,26,31,37,44,52,62,73,86,101,118,137,159,185,215,249,288,333,384,443,511};

void setup() {
  Serial.begin(38400);
  
  //--------------------------------------------------------
  //                 AUDIO SHIELD SETUP
  //--------------------------------------------------------
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(12);

  // Enable the audio shield and set the output volume.
  audioShield.enable();
  audioShield.inputSelect(myLineInput);
  audioShield.volume(0.6);

  // Configure the window algorithm to use
  myFFT.windowFunction(AudioWindowHanning1024);
}

void loop() {
  Serial.print(0);
  Serial.print("\t");
  Serial.print(100);
  Serial.print("\t");
  
//  for(int x=0; x < 32; x++){
//      float level_ = myFFT.read(lowerFFTBins[x],upperFFTBins[x]);
//      Serial.print(100 * level_);
//      Serial.print("\t");
//  }
  float level_ = myFFT.read(30,100);
  Serial.print(100 * level_);
  Serial.print("\t");

  level_ = myFFT.read(100,300);
  Serial.print(100 * level_);
  Serial.print("\t");

  level_ = myFFT.read(300,500);
  Serial.print(100 * level_);
  Serial.print("\t");
  
  Serial.println("");
}

//int getAudioModulation() {
//  float fft_ = getAudioInFFT(minRANGEFFT, maxRANGEFFT);
//  return fft_ ;
//}

//--------------------------------------------------------
//                          FFT
//--------------------------------------------------------
float getAudioInFFT(float minFFT_, float maxFFT_) {
  float fft_ = 0.0;
  int cntFFT_ = 0;

  // reverse born if necessary
  if (minFFT_ > maxFFT_) {
    float f_ = minFFT_;
    minFFT_ = maxFFT_ ;
    maxFFT_ = f_ ;
  }

  for (int i = minFFT_; i < maxFFT_; i++) {
    float curFFT_ = myFFT.read(i);
    if (curFFT_ >= 0.01) {
      fft_ += curFFT_;
      cntFFT_++;
    }
  }
  fft_ /= float(cntFFT_) ; // calculate mean val

  return fft_;
}
