#include "camera_driver/camera.hpp"


using namespace std;
using namespace status_utils;
using namespace cv;


Camera::Camera()
{} // end of "Camera()"


StatusCode Camera::open(int index, VideoCaptureAPIs api_type)
{
    m_capture_device = VideoCapture(index, api_type);
    m_name = "camera_" + index;
    m_index = index;

    Logger::info("Opening camera at index " + index);

    if(is_opened())
    {
        Logger::info("Successfully opened camera!");
        return StatusCode::OK;
    }
    else
    {
        Logger::error("Failed to open camera!");
        return StatusCode::FAILED;
    }

} // end of "open(int, VideoCaptureAPIs)"


StatusCode Camera::open(string path, VideoCaptureAPIs api_type)
{
    m_capture_device = VideoCapture(path, api_type);
    m_name = "camera_" + path;
    m_path = path;

    Logger::info("Opening camera at path " + path);

    if(is_opened())
    {
        Logger::info("Successfully opened camera!");
        return StatusCode::OK;
    }
    else
    {
        Logger::error("Failed to open camera!");
        return StatusCode::FAILED;
    }

} // end of "open(string, VideoCaptureAPIs)"


VideoCapture* Camera::get_device()
{
    return &m_capture_device;

} // end of "get_device()"


bool Camera::is_opened()
{
    return m_capture_device.isOpened();

} // end of "is_opened()"


void Camera::release()
{
    m_capture_device.release();

} // end of "release()"


StatusCode Camera::configure(int width, int height, int fps, int attempts)
{
    return combine_statuses(
        {
            set_property(CAP_PROP_FRAME_WIDTH, width, attempts),
            set_property(CAP_PROP_FRAME_WIDTH, height, attempts),
            set_property(CAP_PROP_FRAME_WIDTH, fps, attempts),
            set_property(CAP_PROP_FRAME_WIDTH, VideoWriter::fourcc('M','J','P','G'), attempts)
        }
    );

} // end of "configure(int, int, int, int)"


double Camera::get_property(VideoCaptureProperties property)
{
    return m_capture_device.get(property);

} // end of "get_property(VideoCaptureProperties)"


StatusCode Camera::set_property(VideoCaptureProperties property, double value, int attempts)
{
    // Try to set the property `attempts` amount of times
    for(int i = 0; i < attempts; i++)
    {
        m_capture_device.set(property, value);

        bool actually_set = get_property(property) == value;

        if(actually_set)
            return StatusCode::OK;
    }

    return StatusCode::FAILED;

} // end of "set_property(VideoCaptureProperties, double, int)"


Mat Camera::get_frame(bool as_gray)
{
    Mat frame;

    m_capture_device >> frame;

    if(as_gray)
        cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

    return frame;

} // end of "get_frame(bool)"