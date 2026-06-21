#ifndef CAMERA_HPP
#define CAMERA_HPP


#include <iostream>
#include <ctime>
#include <cmath>
#include <chrono>
#include <sstream>
#include <vector>
#include <string>

#include <opencv2/opencv.hpp>

#include "plib/util/status.hpp"
#include "plib/util/logger.hpp"


class Camera
{

    public:

        /**
         * @brief Creates a new VideoCapture wrapper but does NOT initialize anything
         * 
         */
        Camera();

        /**
         * @brief Instantiates and opens the `VideoCapture` device at the given index.
         * The name will automatically be set to `camera_<index>`
         * 
         * You can find the index of your camera by using 
         * 
         * `$ ls -l /dev/v4l/by-id/`
         * 
         * Example:
         * 
         * `Camera(0) -> m_name = "camera_0"`
         * 
         * @param index `int` The index of the camera
         * @param api_type `cv::VideoCaptureAPIs` The API Type to use, defaults to `cv::CAP_ANY`
         * @return `status_utils::StatusCode` `OK` if successfully opened, `FAILED` otherwise 
         */
        status_utils::StatusCode open(int index, cv::VideoCaptureAPIs api_type = cv::CAP_ANY);

        /**
         * @brief Instantiates and opens the `VideoCapture` device at the given path.
         * The name will automatically be set to `camera_<path>`
         * 
         *  * You can find the path of your camera by using 
         * 
         * `$ ls -l /dev/v4l/by-path/`
         * 
         * Example:
         * 
         * `Camera("/dev/v4l/by-path/pci-0000:05:00.3-usb-0:4:1.0-video-index0") -> m_path = "camera_/dev/v4l/by-path/pci-0000:05:00.3-usb-0:4:1.0-video-index0`
         * 
         * @param path `std::string` The path of the device, usually in `/dev/...`
         * @param api_type `cv::VideoCaptureAPIs` The API Type to use, defaults to `cv::CAP_ANY`
         * @return `status_utils::StatusCode` `OK` if successfully opened, `FAILED` otherwise 
         */
        status_utils::StatusCode open(std::string path, cv::VideoCaptureAPIs api_type = cv::CAP_ANY);

        /**
         * @brief Gets the pointer to the internal OpenCV VideoCapture device
         * 
         * @return `cv::VideoCapture*` 
         */
        cv::VideoCapture* get_device();

        /**
         * @brief Gets if the VideoCapture device is opened or not
         * 
         * @return `true` 
         * @return `false` 
         */
        bool is_opened();

        /**
         * @brief Releases the device
         * 
         */
        void release();

        /**
         * @brief Configure basic camera properties
         * 
         * @param width `int` Resolution width in pixels
         * @param height `int` Resolution height in pixels
         * @param fps `int` Frames per second
         * @param attempts `int = 1` The number of attempts to try to set each property
         * @return `status_utils::StatusCode` `OK` if all successful. `FAILED` otherwise 
         */
        status_utils::StatusCode configure(int width, int height, int fps, int attempts = 1);

        /**
         * @brief Get the value of the specified property
         * 
         * @param property `cv::VideoCaptureProperties`
         * @return `double` 
         */
        double get_property(cv::VideoCaptureProperties property);

        /**
         * @brief Set a property to the camera, like resolution width / height. This method has an acknowledgement built in,
         * to use it you must set `attempts` to a value greater than 1, signifying how many attempts to try to set the property 
         * 
         * @param property `cv::VideoCaptureProperties`
         * @param value `double` The value to set this property to
         * @param attempts `int = 1` The number of attempts for trying to set this property
         * @return `status_utils::StatusCode` `OK` if set, `FAILED` otherwise 
         */
        status_utils::StatusCode set_property(cv::VideoCaptureProperties property, double value, int attempts = 1);        

        /**
         * @brief Gets the current camera frame
         * 
         * @param as_gray `bool = false` Flag get the grayscale image. Set to true for grayscale
         * @return `cv::Mat` 
         */
        cv::Mat get_frame(bool as_gray = false);

    private:

        // The OpenCV backend device
        cv::VideoCapture m_capture_device;

        // The index of the camera
        int m_index = -1;

        // The device path of the camera
        std::string m_path;

        // The name of the camera
        std::string m_name;


}; // class Camera


#endif // CAMERA_HPP