#pragma once

#include "rad/Core/Common.h"
#include "rad/Core/RefCounted.h"
#include "rad/Core/Span.h"
#include "rad/Core/String.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

namespace sdl
{

// Mix_Chunk RAII wrapper
class MixChunk : public rad::RefCounted<MixChunk>
{
public:
    MixChunk(Mix_Chunk* chunk) :
        m_chunk(chunk) {}
    ~MixChunk()
    {
        if (m_chunk)
        {
            Mix_FreeChunk(m_chunk);
            m_chunk = nullptr;
        }
    }
    Mix_Chunk* GetHandle() { return m_chunk; }
private:
    Mix_Chunk* m_chunk;
}; // class MixChunk

// Mix_Music RAII wrapper
class MixMusic : public rad::RefCounted<MixMusic>
{
public:
    MixMusic(Mix_Music* music) :
        m_music(music) {}
    ~MixMusic()
    {
        if (m_music)
        {
            Mix_FreeMusic(m_music);
            m_music = nullptr;
        }
    }
    Mix_Music* GetHandle() { return m_music; }

    Mix_MusicType GetType() { return Mix_GetMusicType(m_music); }
    const char* GetTitle() { return Mix_GetMusicTitle(m_music); }
    const char* GetTitleTag() { return Mix_GetMusicTitleTag(m_music); }
    const char* GetArtistTag() { return Mix_GetMusicArtistTag(m_music); }
    const char* GetAlbumTag() { return Mix_GetMusicAlbumTag(m_music); }
    const char* GetCopyrightTag() { return Mix_GetMusicCopyrightTag(m_music); }

    // Get a music object's duration, in seconds.
    double GetDuration() { return Mix_MusicDuration(m_music); }
    double GetLoopStartTime() { return Mix_GetMusicLoopStartTime(m_music); }
    double GetLoopEndTime() { return Mix_GetMusicLoopEndTime(m_music); }
    double GetLoopLengthTime() { return Mix_GetMusicLoopLengthTime(m_music); }
    double GetPosition() { return Mix_GetMusicPosition(m_music); }

    int GetVolume() { return Mix_GetMusicVolume(m_music); }

private:
    Mix_Music* m_music;

}; // class MixMusic

class Mixer : public rad::RefCounted<Mixer>
{
public:
    Mixer();
    ~Mixer();

    const SDL_version& GetVersionCompiled() const { return m_versionCompiled; }
    const SDL_version& GetVersionLinked() const { return m_versionLinked; }

    bool OpenAudio(int frequency, Uint16 format, int channels, int chunksize);
    // https://wiki.libsdl.org/SDL2_mixer/Mix_OpenAudioDevice
    bool OpenAudioDevice(int frequency, Uint16 format, int channels, int chunksize,
        const char* device, int allowed_changes);
    void CloseAudio();

    bool HasMusicDecoder(std::string_view name);
    void ReportDecoders();

    // Returns 1 if the audio device has been opened, 0 otherwise.
    int QuerySpec(int* freq, Uint16* format, int* channels);

    int GetNumChunkDecoders();
    const char* GetChunkDecoder(int index);
    int GetNumMusicDecoders();
    const char* GetMusicDecoder(int index);

    rad::Ref<MixChunk> LoadWAV(std::string_view fileName);

    int AllocateChannels(int num);
    int ReserveChannels(int num);
    // If the specified channel is -1, play on the first free channel.
    // Returns which channel was used to play the sound, or -1 if sound could not be played.
    int PlayChannel(int channel, MixChunk* chunk, int loops);
    int HaltChannel(int channel);
    int PlayChannelFadeIn(int channel, Mix_Chunk* chunk, int loops, int ms);
    int PlayChannelFadeInTimed(int channel, Mix_Chunk* chunk, int loops, int ms, int ticks);
    int HaltChannelFadeOut(int channel, int ms);
    int GetChannelFadingStatus(int channel);

    int ExpireChannel(int channel, int ticks);

    int IsChannelPlaying(int channel);
    int IsChannelPaused(int channel);
    // the channel to pause, or -1 to pause all channels.
    void Pause(int channel);
    // the channel to resume, or -1 to resume all paused channels.
    void Resume(int channel);

    int GroupChannel(int channel, int tag);
    int GroupChannels(int beg, int end, int tag);

    void SetChannelFinishedCallback(void (SDLCALL* channel_finished)(int channel));

    rad::Ref<MixMusic> LoadMusic(std::string_view fileName);
    int PlayMusic(MixMusic* music, int loops);
    int HaltMusic();
    // @param ms: the number of milliseconds to spend fading in.
    int PlayMusicFadeIn(MixMusic* music, int loops, int ms);
    // @param ms: the number of milliseconds to spend fading in.
    int PlayMusicFadeIn(MixMusic* music, int loops, int ms, double position);
    // Halt the music stream after fading it out for a specified time.
    // Returns non-zero if music was scheduled to fade, zero otherwise.
    // If no music is currently playing, this returns zero.
    int HaltMusicFadeOut(int ms);
    Mix_Fading GetMusicFadingStatus();
    void PauseMusic();
    // Return 1 if music is paused, 0 otherwise.
    int IsMusicPaused();
    void ResumeMusic();
    void RewindMusic();

    // Set new volume, between 0 and MIX_MAX_VOLUME.
    // Returns the previous volume. If the specified volume is -1, this returns the current volume.
    int SetMusicVolume(int volume);
    int GetMusicVolume();

private:
    bool m_opened = false;
    SDL_version m_versionCompiled = {};
    SDL_version m_versionLinked = {};

}; // class Mixer

} // namespace sdl
