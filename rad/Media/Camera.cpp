#include <rad/Core/MemoryDebug.h>
#include <rad/Media/Camera.h>
#include <rad/IO/Logging.h>

namespace rad
{

std::vector<const char*> EnumerateCameraDrivers()
{
    std::vector<const char*> drivers;
    int count = SDL_GetNumCameraDrivers();
    for (int i = 0; i < count; ++i)
    {
        const char* driver = SDL_GetCameraDriver(i);
        drivers.push_back(driver);
    }
    return drivers;
}

const char* GetCurrentCameraDriver()
{
    return SDL_GetCurrentCameraDriver();
}

std::vector<rad::Ref<Camera>> EnumerateCameras()
{
    std::vector<rad::Ref<Camera>> cameras;
    int count = 0;
    SDL_CameraDeviceID* ids = SDL_GetCameraDevices(&count);
    if (count > 0)
    {
        cameras.resize(count);
        for (int i = 0; i < count; ++i)
        {
            cameras[i] = RAD_NEW Camera(ids[i]);
        }
    }
    return cameras;
}

Camera::Camera(SDL_CameraDeviceID id) :
    m_id(id)
{
    m_name = SDL_GetCameraDeviceName(id);
    m_formats = SDL_GetCameraDeviceSupportedFormats(id, &m_formatCount);
    m_position = SDL_GetCameraDevicePosition(id);
}

Camera::~Camera()
{
}

bool Camera::Open(const SDL_CameraSpec* spec)
{
    m_handle = SDL_OpenCameraDevice(m_id, spec);
    if (m_handle)
    {
        RAD_LOG(GetMediaLogger(), info, "Camera {} opened successfully.", m_name);
        m_propID = SDL_GetCameraProperties(m_handle);
        if (m_propID == 0)
        {
            RAD_LOG(GetMediaLogger(), err, "SDL_GetCameraProperties failed: {}", SDL_GetError());
        }
        return true;
    }
    else
    {
        RAD_LOG(GetMediaLogger(), err, "SDL_OpenCameraDevice failed: {}", SDL_GetError());
        return false;
    }
}

void Camera::Close()
{
    SDL_CloseCamera(m_handle);
}

Camera::Permission Camera::GetPermission()
{
    assert(m_handle != nullptr); // must has been opened.
    int permission = SDL_GetCameraPermissionState(m_handle);
    return static_cast<Permission>(permission);
}

int Camera::GetFormat(SDL_CameraSpec* spec)
{
    return SDL_GetCameraFormat(m_handle, spec);
}

SDL_Surface* Camera::AquireFrame(Uint64* timestamp)
{
    return SDL_AcquireCameraFrame(m_handle, timestamp);
}

int Camera::ReleaseFrame(SDL_Surface* surface)
{
    return SDL_ReleaseCameraFrame(m_handle, surface);
}

} // namespace rad
