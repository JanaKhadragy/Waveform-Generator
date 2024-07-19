#include <Arduino.h>
#include <math.h>

const int squareWavePin = 2;   
const int dacPin = 25;         
const int freq = 1000;         

unsigned long lastUpdateTime = 0;
int samplingInterval = 1000000 / freq; 

const int sineWaveSamples = 256;
uint8_t sineWaveTable[sineWaveSamples];

void generateSineWaveTable() {
  for (int i = 0; i < sineWaveSamples; i++) {
    sineWaveTable[i] = (uint8_t)(127.5 * (1 + sin(2 * PI * i / sineWaveSamples)));
  }
}

void setup() {
  pinMode(squareWavePin, OUTPUT);
  pinMode(dacPin, OUTPUT);
  generateSineWaveTable();
  Serial.begin(115200);
  while (!Serial); 
  Serial.println("Waveform Generator Initialized");
}

void loop() {
  static int dacValue = 0;
  static int direction = 1;
  static int sineIndex = 0;

  unsigned long currentTime = micros();

  if (currentTime - lastUpdateTime >= samplingInterval) {
    lastUpdateTime = currentTime;

    // Generate square wave
    digitalWrite(squareWavePin, !digitalRead(squareWavePin));
    int squareWaveValue = digitalRead(squareWavePin) * 255; 
    Serial.println(squareWaveValue);

    // Generate triangle wave
    dacValue += direction;
    if (dacValue >= 255 || dacValue <= 0) {
      direction = -direction; 
    }
    dacWrite(dacPin, dacValue);
    Serial.println(dacValue);

    // Generate sine wave
    int sineWaveValue = sineWaveTable[sineIndex];
    dacWrite(dacPin, sineWaveValue);
    sineIndex = (sineIndex + 1) % sineWaveSamples;
    Serial.println(sineWaveValue);

    Serial.println(); 
  }
}