#pragma once
#include <string>
#include <map>
#include <fmod.hpp> 

class SoundManager {
private:
    SoundManager();
    ~SoundManager();

   static SoundManager* instance;
    static FMOD::System* system; 
    static std::map<std::string, FMOD::Sound*> sounds; 
    static std::map<std::string, FMOD::Channel*> channels;

public:
    static bool Init();     
    static void Update();   
    static void Shutdown(); 

    static void Play(const std::string& name, float volume = 1.0f, bool loop = false);
    static void StopSound(const std::string& name);
    static bool IsPlaying(const std::string& name);
    static void SetVolume(float volume);            
};