#include <rad/Media/AudioMixer.h>
#include <rad/Core/Integer.h>
#include <rad/IO/Logging.h>

namespace rad
{

spdlog::logger* GetMediaLogger();

AudioChunk::AudioChunk(Mix_Chunk* chunk) :
    m_chunk(chunk)
{
}

AudioChunk::~AudioChunk()
{
    if (m_chunk)
    {
        Mix_FreeChunk(m_chunk);
        m_chunk = nullptr;
    }
}

int AudioChunk::SetVolume(int volume)
{
    return Mix_VolumeChunk(m_chunk, volume);
}

int AudioChunk::GetVolume()
{
    return Mix_VolumeChunk(m_chunk, -1);
}

Music::Music(Mix_Music* music) :
    m_music(music)
{
}

Music::~Music()
{
    if (m_music)
    {
        Mix_FreeMusic(m_music);
        m_music = nullptr;
    }
}

Mix_MusicType Music::GetType()
{
    return Mix_GetMusicType(m_music);
}

const char* Music::GetTitle()
{
    return Mix_GetMusicTitle(m_music);
}

const char* Music::GetTag()
{
    return Mix_GetMusicTitleTag(m_music);
}

const char* Music::GetArtistTag()
{
    return Mix_GetMusicArtistTag(m_music);
}

const char* Music::GetAlbumTag()
{
    return Mix_GetMusicAlbumTag(m_music);
}

const char* Music::GetCopyrightTag()
{
    return Mix_GetMusicCopyrightTag(m_music);
}

int Music::SetVolume(int volume)
{
    return Mix_VolumeMusic(volume);
}

int Music::GetVolume()
{
    return Mix_GetMusicVolume(m_music);
}

int Music::GetTrackCount()
{
    return Mix_GetNumTracks(m_music);
}

bool Music::StartTrack(int track)
{
    int result = Mix_StartTrack(m_music, track);
    return (result == 0);
}

double Music::GetPosition()
{
    return Mix_GetMusicPosition(m_music);
}

double Music::GetDuration()
{
    return Mix_MusicDuration(m_music);
}

double Music::GetLoopStartTime()
{
    return Mix_GetMusicLoopStartTime(m_music);
}

double Music::GetLoopEndTime()
{
    return Mix_GetMusicLoopEndTime(m_music);
}

double Music::GetLoopLengthTime()
{
    return Mix_GetMusicLoopLengthTime(m_music);
}

AudioMixer::AudioMixer()
{
    m_modules = Mix_Init(
        MIX_INIT_FLAC | MIX_INIT_MOD |
        MIX_INIT_MP3 | MIX_INIT_OGG |
        MIX_INIT_MID | MIX_INIT_OPUS | MIX_INIT_WAVPACK);

    std::string moduleNames;
    if (HasBits<uint32_t>(m_modules, MIX_INIT_FLAC))
    {
        moduleNames += "FLAC,";
    }
    if (HasBits<uint32_t>(m_modules, MIX_INIT_MOD))
    {
        moduleNames += "MOD,";
    }
    if (HasBits<uint32_t>(m_modules, MIX_INIT_MP3))
    {
        moduleNames += "MP3,";
    }
    if (HasBits<uint32_t>(m_modules, MIX_INIT_OGG))
    {
        moduleNames += "OGG,";
    }
    if (HasBits<uint32_t>(m_modules, MIX_INIT_MID))
    {
        moduleNames += "MID,";
    }
    if (HasBits<uint32_t>(m_modules, MIX_INIT_OPUS))
    {
        moduleNames += "OPUS,";
    }
    if (HasBits<uint32_t>(m_modules, MIX_INIT_WAVPACK))
    {
        moduleNames += "WAVEPACK,";
    }

    if ((m_modules != 0) && !moduleNames.empty())
    {
        moduleNames.pop_back();
        RAD_LOG(GetMediaLogger(), info, "Initialized audio modules: {}", moduleNames);
    }
}

AudioMixer::~AudioMixer()
{
    if (m_opened)
    {
        Close();
    }
    Mix_Quit();
}

bool AudioMixer::Open(SDL_AudioDeviceID deviceID, const SDL_AudioSpec* spec)
{
    int result = Mix_OpenAudio(deviceID, spec);
    if (result == 0)
    {
        m_opened = true;
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "Mix_OpenAudio failed: {}", SDL_GetError());
        return false;
    }
}

void AudioMixer::Close()
{
    assert(m_opened);
    Mix_CloseAudio();
    m_opened = false;
}

