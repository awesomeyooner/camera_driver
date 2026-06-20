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


class Camera
{

    public:

        /**
         * @brief Create a new VideoCapture wrapper with the given camera index.
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
         */
        Camera(int index, cv::VideoCaptureAPIs api_type = cv::CAP_ANY);

        /**
         * @brief Create a new VideoCapture wrapper at the given path.
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
         */
        Camera(std::string path, cv::VideoCaptureAPIs api_type = cv::CAP_ANY);

        



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