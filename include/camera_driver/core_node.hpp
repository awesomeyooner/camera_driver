#ifndef CORE_NODE_HPP
#define CORE_NODE_HPP


#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/compressed_image.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <cv_bridge/cv_bridge.hpp>
#include <camera_driver/camera_driver_parameters.hpp>

#include "camera_driver/camera.hpp"


class CoreNode : public rclcpp::Node
{

    public:

        CoreNode();

        status_utils::StatusCode init();

        void shutdown();

        status_utils::StatusCode publish_data();

    private:

        Camera m_camera;

        rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr m_image_raw_publisher;
        rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr m_image_compressed_publisher;

        // I'll implement this later
        // rclcpp::Publisher<sensor_msgs::msg::CameraInfo>::SharedPtr m_camera_info_publisher

        rclcpp::TimerBase::SharedPtr m_timer;

        std::shared_ptr<camera_driver::ParamListener> m_param_listener;
        camera_driver::Params m_params;

        void info(std::string message);


}; // class CoreNode : public rclcpp::Node


#endif // CORE_NODE_HPP