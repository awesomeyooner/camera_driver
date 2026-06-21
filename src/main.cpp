#include <rclcpp/rclcpp.hpp>

#include "camera_driver/core_node.hpp"


using namespace status_utils;
using namespace std;


int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);

    shared_ptr node = make_shared<CoreNode>();

    StatusCode init_status = node->init();

    if(init_status == StatusCode::OK)
        rclcpp::spin(node);

    node->shutdown();

    rclcpp::shutdown();

    return 0;

} // end of "main(int, char**)"