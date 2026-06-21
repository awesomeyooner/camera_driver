#include "camera_driver/core_node.hpp"


using namespace rclcpp;
using namespace camera_driver;
using namespace status_utils;
using namespace cv;
using namespace cv_bridge;
using namespace std;


CoreNode::CoreNode()
    :   Node("camera_driver_node")
{} // end of "CoreNode()"


StatusCode CoreNode::init()
{
    m_param_listener = std::make_shared<ParamListener>(this);
    m_params = m_param_listener->get_params();

    if(m_params.camera_path == "unused")
    {
        StatusCode open_status = m_camera.open(m_params.camera_index, CAP_V4L2);

        if(open_status != StatusCode::OK)
            return StatusCode::FAILED;
    }
    else
    {
        StatusCode open_status = m_camera.open(m_params.camera_path, CAP_V4L2);

        if(open_status != StatusCode::OK)
            return StatusCode::FAILED;
    }

    int width = m_params.resolution[0];
    int height = m_params.resolution[1];
    int fps = m_params.image_fps;

    if(width != -1 && height != -1 && fps != -1)
    {
        StatusCode configure_status = m_camera.configure(width, height, fps);
        
        if(configure_status != StatusCode::OK)
            return StatusCode::FAILED;
    }

    m_image_raw_publisher = this->create_publisher<sensor_msgs::msg::Image>(m_params.camera_name + "/image_raw", 10);
    m_image_compressed_publisher = this->create_publisher<sensor_msgs::msg::CompressedImage>(m_params.camera_name + "/image_raw/compressed", 10);
    // m_camera_info_publisher = this->create_publisher<sensor_msgs::msg::CameraInfo>(m_params.camera_name + "/camera_info", 10);

    m_timer = this->create_wall_timer(
        std::chrono::milliseconds(int((1.0 / m_params.publish_rate) * 1000)),
        std::bind(&CoreNode::publish_data, this)
    );

    return StatusCode::OK;

} // end of "init()"


void CoreNode::info(string message)
{
    RCLCPP_INFO(this->get_logger(), message.c_str());

} // end of "info(string)"


void CoreNode::shutdown()
{
    m_camera.release();

} // end of "shutdown()"


StatusCode CoreNode::publish_data()
{
    cv::Mat frame = m_camera.get_frame(m_params.publish_as_gray);

    if(frame.empty())
        return StatusCode::FAILED;

    std_msgs::msg::Header header = std_msgs::msg::Header();
        header.stamp = this->now();
        header.frame_id = m_params.camera_frame_id;

    CvImage cv_image = CvImage(
        header,
        m_params.publish_as_gray ? "mono8" : "bgr8",
        frame
    );
    
    // Raw Image
    m_image_raw_publisher->publish(*cv_image.toImageMsg());

    // Compressed Image
    if(m_params.publish_compressed)
        m_image_compressed_publisher->publish(*cv_image.toCompressedImageMsg(Format::JPEG));

    // Camera Info (I'll do it later)
    if(m_params.publish_camera_info)
    {
        //...
    }

    return StatusCode::OK;

} // end of "publish_data()"