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

public:
    static bool Init();     
    static void Update();   
    static void Shutdown(); 

    static void Play(const std::string& name, float volume = 1.0f, bool loop = false);
    static void Stop(const std::string& name);      
    static void SetVolume(float volume);            
};