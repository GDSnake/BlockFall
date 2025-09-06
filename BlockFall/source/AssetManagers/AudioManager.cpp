#include "AudioManager.h"

#include <iostream>

#include "Config/Config.h"

AudioManager::AudioManager(): _mixer(nullptr, MIX_DestroyMixer),
                              //_bgm(nullptr, MIX_DestroyAudio),
                              _track(nullptr, MIX_DestroyTrack)
{   
    const std::string& filePath = std::format("{}{}", Config::getInstance().getAssetsFolder(), Config::getInstance().getConfigData().bgmPath);
    if (filePath.empty())
    {
        std::cerr << "Empty music file path" << '\n';
        return;
    }
    if (!MIX_Init()) {
        std::cerr << "MIX_Init failed: " << SDL_GetError() << '\n';
    }
    SDL_AudioSpec audioSpec = {SDL_AUDIO_S16, 2, 44100};
    _mixer.reset(MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audioSpec));
    if (!_mixer)
    {
        std::cerr << "Audio Manager Init Error: " << SDL_GetError() << '\n';
    }

    //_bgm.reset(MIX_LoadAudio(_mixer.get(), filePath.c_str(), true));
    //if (!_bgm)
    //{
    //    std::cerr << "Audio Manager Init Error: " << SDL_GetError() << '\n';
    //}

    //_track.reset(MIX_CreateTrack(_mixer.get()));
    //MIX_SetTrackAudio(_track.get(), _bgm.get());

    if (!_track)
    {
        SDL_PropertiesID properties;

        MIX_PlayTrack(_track.get(), 0);
    }
}

AudioManager::~AudioManager()
{
	MIX_Quit();
}

void AudioManager::toggleMusic() const
{
    if (MIX_TrackPlaying(_track.get()))
    {
        stopMusic();
    }
    else
    {
        playMusic();
    }
}

void AudioManager::playMusic() const
{
    return;
    if(!MIX_TrackPlaying(_track.get()))
    {
        SDL_PropertiesID properties = SDL_CreateProperties();
        SDL_SetNumberProperty(properties, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
        MIX_PlayTrack(_track.get(), properties);
    }
}

void AudioManager::stopMusic() const
{
    MIX_StopTrack(_track.get(), 0);
}

void AudioManager::resumeMusic() const
{
    if (MIX_TrackPaused(_track.get()))
    {
        MIX_ResumeTrack(_track.get());
    }
    else
    {
        playMusic();
    }
}

void AudioManager::pauseMusic() const
{
    MIX_PauseTrack(_track.get());
}
