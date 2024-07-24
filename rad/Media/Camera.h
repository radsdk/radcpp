#pragma once

#include <rad/Core/Platform.h>
#include <rad/Core/RefCounted.h>
#include <rad/Container/Span.h>
#include <SDL3/SDL_camera.h>

namespace rad
{

std::vector<const char*> EnumerateCameraDrivers();
const char* GetCurrentCameraDriver();

class Camera;
std::vector<Ref<Camera>> EnumerateCameras();

// Simple wrapper for SDL_camera.
class Camera : public RefCounted<Camera>
{
public:
    Camera(SDL_CameraID id);
    ~Camera();

    const char* GetName() const { return m_name; }
    Span<const SDL_CameraSpec*> GetSupportedFormats() const
    {
        return Span{ m_formats, static_cast<size_t>(m_formatCount) };
    }
    SDL_CameraPosition GetPosition() const { return m_position; }

    bool Open(const SDL_CameraSpec* spec);
    void Close();

    enum class Permission : int
    {
        WaitForResponse = 0,
        Denied = -1,
        Approved = 1,
    };
    Permission GetPermission();
    int GetFormat(SDL_CameraSpec* spec);

    // Do not call SDL_FreeSurface() on the returned surface!
    SDL_Surface* AquireFrame(Uint64* timestamp);
    int ReleaseFrame(SDL_Surface* surface);

private:
    SDL_CameraID m_id = 0;
    const char* m_name = nullptr;
    const SDL_CameraSpec* const* m_formats;
    int m_formatCount = 0;
    SDL_CameraPosition m_position;
    // The opaque structure used to identify an opened SDL camera.
    SDL_Camera* m_handle = nullptr;
    SDL_PropertiesID m_propID = 0;
}; // class Camera

} // namespace rad