bool AudioMixer::IsFormatSupported(int flags) const
{
    return HasBits<uint32_t>(m_modules, flags);
}

void AudioMixer::Pause()
{
    assert(m_opened);
    Mix_PauseAudio(1);
}

void AudioMixer::Resume()
{
    assert(m_opened);
    Mix_PauseAudio(0);
}

bool AudioMixer::QuerySpec(int* frequency, SDL_AudioFormat* format, int* channels)
{
    assert(m_opened);
    return Mix_QuerySpec(frequency, format, channels);
}

int AudioMixer::AllocateChannels(int channelCount)
{
    return Mix_AllocateChannels(channelCount);
}

Ref<AudioChunk> AudioMixer::LoadWAV(SDL_IOStream* src, SDL_bool closeio)
{
    Mix_Chunk* chunk = Mix_LoadWAV_IO(src, closeio);
    if (chunk)
    {
        return RAD_NEW AudioChunk(chunk);
    }
    else
    {
        return nullptr;
    }
}

Ref<AudioChunk> AudioMixer::LoadWAV(std::string_view fileName)
{
    Mix_Chunk* chunk = Mix_LoadWAV(fileName.data());
    if (chunk)
    {
        return RAD_NEW AudioChunk(chunk);
    }
    else
    {
        return nullptr;
    }
}

Ref<AudioChunk> AudioMixer::LoadWAVFromMemory(Uint8* memory)
{
    Mix_Chunk* chunk = Mix_QuickLoad_WAV(memory);
    if (chunk)
    {
        return RAD_NEW AudioChunk(chunk);
    }
    else
    {
        return nullptr;
    }
}

Ref<Music> AudioMixer::LoadMusic(SDL_IOStream* src, SDL_bool closeio)
{
    Mix_Music* music = Mix_LoadMUS_IO(src, closeio);
    if (music)
    {
        return RAD_NEW Music(music);
    }
    else
    {
        return nullptr;
    }
}

Ref<Music> AudioMixer::LoadMusic(std::string_view fileName)
{
    Mix_Music* music = Mix_LoadMUS(fileName.data());
    if (music)
    {
        return RAD_NEW Music(music);
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "Mix_LoadMUS(\"{}\") failed: {}",
            fileName, Mix_GetError());
        return nullptr;
    }
}

Ref<Music> AudioMixer::LoadMusic(SDL_IOStream* src, Mix_MusicType type, SDL_bool closeio)
{
    Mix_Music* music = Mix_LoadMUSType_IO(src, type, closeio);
    if (music)
    {
        return RAD_NEW Music(music);
    }
    else
    {
        return nullptr;
    }
}

Ref<AudioChunk> AudioMixer::LoadRawFromMemory(Uint8* memory, Uint32 sizeInBytes)
{
    Mix_Chunk* chunk = Mix_QuickLoad_RAW(memory, sizeInBytes);
    if (chunk)
    {
        return RAD_NEW AudioChunk(chunk);
    }
    else
    {
        return nullptr;
    }
}

std::vector<const char*> AudioMixer::GetChunkDecoders()
{
    std::vector<const char*> decoders;
    int count = Mix_GetNumChunkDecoders();
    if (count > 0)
    {
        decoders.resize(count);
        for (int i = 0; i < count; ++i)
        {
            decoders[i] = Mix_GetChunkDecoder(i);
        }
    }
    return decoders;
}

bool AudioMixer::HasChunkDecoder(std::string_view name)
{
    return (Mix_HasChunkDecoder(name.data()) == SDL_TRUE);
}

std::vector<const char*> AudioMixer::GetMusicDecoders()
{
    std::vector<const char*> decoders;
    int count = Mix_GetNumMusicDecoders();
    if (count > 0)
    {
        decoders.resize(count);
        for (int i = 0; i < count; ++i)
        {
            decoders[i] = Mix_GetMusicDecoder(i);
        }
    }
    return decoders;
}

bool AudioMixer::HasMusicDecoder(std::string_view name)
{
    return (Mix_HasMusicDecoder(name.data()) == SDL_TRUE);
}

void AudioMixer::SetPostMixingCallback(Mix_MixCallback callback, void* arg)
{
    Mix_SetPostMix(callback, arg);
}

void AudioMixer::HookMusic(Mix_MixCallback callback, void* arg)
{
    Mix_HookMusic(callback, arg);
}

void* AudioMixer::GetMusicHookData()
{
    return Mix_GetMusicHookData();
}

