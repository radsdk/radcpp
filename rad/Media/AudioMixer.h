#pragma once

#include <rad/Core/Platform.h>
#include <rad/Media/Audio.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <vector>
#include <string_view>

namespace rad
{

class AudioChunk : public RefCounted<AudioChunk>
{
public:
    AudioChunk(Mix_Chunk* chunk);
    ~AudioChunk();

    Mix_Chunk* GetChunk() { return m_chunk; }
    int SetVolume(int volume);
    int GetVolume();

private:
    Mix_Chunk* m_chunk;

}; // class AudioChunk

class Music : public RefCounted<Music>
{
public:
    Music(Mix_Music* music);
    ~Music();

    Mix_Music* GetMusic() { return m_music; }

    Mix_MusicType GetType();
    const char* GetTitle();
    const char* GetTag();
    const char* GetArtistTag();
    const char* GetAlbumTag();
    const char* GetCopyrightTag();

    static int SetVolume(int volume);
    int GetVolume();

    int GetTrackCount();
    bool StartTrack(int track);
    double GetPosition();
    double GetDuration();
    double GetLoopStartTime();
    double GetLoopEndTime();
    double GetLoopLengthTime();

private:
    Mix_Music* m_music;
}; // class Music

class AudioMixer : public RefCounted<AudioMixer>
{
public:
    AudioMixer();
    ~AudioMixer();

    bool Open(SDL_AudioDeviceID deviceID, const SDL_AudioSpec* spec);
    void Close();

    bool IsFormatSupported(int flags) const;

    void Pause();
    void Resume();
    bool QuerySpec(int* frequency, SDL_AudioFormat* format, int* channels);
    int AllocateChannels(int channelCount);
    Ref<AudioChunk> LoadWAV(SDL_IOStream* src, SDL_bool closeio);
    Ref<AudioChunk> LoadWAV(std::string_view fileName);
    Ref<AudioChunk> LoadWAVFromMemory(Uint8* memory);
    Ref<Music> LoadMusic(SDL_IOStream* src, SDL_bool closeio);
    Ref<Music> LoadMusic(std::string_view fileName);
    Ref<Music> LoadMusic(SDL_IOStream* src, Mix_MusicType type, SDL_bool closeio);
    Ref<AudioChunk> LoadRawFromMemory(Uint8* memory, Uint32 sizeInBytes);

    std::vector<const char*> GetChunkDecoders();
    bool HasChunkDecoder(std::string_view name);
    std::vector<const char*> GetMusicDecoders();
    bool HasMusicDecoder(std::string_view name);

    void SetPostMixingCallback(Mix_MixCallback callback, void* arg);
    void HookMusic(Mix_MixCallback callback, void* arg);
    void* GetMusicHookData();
    void HookMusicFinished(Mix_MusicFinishedCallback callback);

    void SetChannelFinishedCallback(Mix_ChannelFinishedCallback callback);
    bool RegisterEffect(int channel, Mix_EffectFunc_t f, Mix_EffectDone_t d, void* arg);
    bool UnregisterEffect(int channel, Mix_EffectFunc_t f);
    bool UnregisterAllEffects(int channel);

    bool SetPanning(int channel, Uint8 left, Uint8 right);
    bool SetPosition(int channel, Sint16 angle, Uint8 distance);
    bool SetDistance(int channel, Uint8 distance);
    bool SetReverseStereo(int channel, bool flip);

    int ReserveChannels(int num);
    bool GroupChannel(int which, int tag);
    bool GroupChannels(int from, int to, int tag);
    int GetGroupChannelAvailable(int tag);
    int GetGroupChannelCount(int tag);
    int GetGroupChannelOldest(int tag);
    int GetGroupChannelNewer(int tag);

    int SetVolume(int channel, int volume);
    int GetVolume(int channel);
    int SetMusicVolume(int volume);
    int GetMusicVolume();
    int SetMasterVolume(int volume);
    int GetMasterVolume();

    bool PlayChannel(int channel, AudioChunk* chunk, int loops);
    bool PlayChannelTimed(int channel, AudioChunk* chunk, int loops, int ticks);
    bool PlayChannelFadeIn(int channel, AudioChunk* chunk, int loops, int ms);
    bool PlayChannelFadeInTimed(int channel, AudioChunk* chunk, int loops, int ms, int ticks);
    Mix_Chunk* GetChunk(int channel);
    bool PlayMusic(Music* music, int loops);
    bool PlayMusicFadeIn(Music* music, int loops, int ms);
    bool PlayMusicFadeInFromPosition(Music* music, int loops, int ms, double position);
    bool HaltChannel(int channel);
    bool HaltGroup(int tag);
    bool HaltMusic();
    bool ExpireChannel(int channel, int ticks);
    bool ExpireAllChannels(int ticks);
    bool FadeOutChannel(int which, int ms);
    bool FadeOutGroup(int tag, int ms);
    bool FadeOutMusic(int ms);
    Mix_Fading GetMusicFading();
    Mix_Fading GetChannelFading(int which);
    void Pause(int channel);
    bool PauseGroup(int tag);
    void Resume(int channel);
    bool ResumeGroup(int tag);
    bool IsChannelPaused(int channel);
    void PauseMusic();
    void ResumeMusic();
    void RewindMusic();
    bool IsMusicPaused();
    bool ModMusicJumpToOrder(int order);

    bool IsChannelPlaying(int channel);
    int GetChannelCountPlaying();
    bool IsMusicPlaying();

    bool SetMusicCommand(std::string_view command);

    bool SetSoundFonts(std::string_view paths);
    const char* GetSoundFonts();
    bool IterateSoundFonts(Mix_EachSoundFontCallback function, void* data);

    bool SetTimidityCfg(std::string_view path);
    const char* GetTimidityCfg();

private:
    // Save initialized modules, with bits of MIX_InitFlags.
    int m_modules = 0;
    bool m_opened = false;

}; // class AudioMixer

} // namespace rad
