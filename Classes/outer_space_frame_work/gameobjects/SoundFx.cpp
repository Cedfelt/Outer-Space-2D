#include "SoundFx.h"
#include "SimpleAudioEngine.h"

SoundFx::SoundFx(const char* fileName) {
  this->fileName = fileName;
  auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
  audio->preloadEffect(fileName);
  bool loop = false;
  float pitch = 1.0f; 
  float pan = 0.0f;
}

void SoundFx::play(float volume) {
  auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
  sound_id = audio->playEffect(fileName, loop,pitch, pan, volume);
}