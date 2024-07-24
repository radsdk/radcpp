#include <rad/Media/Audio.h>
#include <rad/IO/Logging.h>

namespace rad
{

spdlog::logger* GetMediaLogger();

std::vector<const char*> GetAudioDrivers()
{
    std::vector<const char*> drivers;
    int driverCount = SDL_GetNumAudioDrivers();
    if (driverCount > 0)
    {
        drivers.resize(driverCount);
        for (int i = 0; i < driverCount; ++i)
        {
            drivers[i] = SDL_GetAudioDriver(i);
        }
    }
    return drivers;
}

const char* GetCurrentAudioDriver()
{
    return SDL_GetCurrentAudioDriver();
}

std::vector<Ref<AudioDevice>> EnumerateAudioPlaybackDevices()
{
    std::vector<Ref<AudioDevice>> devices;
    int count = 0;
    const SDL_AudioDeviceID* ids = SDL_GetAudioPlaybackDevices(&count);
    if (count > 0)
    {
        devices.resize(count);
        for (int i = 0; i < count; ++i)
        {
            devices[i] = RAD_NEW AudioDevice(ids[i]);
        }
    }
    return devices;
}

std::vector<Ref<AudioDevice>> EnumerateAudioRecordingDevices()
{
    std::vector<Ref<AudioDevice>> devices;
    int count = 0;
    const SDL_AudioDeviceID* ids = SDL_GetAudioRecordingDevices(&count);
    if (count > 0)
    {
        devices.resize(count);
        for (int i = 0; i < count; ++i)
        {
            devices[i] = RAD_NEW AudioDevice(ids[i]);
        }
    }
    return devices;
}

bool LoadWAV(SDL_IOStream* src, SDL_bool close, SDL_AudioSpec* spec,
    Uint8** buffer, Uint32* sizeInBytes)
{
    int err = SDL_LoadWAV_IO(src, close, spec, buffer, sizeInBytes);
    if (err == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_LoadWAV_IO failed: {}", SDL_GetError());
        return false;
    }
}

bool LoadWAVFromFile(std::string_view path, SDL_AudioSpec* spec,
    Uint8** buffer, Uint32* sizeInBytes)
{
    int err = SDL_LoadWAV(path.data(), spec, buffer, sizeInBytes);
    if (err == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_LoadWAV failed: {}", SDL_GetError());
        return false;
    }
}

bool MixAudioData(Uint8* dst, const Uint8* src, SDL_AudioFormat format, Uint32 sizeInBytes, float volume)
{
    int err = SDL_MixAudio(dst, src, format, sizeInBytes, volume);
    if (err == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_MixAudio failed: {}", SDL_GetError());
        return false;
    }
}

bool ConvertAudioData(const SDL_AudioSpec* srcSpec, const Uint8* srcData, int srcSizeInBytes,
    const SDL_AudioSpec* dstSpec, Uint8** dstData, int* dstSizeInBytes)
{
    int err = SDL_ConvertAudioSamples(
        srcSpec, srcData, srcSizeInBytes, dstSpec, dstData, dstSizeInBytes);
    if (err == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_ConvertAudioSamples failed: {}", SDL_GetError());
        return false;
    }
}

AudioDevice::AudioDevice(SDL_AudioDeviceID id) :
    m_id(id)
{
    m_name = SDL_GetAudioDeviceName(id);
    int err = SDL_GetAudioDeviceFormat(m_id, &m_format, &m_sampleFrames);
    if (err != 0)
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_GetAudioDeviceFormat failed: {}", SDL_GetError());
    }
}

AudioDevice::~AudioDevice()
{
}

bool AudioDevice::Open(const SDL_AudioSpec* spec)
{
    SDL_AudioDeviceID id = SDL_OpenAudioDevice(m_id, spec);
    if (id == m_id)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_OpenAudioDevice failed: {}", SDL_GetError());
        return false;
    }
}

void AudioDevice::Pause()
{
    int err = SDL_PauseAudioDevice(m_id);
    if (err != 0)
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_PauseAudioDevice failed: {}", SDL_GetError());
    }
}

void AudioDevice::Resume()
{
    int err = SDL_ResumeAudioDevice(m_id);
    if (err != 0)
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_ResumeAudioDevice failed: {}", SDL_GetError());
    }
}

bool AudioDevice::IsPaused()
{
    return (SDL_AudioDevicePaused(m_id) == SDL_TRUE);
}

void AudioDevice::Close()
{
    SDL_CloseAudioDevice(m_id);
}

bool AudioDevice::BindStreams(SDL_AudioStream** streams, int streamCount)
{
    int err = SDL_BindAudioStreams(m_id, streams, streamCount);
    if (err == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_BindAudioStreams failed: {}", SDL_GetError());
        return false;
    }
}

void AudioDevice::UnbindAudioStreams(SDL_AudioStream** streams, int streamCount)
{
    SDL_UnbindAudioStreams(streams, streamCount);
}

bool AudioDevice::BindStream(SDL_AudioStream* stream)
{
    int err = SDL_BindAudioStream(m_id, stream);
    if (err == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_BindAudioStream failed: {}", SDL_GetError());
        return false;
    }
}

