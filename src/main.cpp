#include <rclcpp/rclcpp.hpp>

#include "camera_driver/core_node.hpp"


using namespace std;


int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);

    shared_ptr node = make_shared<CoreNode>();

    node->init();

    rclcpp::spin(node);

    node->shutdown();

    rclcpp::shutdown();

    return 0;

} // end of "main(int, char**)"