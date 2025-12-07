#pragma once
#include <fmod.hpp>
#include <fmod_errors.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>

// FMOD 에러 체크
#define ERRCHECK(fn) { FMOD_RESULT result = fn; if (result != FMOD_OK) { std::cerr << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << " - " << #fn << std::endl; } }

class SoundManager {
private:
    FMOD::System* system;
    std::map<std::string, FMOD::Sound*> sounds;
    bool initialized = false;

public:
    SoundManager() : system(nullptr) {}
    ~SoundManager() { shutdown(); }

    // 1. 초기화 (프로그램 시작 시 호출)
    bool init() {
        ERRCHECK(FMOD::System_Create(&system));
        ERRCHECK(system->init(512, FMOD_INIT_NORMAL, nullptr));  // 512 채널, 기본 설정
        initialized = true;
        std::cout << "SoundManager initialized!" << std::endl;
        return true;
    }

    // 사운드 로드 (프로그램 시작 시 한번 호출)
    void preloadAllSounds() {
        if (!initialized) {
            std::cerr << "SoundManager not initialized! Call init() first." << std::endl;
            return;
        }

		// 사운드 파일 목록 (이름, 경로)
        std::vector<std::pair<std::string, std::string>> soundList = {
            {"안민용", "sounds/안민용.wav"},
            {"김태건", "sounds/김태건.wav"},
            {"양현빈", "sounds/양현빈.wav"},
            {"신동호", "sounds/신동호.wav"},
        };

        for (const auto& s : soundList) {
            preloadSound(s.first, s.second);
        }
        std::cout << "All " << soundList.size() << " sounds preloaded!" << std::endl;
    }
    bool preloadSound(const std::string& name, const std::string& filePath) {
        FMOD::Sound* sound = nullptr;
        FMOD_RESULT result = system->createSound(filePath.c_str(), FMOD_DEFAULT | FMOD_LOOP_OFF, nullptr, &sound);
        if (result == FMOD_OK) {
            sounds[name] = sound;
            std::cout << "Preloaded: " << name << " (" << filePath << ")" << std::endl;
            return true;
        }
        else {
            std::cerr << "Failed to preload " << name << ": " << FMOD_ErrorString(result) << std::endl;
            return false;
        }
    }
    // 사운드 재생
    FMOD::Channel* play(const std::string& name, bool loop = false, float volume = 1.0f, float pitch = 1.0f) {
        auto it = sounds.find(name);
        if (it == sounds.end()) {
            std::cerr << "Sound not found: " << name << std::endl;
            return nullptr;
        }

        FMOD::Sound* sound = it->second;
        FMOD::Channel* channel = nullptr;
        FMOD_MODE mode = loop ? (FMOD_DEFAULT | FMOD_LOOP_NORMAL) : FMOD_DEFAULT;
        ERRCHECK(sound->setMode(mode));
        ERRCHECK(system->playSound(sound, nullptr, false, &channel));

        if (channel) {
            channel->setVolume(volume);
            channel->setPitch(pitch);
            channel->setPaused(false);
        }
        return channel;
    }

    void update() {
        if (initialized) {
            system->update();
        }
    }

    void shutdown() {
        for (auto& pair : sounds) {
            pair.second->release();
        }
        sounds.clear();
        if (system) {
            system->close();
            system->release();
            system = nullptr;
        }
        initialized = false;
        std::cout << "SoundManager shutdown!" << std::endl;
    }

    // 개별 볼륨 조절
    void setVolume(const std::string& name, float volume) {
        auto it = sounds.find(name);
        if (it != sounds.end()) {
            FMOD::ChannelGroup* group = nullptr;
            system->getMasterChannelGroup(&group);
            group->setVolume(volume);
        }
    }

    // 전체 볼륨 조절
    void setMasterVolume(float volume) {
        FMOD::ChannelGroup* group = nullptr;
        system->getMasterChannelGroup(&group);
        group->setVolume(volume);
    }
};