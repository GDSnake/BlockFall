#include "AudioManager.h"
//#include <SDL3/SDL_mixer.h>

#include <iostream>

AudioManager::AudioManager(const std::string& filePath)
{

	//if (filePath.empty())
	//{
	//	std::cerr << "Empty music file path" << std::endl;
	//	return;
	//}

	//const SDL_AudioSpec* audioSpec = new SDL_AudioSpec(SDL_AUDIO_S16, -1, 44100);

	//if (!MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK ,audioSpec)) {
	//	std::cerr << "Audio Manager Init Error: " << SDL_GetError() << std::endl;
	//}

	//_bgm = MIX_LoadAudio(_mixer, filePath.c_str(), true);
	//if (!_bgm)
	//{
	//	std::cerr << "Audio Manager Init Error: " << SDL_GetError() << std::endl;
	//}

	//_track = MIX_CreateTrack(_mixer);
	//MIX_SetTrackAudio(_track, _bgm);

	//if (!_track) {
	//	//SDL_PropertiesID properties;
	//	
	//	MIX_PlayTrack(_track, 0);
	//}
}

AudioManager::~AudioManager()
{
	//MIX_DestroyAudio(_bgm);
	//MIX_DestroyMixer(_mixer);
	//
	//MIX_Quit();
}

void AudioManager::playMusic() const
{
	//MIX_PlayTrack(_track, 0);
}

void AudioManager::pauseMusic() const
{
    //MIX_PauseAllTracks(_mixer);
}
