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


class Camera
{

    public:

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