void AudioDevice::UnbindAudioStream(SDL_AudioStream* stream)
{
    SDL_UnbindAudioStream(stream);
}

bool AudioDevice::SetAudioPostmixCallback(SDL_AudioPostmixCallback callback, void* userData)
{
    int err = SDL_SetAudioPostmixCallback(m_id, callback, userData);
    if (err == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_SetAudioPostmixCallback failed: {}", SDL_GetError());
        return false;
    }
}

Ref<AudioStream> AudioStream::Create(const SDL_AudioSpec* srcSpec, const SDL_AudioSpec* dstSpec)
{
    SDL_AudioStream* handle = SDL_CreateAudioStream(srcSpec, dstSpec);
    if (handle)
    {
        return RAD_NEW AudioStream(handle);
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_CreateAudioStream failed: {}", SDL_GetError());
        return nullptr;
    }
}

Ref<AudioStream> AudioStream::Create(SDL_AudioDeviceID deviceID, const SDL_AudioSpec* spec,
    SDL_AudioStreamCallback callback, void* userData)
{
    SDL_AudioStream* stream = SDL_OpenAudioDeviceStream(deviceID, spec, callback, userData);
    if (stream)
    {
        return RAD_NEW AudioStream(stream);
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_OpenAudioDeviceStream failed: {}", SDL_GetError());
        return nullptr;
    }
}

Ref<AudioStream> AudioStream::CreateDefaultPlayback(const SDL_AudioSpec* spec)
{
    SDL_AudioStream* stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
        spec, nullptr, nullptr);
    if (stream)
    {
        return RAD_NEW AudioStream(stream);
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_OpenAudioDeviceStream failed: {}", SDL_GetError());
        return nullptr;
    }
}

AudioStream::AudioStream(SDL_AudioStream* handle) :
    m_handle(handle)
{
    m_propID = SDL_GetAudioStreamProperties(m_handle);
    if (m_propID == 0)
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_GetAudioStreamProperties failed: {}", SDL_GetError());
    }
}

AudioStream::~AudioStream()
{
    Destroy();
}

void AudioStream::Destroy()
{
    if (m_handle)
    {
        SDL_DestroyAudioStream(m_handle);
        m_handle = nullptr;
    }
}

bool AudioStream::GetFormat(SDL_AudioSpec* srcSpec, SDL_AudioSpec* dstSpec)
{
    return (SDL_GetAudioStreamFormat(m_handle, srcSpec, dstSpec) == 0);
}

bool AudioStream::SetFormat(const SDL_AudioSpec* srcSpec, const SDL_AudioSpec* dstSpec)
{
    return (SDL_SetAudioStreamFormat(m_handle, srcSpec, dstSpec) == 0);
}

float AudioStream::GetFrequencyRatio()
{
    return SDL_GetAudioStreamFrequencyRatio(m_handle);
}

bool AudioStream::SetFrequencyRatio(float ratio)
{
    return (SDL_SetAudioStreamFrequencyRatio(m_handle, ratio) == 0);
}

bool AudioStream::PutData(const void* data, int sizeInBytes)
{
    return (SDL_PutAudioStreamData(m_handle, data, sizeInBytes) == 0);
}

bool AudioStream::GetData(void* data, int sizeInBytes)
{
    return (SDL_GetAudioStreamData(m_handle, data, sizeInBytes));
}

int AudioStream::GetDataSizeAvailable()
{
    return SDL_GetAudioStreamAvailable(m_handle);
}

int AudioStream::GetDataSizeQueued()
{
    return SDL_GetAudioStreamQueued(m_handle);
}

bool AudioStream::Flush()
{
    int err = SDL_FlushAudioStream(m_handle);
    if (err == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_FlushAudioStream failed: {}", SDL_GetError());
        return false;
    }
}

bool AudioStream::Clear()
{
    int err = SDL_ClearAudioStream(m_handle);
    if (err == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_ClearAudioStream failed: {}", SDL_GetError());
        return false;
    }
}

bool AudioStream::Pause()
{
    int err = SDL_PauseAudioStreamDevice(m_handle);
    if (err == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_PauseAudioStreamDevice failed: {}", SDL_GetError());
        return false;
    }
}

bool AudioStream::Resume()
{
    int err = SDL_ResumeAudioStreamDevice(m_handle);
    if (err == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_ResumeAudioStreamDevice failed: {}", SDL_GetError());
        return false;
    }
}

bool AudioStream::Lock()
{
    int err = SDL_LockAudioStream(m_handle);
    if (err == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_LockAudioStream failed: {}", SDL_GetError());
        return false;
    }
}

bool AudioStream::Unlock()
{
    int err = SDL_UnlockAudioStream(m_handle);
    if (err == 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_UnlockAudioStream failed: {}", SDL_GetError());
        return false;
    }
}

bool AudioStream::SetAudioStreamGetCallback(SDL_AudioStreamCallback callback, void* userData)
{
    return (SDL_SetAudioStreamGetCallback(m_handle, callback, userData) == 0);
}

bool AudioStream::SetAudioStreamPutCallback(SDL_AudioStreamCallback callback, void* userData)
{
    return (SDL_SetAudioStreamPutCallback(m_handle, callback, userData) == 0);
}

} // namespace rad
