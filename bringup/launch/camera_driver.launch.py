import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.substitutions import LaunchConfiguration, Command
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch_ros.actions import Node

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, RegisterEventHandler
from launch.conditions import IfCondition, UnlessCondition
from launch.event_handlers import OnProcessExit
from launch.substitutions import Command, FindExecutable, PathJoinSubstitution, LaunchConfiguration
from launch.launch_description_sources import PythonLaunchDescriptionSource

from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

import xacro

def generate_launch_description():
    
    # Declare arguments
    declared_arguments = []

    declared_arguments.append(
        DeclareLaunchArgument(
            "params_file",
            default_value=os.path.join(get_package_share_directory('camera_driver'), 'config', 'camera_driver.yaml'),
            description="The YAML configuration file to use when launching the main node"
        )
    )

    params_file = LaunchConfiguration("params_file")

    camera_driver_node = Node(
        package="camera_driver",
        executable="camera_driver",
        parameters=[params_file],
    )

   
    nodes = [
        camera_driver_node
    ]

    return LaunchDescription(declared_arguments + nodes)
