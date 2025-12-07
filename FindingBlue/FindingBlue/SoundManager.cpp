#include "SoundManager.h"
#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <filesystem>


SoundManager* SoundManager::instance = nullptr;
FMOD::System* SoundManager::system = nullptr;
std::map<std::string, FMOD::Sound*> SoundManager::sounds;
std::map<std::string, FMOD::Channel*> SoundManager::channels;


#define FMOD_CHECK(result)                                               \
    do {                                                                 \
        FMOD_RESULT r = (result);                                        \
        if (r != FMOD_OK) {                                              \
            std::cerr << "FMOD error! (" << r << ") "                    \
                      << FMOD_ErrorString(r) << " at "                   \
                      << __FILE__ << ":" << __LINE__ << std::endl;       \
        }                                                                \
    } while(0)

SoundManager::SoundManager() {
    FMOD_CHECK(FMOD::System_Create(&system));
    if (system) {
        std::cout << "FMOD System 생성 완료\n";
    }
}
SoundManager::~SoundManager() {
    for (auto& p : sounds) {
        if (p.second) p.second->release();
    }
    sounds.clear();

    if (system) {
        system->close();
        system->release();
    }
    std::cout << "SoundManager 정리 완료\n";
}

bool SoundManager::Init() {
    if (instance) return true;

    instance = new SoundManager();
    if (!instance || !instance->system) {
        std::cerr << "FMOD System 생성 실패!\n";
        return false;
    }

    FMOD_RESULT init_result = instance->system->init(512, FMOD_INIT_NORMAL, nullptr);
    FMOD_CHECK(init_result);

    if (init_result != FMOD_OK) {
        std::cerr << "FMOD init 실패! (코드: " << init_result << ")\n";
        delete instance;
        instance = nullptr;
        return false;
    }

    std::cout << "SoundManager 완전 초기화 성공!\n";

    // 디버깅 정보 출력
    std::cout << "현재 작업 디렉토리: " << std::filesystem::current_path() << "\n";

    // sound 폴더 존재 확인
    if (std::filesystem::exists("sound")) {
        std::cout << "sound 폴더 존재함\n";
        std::cout << "sound 폴더 내 파일들:\n";
        for (const auto& entry : std::filesystem::directory_iterator("sound")) {
            std::cout << "  - " << entry.path().filename() << "\n";
        }
    }
    else if (std::filesystem::exists("./sound")) {
        std::cout << "./sound 폴더 존재함\n";
    }
    else {
        std::cerr << "sound 폴더가 존재하지 않습니다!\n";
    }

    struct S { std::string name, path; bool loop; };
    std::vector<S> list = {
        {"sin", "sound/sin.wav", false},
        {"bin", "sound/bin.wav", false},
        {"gun", "sound/gun.wav", false},
        {"yong", "sound/yong.wav", false},
        {"kark", "sound/kark.wav", false},
        {"reload", "sound/reload.wav", false},
        {"button", "sound/button.wav", false},
        {"door", "sound/door.wav", false},
        {"swing", "sound/swing.wav", false},
        {"sword", "sound/sword.wav", false},
        {"siren", "sound/siren.wav", false},
        {"reload", "sound/reload.wav", false},
        {"hit", "sound/hit.wav", false},
        {"fire", "sound/fire.wav", false},
        {"gunshot", "sound/gunshot.wav", false},
    };

    for (const auto& s : list) {
        std::cout << "[Sound] 파일 경로 체크: " << s.path << "\n";

        if (!std::filesystem::exists(s.path)) {
            std::cerr << "[Sound] 파일이 존재하지 않음: " << s.path << "\n";
            continue;
        }

        FMOD::Sound* sound = nullptr;

        // FMOD_DEFAULT 사용 (가장 호환성 좋음)
        FMOD_MODE mode = FMOD_DEFAULT;
        if (s.loop) mode |= FMOD_LOOP_NORMAL;

        FMOD_RESULT r = instance->system->createSound(s.path.c_str(), mode, nullptr, &sound);

        // 실패하면 다른 모드들 시도
        if (r != FMOD_OK) {
            std::cout << "[Sound] FMOD_DEFAULT 실패, CREATESTREAM 시도...\n";
            mode = FMOD_CREATESTREAM;
            if (s.loop) mode |= FMOD_LOOP_NORMAL;
            r = instance->system->createSound(s.path.c_str(), mode, nullptr, &sound);
        }

        if (r != FMOD_OK) {
            std::cout << "[Sound] CREATESTREAM 실패, 압축해제 모드 시도...\n";
            mode = FMOD_CREATESAMPLE;
            if (s.loop) mode |= FMOD_LOOP_NORMAL;
            r = instance->system->createSound(s.path.c_str(), mode, nullptr, &sound);
        }

        std::cout << "[Sound] createSound 결과: " << r << " (" << FMOD_ErrorString(r) << ")\n";

        if (r == FMOD_OK && sound) {
            instance->sounds[s.name] = sound;
            std::cout << "[Sound] 로드 완료: " << s.name << "\n";
        }
        else {
            std::cerr << "[Sound] 로드 실패: " << s.name << " (" << s.path << ") - "
                << FMOD_ErrorString(r) << "\n";
        }
    }

    return true;
}
void SoundManager::Update() {
    if (!instance || !instance->system) return;

    instance->system->update();

    // 끝난 채널들을 정리
    auto it = instance->channels.begin();
    while (it != instance->channels.end()) {
        bool isPlaying = false;
        FMOD_RESULT r = it->second->isPlaying(&isPlaying);

        if (r != FMOD_OK || !isPlaying) {
            // 채널이 유효하지 않거나 재생이 끝났으면 제거
            it = instance->channels.erase(it);
        }
        else {
            ++it;
        }
    }
}

