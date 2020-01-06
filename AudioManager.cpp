#include "AudioManager.h"

AudioManager::AudioManager()
{

}



void AudioManager::initSound()
{
	result = System_Create(&fmodSystem);
	result = fmodSystem->init(NUM_CHANNELS, FMOD_INIT_NORMAL, 0);
}

void AudioManager::update()
{
	result = fmodSystem->update();
}

void AudioManager::load()
{
	//  Load sound into memory

	result = fmodSystem->createSound("pokeball.mp3", FMOD_DEFAULT, 0, &sound1);
	sound1->setMode(FMOD_LOOP_OFF);

	result = fmodSystem->createSound("pokemon.mp3", FMOD_DEFAULT, 0, &music1);
	music1->setMode(FMOD_LOOP_NORMAL);

}

void AudioManager::playPowerUpEffect()
{
	result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sound1, false, &channel);
	channel->setPan(0.0);
	channel->setVolume(0.2);
	channel->setPaused(false);
}

void AudioManager::playSoundTrack()
{
	result = fmodSystem->playSound(FMOD_CHANNEL_FREE, music1, false, &channel);
	channel->setPan(0.0);
	channel->setVolume(2);
	channel->setPaused(false);
}


AudioManager::~AudioManager()
{
}