void AudioMixer::HookMusicFinished(Mix_MusicFinishedCallback callback)
{
    Mix_HookMusicFinished(callback);
}

void AudioMixer::SetChannelFinishedCallback(Mix_ChannelFinishedCallback callback)
{
    Mix_ChannelFinished(callback);
}

bool AudioMixer::RegisterEffect(int channel, Mix_EffectFunc_t f, Mix_EffectDone_t d, void* arg)
{
    int result = Mix_RegisterEffect(channel, f, d, arg);
    if (result != 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "Mix_RegisterEffect failed: {}", Mix_GetError());
        return false;
    }
}

bool AudioMixer::UnregisterEffect(int channel, Mix_EffectFunc_t f)
{
    int result = Mix_UnregisterEffect(channel, f);
    if (result != 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "Mix_UnregisterEffect failed: {}", Mix_GetError());
        return false;
    }
}

bool AudioMixer::UnregisterAllEffects(int channel)
{
    int result = Mix_UnregisterAllEffects(channel);
    if (result != 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "Mix_UnregisterAllEffects failed: {}", Mix_GetError());
        return false;
    }
}

bool AudioMixer::SetPanning(int channel, Uint8 left, Uint8 right)
{
    return (Mix_SetPanning(channel, left, right) != 0);
}

bool AudioMixer::SetPosition(int channel, Sint16 angle, Uint8 distance)
{
    int result = Mix_SetPosition(channel, angle, distance);
    if (result != 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "Mix_SetPosition failed: {}", Mix_GetError());
        return false;
    }
}

bool AudioMixer::SetDistance(int channel, Uint8 distance)
{
    int result = Mix_SetDistance(channel, distance);
    if (result != 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "Mix_SetDistance failed: {}", Mix_GetError());
        return false;
    }
}

bool AudioMixer::SetReverseStereo(int channel, bool flip)
{
    int result = Mix_SetReverseStereo(channel, flip);
    if (result != 0)
    {
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "Mix_SetReverseStereo failed: {}", Mix_GetError());
        return false;
    }
}

int AudioMixer::ReserveChannels(int num)
{
    return Mix_ReserveChannels(num);
}

bool AudioMixer::GroupChannel(int which, int tag)
{
    return (Mix_GroupChannel(which, tag) != 0);
}

bool AudioMixer::GroupChannels(int from, int to, int tag)
{
    return (Mix_GroupChannels(from, to, tag) == 0);
}

int AudioMixer::GetGroupChannelAvailable(int tag)
{
    return Mix_GroupAvailable(tag);
}

int AudioMixer::GetGroupChannelCount(int tag)
{
    return Mix_GroupCount(tag);
}

int AudioMixer::GetGroupChannelOldest(int tag)
{
    return Mix_GroupOldest(tag);
}

int AudioMixer::GetGroupChannelNewer(int tag)
{
    return Mix_GroupNewer(tag);
}

int AudioMixer::SetVolume(int channel, int volume)
{
    return Mix_Volume(channel, volume);
}

int AudioMixer::GetVolume(int channel)
{
    return Mix_Volume(channel, -1);
}

int AudioMixer::SetMusicVolume(int volume)
{
    return Mix_VolumeMusic(volume);
}

int AudioMixer::GetMusicVolume()
{
    return Mix_VolumeMusic(-1);
}

int AudioMixer::SetMasterVolume(int volume)
{
    return Mix_MasterVolume(volume);
}

int AudioMixer::GetMasterVolume()
{
    return Mix_MasterVolume(-1);
}

bool AudioMixer::PlayChannel(int channel, AudioChunk* chunk, int loops)
{
    int result = Mix_PlayChannel(channel, chunk->GetChunk(), loops);
    return (result != -1);
}

bool AudioMixer::PlayChannelTimed(int channel, AudioChunk* chunk, int loops, int ticks)
{
    int result = Mix_PlayChannelTimed(channel, chunk->GetChunk(), loops, ticks);
    return (result != -1);
}

bool AudioMixer::PlayChannelFadeIn(int channel, AudioChunk* chunk, int loops, int ms)
{
    int result = Mix_FadeInChannel(channel, chunk->GetChunk(), loops, ms);
    return (result != -1);
}

bool AudioMixer::PlayChannelFadeInTimed(int channel, AudioChunk* chunk, int loops, int ms, int ticks)
{
    int result = Mix_FadeInChannelTimed(channel, chunk->GetChunk(), loops, ms, ticks);
    return (result != -1);
}

