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
        {"신동호", "sound/sin.wav", false},
        {"양현빈", "sound/bin.wav", false},
        {"김태건", "sound/gun.wav", false},
        {"안민용", "sound/yong.wav", false},
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
    if (instance && instance->system)
        instance->system->update();
}
void SoundManager::Shutdown() {
    delete instance;
    instance = nullptr;
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
    
    if (channel) {
        channel->setVolume(volume);
        if (loop) channel->setMode(FMOD_LOOP_NORMAL);

        channel->setPaused(false);

    }
}


// ----------------------------------------------------------------------
// 9. Stop() - 사운드 정지 (기능 제거됨)
// ----------------------------------------------------------------------
void SoundManager::Stop(const std::string& name) {
    // 채널 관리를 하지 않으므로, 이름으로 특정 사운드를 정지시키는 기능은 구현하지 않습니다.
    std::cout << "[SoundManager] Stop 기능은 현재 구현되어 있지 않습니다 (채널 관리 제거됨).\n";
}


// ----------------------------------------------------------------------
// 10. SetVolume() - 마스터 볼륨 설정
// ----------------------------------------------------------------------
void SoundManager::SetVolume(float volume) {
    if (!instance || !instance->system) return;

    FMOD::ChannelGroup* master = nullptr;
    FMOD_CHECK(instance->system->getMasterChannelGroup(&master));

    if (master) {
        FMOD_CHECK(master->setVolume(volume));
    }
}