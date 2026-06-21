# camera_driver
ROS 2 package for publishing OpenCV camera data, such as raw frames and also camera distortion parameters.

## Prerequisites

Before using, please install `v4l-utils`

```bash
$ sudo apt install v4l-utils
```

Everything else is already installed in the base `jazzy` installation

## Usage

Please create a `.yaml` file, here is an example

```yaml
camera_driver:
  ros__parameters:

    camera_name: ""
    camera_frame_id: "my_camera"
    # camera_path: "/dev/v4l/by-path/pci-0000:05:00.3-usb-0:4:1.0-video-index0"
    camera_index: 0

    publish_rate: 60.0 # Hz
    publish_as_gray: false
    publish_compressed: true
    publish_camera_info: false

    flip_horizontally: false
    flip_vertically: false

    resolution: [1280, 720]
    image_fps: 30

    configuration_attempts: 10
```

You can find these params using the following:

```bash
# Lists all V4L2 devices
$ v4l2-ctl --list-devices

# List the IO capabilities for a specific camera (found with the above command)
$ v4l2-ctl -d /dev/video0 --list-formats-ext

# Lists the IDs of a device like /dev/video0. To use this, pass it into `camera_index`
ls -l /dev/v4l/by-id/

# Lists the hardware path of the device. To use this, pass it into `camera_path`
ls -l /dev/v4l/by-path/
```

To launch, create a `.launch.py` file in the `\launch` folder

`camera.launch.py`

```python
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
    
    # CHANGE ME
    package = "my_package"

    # CHANGE ME
    params_file = os.path.join(get_package_share_directory(package), 'config','camera_driver.yaml')

    launch_file = os.path.join(
        get_package_share_directory("camera_driver"),
        "launch",
        "camera_driver.launch.py"
    )

    camera_driver_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(launch_file),
        launch_arguments={
            "params_file": params_file
        }.items()
    )

    nodes = [
        joystick_launch
    ]

    return LaunchDescription(nodes)

```

and launch with `ros2 launch <my_package> camera.launch.py`

## Usage with Docker

When using docker, you can put a volume mount on all your devices and give permissions for video devices

```bash
docker run \
    --device-cgroup-rule='c 81:* rmw' \
    --device-cgroup-rule='a 189:* rmw' \
    -v /dev:/dev \
    image:tag
```

- `--volume /dev:/dev` Will put a volume mount on all devices
- `--device-cgroup-rule='c 81:* rmw'` Will allow permission to use video devices
- `--device-cgroup-rule='c 189:* rmw'` Will allow permission to use USB video devices specifically