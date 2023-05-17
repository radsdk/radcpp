#include "Mixer.h"
#include "rad/IO/Logging.h"

namespace sdl
{

Mixer::Mixer()
{
    m_versionCompiled = {};
    m_versionLinked = {};
    SDL_MIXER_VERSION(&m_versionCompiled);
    SDL_memcpy(&m_versionLinked, Mix_Linked_Version(), sizeof(SDL_version));
    LogGlobal(Info, "SDL_mixer version compiled: %d.%d.%d",
        m_versionCompiled.major, m_versionCompiled.minor, m_versionCompiled.patch);
    LogGlobal(Info, "SDL_mixer version linked: %d.%d.%d",
        m_versionLinked.major, m_versionLinked.minor, m_versionLinked.patch);
}

Mixer::~Mixer()
{
    if (m_opened)
    {
        CloseAudio();
    }
}

bool Mixer::OpenAudio(int frequency, Uint16 format, int channels, int chunksize)
{
    int res = Mix_OpenAudio(frequency, format, channels, chunksize);
    if (res == 0)
    {
        LogGlobal(Info, "Mix_OpenAudio sccuess: frequency=%d; format=%d; channels=%d; chunksize=%d",
            frequency, format, channels, chunksize);
        m_opened = true;
        return true;
    }
    else
    {
        LogGlobal(Error, "Mix_OpenAudio failed!");
        return false;
    }
}

bool Mixer::OpenAudioDevice(int frequency, Uint16 format, int channels, int chunksize,
    const char* device, int allowed_changes)
{
    int res = Mix_OpenAudioDevice(frequency, format, channels, chunksize,
        device, allowed_changes);
    if (res == 0)
    {
        LogGlobal(Info, "OpenAudioDevice sccuess: frequency=%d; format=%d; channels=%d; chunksize=%d",
            frequency, format, channels, chunksize);
        return true;
    }
    else
    {
        LogGlobal(Error, "OpenAudioDevice failed: %s", SDL_GetError());
        return false;
    }
}

void Mixer::CloseAudio()
{
    Mix_CloseAudio();
    m_opened = false;
}

bool Mixer::HasMusicDecoder(std::string_view name)
{
    return (Mix_HasMusicDecoder(name.data()) == SDL_TRUE);
}

void Mixer::ReportDecoders()
{
    for (int i = 0; i < GetNumChunkDecoders(); ++i)
    {
        LogGlobal(Info, "SDL_mixer ChunkDecoder #%d: %s",
            i, GetChunkDecoder(i));
    }
    for (int i = 0; i < GetNumMusicDecoders(); ++i)
    {
        LogGlobal(Info, "SDL_mixer MusicDecoder #%d: %s",
            i, GetMusicDecoder(i));
    }
}

int Mixer::QuerySpec(int* freq, Uint16* format, int* channels)
{
    return Mix_QuerySpec(freq, format, channels);
}

int Mixer::GetNumChunkDecoders()
{
    return Mix_GetNumChunkDecoders();
}

const char* Mixer::GetChunkDecoder(int index)
{
    return Mix_GetChunkDecoder(index);
}

int Mixer::GetNumMusicDecoders()
{
    return Mix_GetNumMusicDecoders();
}

const char* Mixer::GetMusicDecoder(int index)
{
    return Mix_GetMusicDecoder(index);
}

rad::Ref<MixChunk> Mixer::LoadWAV(std::string_view fileName)
{
    Mix_Chunk* chunk = Mix_LoadWAV(fileName.data());
    return new MixChunk(chunk);
}

int Mixer::AllocateChannels(int num)
{
    return Mix_AllocateChannels(num);
}

int Mixer::ReserveChannels(int num)
{
    return Mix_ReserveChannels(num);
}

int Mixer::PlayChannel(int channel, MixChunk* chunk, int loops)
{
    int res = Mix_PlayChannel(channel, chunk->GetHandle(), loops);
    if (res == -1)
    {
        LogGlobal(Warn, "Mix_PlayChannel failed: cannot play chunk on channel %d (%s)",
            channel, SDL_GetError());
    }
    return res;
}

int Mixer::HaltChannel(int channel)
{
    int res = Mix_HaltChannel(channel);
    if (res == -1)
    {
        LogGlobal(Warn, "Mix_HaltChannel failed: %s", SDL_GetError());
    }
    return res;
}

int Mixer::PlayChannelFadeIn(int channel, Mix_Chunk* chunk, int loops, int ms)
{
    return Mix_FadeInChannel(channel, chunk, loops, ms);
}

int Mixer::PlayChannelFadeInTimed(int channel, Mix_Chunk* chunk, int loops, int ms, int ticks)
{
    return Mix_FadeInChannelTimed(channel, chunk, loops, ms, ticks);
}

int Mixer::HaltChannelFadeOut(int channel, int ms)
{
    return Mix_FadeOutChannel(channel, ms);
}

int Mixer::GetChannelFadingStatus(int channel)
{
    return Mix_FadingChannel(channel);
}

int Mixer::ExpireChannel(int channel, int ticks)
{
    return Mix_ExpireChannel(channel, ticks);
}

int Mixer::IsChannelPlaying(int channel)
{
    return Mix_Playing(channel);
}

int Mixer::IsChannelPaused(int channel)
{
    return Mix_Paused(channel);
}

void Mixer::Pause(int channel)
{
    Mix_Pause(channel);
}

void Mixer::Resume(int channel)
{
    Mix_Resume(channel);
}

int Mixer::GroupChannel(int channel, int tag)
{
    return Mix_GroupChannel(channel, tag);
}

int Mixer::GroupChannels(int beg, int end, int tag)
{
    return Mix_GroupChannels(beg, end, tag);
}

void Mixer::SetChannelFinishedCallback(void (SDLCALL* channel_finished)(int channel))
{
    Mix_ChannelFinished(channel_finished);
}

rad::Ref<MixMusic> Mixer::LoadMusic(std::string_view fileName)
{
    Mix_Music* music = Mix_LoadMUS(fileName.data());
    return new MixMusic(music);
}

int Mixer::PlayMusic(MixMusic* music, int loops)
{
    int res = Mix_PlayMusic(music->GetHandle(), loops);
    if (res == -1)
    {
        LogGlobal(Warn, "Mix_PlayMusic failed: %s", SDL_GetError());
    }
    return res;
}

int Mixer::HaltMusic()
{
    return Mix_HaltMusic();
}

int Mixer::PlayMusicFadeIn(MixMusic* music, int loops, int ms)
{
    int res = Mix_FadeInMusic(music->GetHandle(), loops, ms);
    if (res == -1)
    {
        LogGlobal(Warn, "Mix_FadeInMusic failed: %s", SDL_GetError());
    }
    return res;
}

int Mixer::PlayMusicFadeIn(MixMusic* music, int loops, int ms, double position)
{
    int res = Mix_FadeInMusicPos(music->GetHandle(), loops, ms, position);
    if (res == -1)
    {
        LogGlobal(Warn, "Mix_FadeInMusicPos failed: %s", SDL_GetError());
    }
    return res;
}

int Mixer::HaltMusicFadeOut(int ms)
{
    return Mix_FadeOutMusic(ms);
}

Mix_Fading Mixer::GetMusicFadingStatus()
{
    return Mix_FadingMusic();
}

void Mixer::PauseMusic()
{
    Mix_PauseMusic();
}

int Mixer::IsMusicPaused()
{
    return Mix_PausedMusic();
}

void Mixer::ResumeMusic()
{
    Mix_ResumeMusic();
}

void Mixer::RewindMusic()
{
    Mix_RewindMusic();
}

int Mixer::SetMusicVolume(int volume)
{
    return Mix_VolumeMusic(volume);
}

int Mixer::GetMusicVolume()
{
    return Mix_VolumeMusic(-1);
}

} // namespace sdl
