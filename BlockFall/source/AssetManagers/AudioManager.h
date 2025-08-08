#pragma once
#include <string>

class MIX_Mixer;
class MIX_Audio;
class MIX_Track;

class AudioManager
{
public:
	AudioManager() = delete;
	AudioManager(const std::string& filePath);
	~AudioManager();

	/*
	 * Resumes music play
	 */
    void playMusic() const;
	/*
	 * Pauses music
	 */
    void pauseMusic() const;

private:
	MIX_Mixer* _mixer = nullptr;
	MIX_Audio* _bgm = nullptr;
	MIX_Track* _track = nullptr;
};