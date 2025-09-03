#pragma once
#include <memory>
#include <SDL3/SDL_mixer.h>

class AudioManager
{
public:
    static AudioManager& getInstance()
    {
        static AudioManager instance;
        return instance;
    }

    void playMusic() const;
    void pauseMusic() const;

private:
    AudioManager();

    ~AudioManager();
	std::unique_ptr<MIX_Mixer, decltype(&MIX_DestroyMixer)> _mixer;
	std::unique_ptr<MIX_Audio, decltype(& MIX_DestroyAudio)> _bgm;
	std::unique_ptr <MIX_Track, decltype(&MIX_DestroyTrack)> _track;
};