void SoundManager::Shutdown() {
    delete instance;
    instance = nullptr;
}
bool SoundManager::IsPlaying(const std::string& name) {
    if (!instance || !instance->system) return false;

    auto it = instance->channels.find(name);
    if (it == instance->channels.end()) {
        return false; // 해당 사운드가 재생된 적이 없음
    }

    FMOD::Channel* channel = it->second;
    if (!channel) return false;

    bool isPlaying = false;
    FMOD_RESULT r = channel->isPlaying(&isPlaying);

    if (r != FMOD_OK) {
        // 채널이 유효하지 않으면 맵에서 제거
        instance->channels.erase(it);
        return false;
    }

    // 재생이 끝났으면 맵에서 제거
    if (!isPlaying) {
        instance->channels.erase(it);
    }

    return isPlaying;
}
void SoundManager::Play(const std::string& name, float volume, bool loop) {
    if (!instance || !instance->system) return;

    auto it = instance->sounds.find(name);
    if (it == instance->sounds.end()) {
        std::cerr << "[Sound] 사운드 없음: " << name << "\n";
        return;
    }

    FMOD::Channel* channel = nullptr;
    FMOD_RESULT r = instance->system->playSound(it->second, nullptr, true, &channel);

    if (r == FMOD_OK && channel) {
        channel->setVolume(volume);
        if (loop) channel->setMode(FMOD_LOOP_NORMAL);
        channel->setPaused(false);

        // 채널을 맵에 저장
        instance->channels[name] = channel;

        std::cout << "[Sound] 재생 시작: " << name << "\n";
    }
    else {
        std::cerr << "[Sound] 재생 실패: " << name << " - " << FMOD_ErrorString(r) << "\n";
    }
}

void SoundManager::StopSound(const std::string& name) {
    if (!instance || !instance->system) return;

    auto it = instance->channels.find(name);
    if (it == instance->channels.end()) {
        std::cout << "[Sound] 재생 중이지 않음: " << name << "\n";
        return;
    }

    FMOD::Channel* channel = it->second;
    if (channel) {
        FMOD_RESULT r = channel->stop();
        if (r == FMOD_OK) {
            std::cout << "[Sound] 정지됨: " << name << "\n";
        }
        else {
            std::cerr << "[Sound] 정지 실패: " << name << " - " << FMOD_ErrorString(r) << "\n";
        }
    }

    // 맵에서 제거
    instance->channels.erase(it);
}


void SoundManager::SetVolume(float volume) {
    if (!instance || !instance->system) return;

    FMOD::ChannelGroup* master = nullptr;
    FMOD_CHECK(instance->system->getMasterChannelGroup(&master));

    if (master) {
        FMOD_CHECK(master->setVolume(volume));
    }
}