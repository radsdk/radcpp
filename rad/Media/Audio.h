#pragma once

#include <rad/Core/Platform.h>
#include <rad/Core/RefCounted.h>
#include <SDL3/SDL_audio.h>
#include <vector>

namespace rad
{

std::vector<const char*> GetAudioDrivers();
const char* GetCurrentAudioDriver();

class AudioDevice;
std::vector<Ref<AudioDevice>> EnumerateAudioPlaybackDevices();
std::vector<Ref<AudioDevice>> EnumerateAudioRecordingDevices();

class AudioStream;

class AudioDevice : public RefCounted<AudioDevice>
{
public:
    AudioDevice(SDL_AudioDeviceID id);
    ~AudioDevice();

    SDL_AudioDeviceID GetID() const { return m_id; }
    const char* GetName() const { return m_name; }

    bool Open(const SDL_AudioSpec* spec);
    void Pause();
    void Resume();
    bool IsPaused();
    void Close();

    bool BindStreams(SDL_AudioStream** streams, int streamCount);
    void UnbindAudioStreams(SDL_AudioStream** streams, int streamCount);
    bool BindStream(SDL_AudioStream* stream);
    void UnbindAudioStream(SDL_AudioStream* stream);
private:
    SDL_AudioDeviceID m_id;
    const char* m_name = nullptr;
    SDL_AudioSpec m_format = {};
    int m_sampleFrames = 0;
}; // class AudioDevice


class AudioStream : public RefCounted<AudioStream>
{
public:
    static Ref<AudioStream> Create(const SDL_AudioSpec* srcSpec, const SDL_AudioSpec* dstSpec);
    AudioStream(SDL_AudioStream* handle);
    ~AudioStream();

    void Destroy();

    bool GetFormat(SDL_AudioSpec* srcSpec, SDL_AudioSpec* dstSpec);
    bool SetFormat(const SDL_AudioSpec* srcSpec, const SDL_AudioSpec* dstSpec);
    float GetFrequencyRatio();
    bool SetFrequencyRatio(float ratio);
    bool PutData(const void* data, int sizeInBytes);
    bool GetData(void* data, int sizeInBytes);
    int GetDataSizeAvailable();
    int GetDataSizeQueued();
    bool Flush();
    bool Clear();
    bool Pause();
    bool Resume();
    bool Lock();
    bool Unlock();

    bool SetAudioStreamGetCallback(SDL_AudioStreamCallback callback, void* userData);
    bool SetAudioStreamPutCallback(SDL_AudioStreamCallback callback, void* userData);
private:
    SDL_AudioStream* m_handle;
    SDL_PropertiesID m_propID = 0;
}; // class AudioStream

} // namespace rad
