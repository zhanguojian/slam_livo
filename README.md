# FAST-livo ROS2 HUMBLE

### 1 Our LiDAR-camera calibration method
The [**FAST-Calib**](https://github.com/hku-mars/FAST-Calib) toolkit is recommended. Its output extrinsic parameters can be directly filled into the YAML file. 

## 2. Prerequisited

### 2.1 Ubuntu and ROS

Ubuntu 22.04.  [ROS Installation](http://wiki.ros.org/ROS/Installation).

### 2.2 PCL && Eigen && OpenCV

PCL>=1.8, Follow [PCL Installation](https://pointclouds.org/). 

Eigen>=3.3.4, Follow [Eigen Installation](https://eigen.tuxfamily.org/index.php?title=Main_Page).

OpenCV>=4.2, Follow [Opencv Installation](http://opencv.org/).

sudo apt update

sudo apt install -y \
  build-essential \
  cmake \
  git \
  pkg-config \
  libpcl-dev \
  libeigen3-dev \
  libopencv-dev


### 3 Vikit

Vikit contains camera models, some math and interpolation functions that we need. Vikit is a catkin project, therefore, download it into your catkin workspace source folder.

```bash
# Different from the one used in fast-livo1
cd livo_ws/src
git clone https://github.com/zhanguojian/vpg_vikit.git 
```

Building vikit_common
```bash
cd vikit_common
mkdir build && cd build
cmake .. && make -j$(nproc)
sudo make install
```

Building vikit_ros
```bash
# Move vikit_ros to your workspace src directory
cd ~/ros2_ws
colcon build --symlink-install --packages-select vikit_ros
```

**需要先编译rqg_vikit 再对slam仓库进行编译**

## 4. Build

Clone the repository and colcon build:

```
cd ~/livo_ws/src
git clone  https://github.com/zhanguojian/slam_livo.git
cd ../
colcon build --symlink-install --continue-on-error
source ~/livo_ws/install/setup.zsh
```

### Run the demo

Do not forget to `source` your ROS2 workspace before running the following command.

```bash
ros2 launch fast_livo mapping_aviz.launch.py use_rviz:=True

source /opt/ros/humble/setup.zsh
source ~/livo_ws/install/setup.zsh
ros2 bag play  bag数据包 # space bar controls play/pause
```