Mix_Chunk* AudioMixer::GetChunk(int channel)
{
    return Mix_GetChunk(channel);
}

bool AudioMixer::PlayMusic(Music* music, int loops)
{
    int result = Mix_PlayMusic(music->GetMusic(), loops);
    return (result == 0);
}

bool AudioMixer::PlayMusicFadeIn(Music* music, int loops, int ms)
{
    int result = Mix_FadeInMusic(music->GetMusic(), loops, ms);
    return (result == 0);
}

bool AudioMixer::PlayMusicFadeInFromPosition(Music* music, int loops, int ms, double position)
{
    int result = Mix_FadeInMusicPos(music->GetMusic(), loops, ms, position);
    return (result == 0);
}

bool AudioMixer::HaltChannel(int channel)
{
    int result = Mix_HaltChannel(channel);
    return (result == 0);
}

bool AudioMixer::HaltGroup(int tag)
{
    int result = Mix_HaltGroup(tag);
    return (result == 0);
}

bool AudioMixer::HaltMusic()
{
    int result = Mix_HaltMusic();
    return (result == 0);
}

bool AudioMixer::ExpireChannel(int channel, int ticks)
{
    int result = Mix_ExpireChannel(channel, ticks);
    if (channel >= 0)
    {
        return (result == channel);
    }
    else
    {
        return (result > 0);
    }
}

bool AudioMixer::ExpireAllChannels(int ticks)
{
    int result = Mix_ExpireChannel(-1, ticks);
    return (result > 0);
}

bool AudioMixer::FadeOutChannel(int which, int ms)
{
    int result = Mix_FadeOutChannel(which, ms);
    return (result > 0);
}

bool AudioMixer::FadeOutGroup(int tag, int ms)
{
    int result = Mix_FadeOutGroup(tag, ms);
    return (result > 0);
}

bool AudioMixer::FadeOutMusic(int ms)
{
    int result = Mix_FadeOutMusic(ms);
    return (result != 0);
}

Mix_Fading AudioMixer::GetMusicFading()
{
    return Mix_FadingMusic();
}

Mix_Fading AudioMixer::GetChannelFading(int which)
{
    return Mix_FadingChannel(which);
}

void AudioMixer::Pause(int channel)
{
    Mix_Pause(channel);
}

bool AudioMixer::PauseGroup(int tag)
{
    int result = Mix_PauseGroup(tag);
    return (result == 0);
}

void AudioMixer::Resume(int channel)
{
    Mix_Resume(channel);
}

bool AudioMixer::ResumeGroup(int tag)
{
    int result = Mix_ResumeGroup(tag);
    return (result == 0);
}

bool AudioMixer::IsChannelPaused(int channel)
{
    int result = Mix_Paused(channel);
    return (result == 1);
}

void AudioMixer::PauseMusic()
{
    Mix_PauseMusic();
}

void AudioMixer::ResumeMusic()
{
    Mix_ResumeMusic();
}

void AudioMixer::RewindMusic()
{
    Mix_RewindMusic();
}

bool AudioMixer::IsMusicPaused()
{
    return (Mix_PausedMusic() == 1);
}

bool AudioMixer::ModMusicJumpToOrder(int order)
{
    int result = Mix_ModMusicJumpToOrder(order);
    return (result == 0);
}

bool AudioMixer::IsChannelPlaying(int channel)
{
    int result = Mix_Playing(channel);
    return (result != 0);
}

int AudioMixer::GetChannelCountPlaying()
{
    return Mix_Playing(-1);
}

bool AudioMixer::IsMusicPlaying()
{
    int result = Mix_PlayingMusic();
    return (result != 0);
}

bool AudioMixer::SetMusicCommand(std::string_view command)
{
    int result = Mix_SetMusicCMD(command.data());
    return (result == 0);
}

bool AudioMixer::SetSoundFonts(std::string_view paths)
{
    int result = Mix_SetSoundFonts(paths.data());
    return (result == 0);
}

const char* AudioMixer::GetSoundFonts()
{
    return Mix_GetSoundFonts();
}

bool AudioMixer::IterateSoundFonts(Mix_EachSoundFontCallback function, void* data)
{
    int result = Mix_EachSoundFont(function, data);
    return (result != 0);
}

bool AudioMixer::SetTimidityCfg(std::string_view path)
{
    int result = Mix_SetTimidityCfg(path.data());
    return (result == 1);
}

const char* AudioMixer::GetTimidityCfg()
{
    return Mix_GetTimidityCfg();
}

} // namespace rad
