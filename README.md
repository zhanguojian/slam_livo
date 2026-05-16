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

### 2.3 Sophus

#### Binary installation
```bash
sudo apt install ros-$ROS_DISTRO-sophus
```

#### Building from source
Sophus Installation for the non-templated/double-only version.

```bash
git clone https://github.com/strasdat/Sophus.git
cd Sophus
git checkout a621ff
mkdir build && cd build && cmake ..
make
sudo make install
```

if build fails due to `so2.cpp:32:26: error: lvalue required as left operand of assignment`, modify the code as follows:

**so2.cpp**
```diff
namespace Sophus
{

SO2::SO2()
{
-  unit_complex_.real() = 1.;
-  unit_complex_.imag() = 0.;
+  unit_complex_.real(1.);
+  unit_complex_.imag(0.);
}
```

### 2.4 Vikit

Vikit contains camera models, some math and interpolation functions that we need. Vikit is a catkin project, therefore, download it into your catkin workspace source folder.

For well-known reasons, ROS2 does not have a direct global parameter server and a simple method to obtain the corresponding parameters. For details, please refer to https://discourse.ros.org/t/ros2-global-parameter-server-status/10114/11. I use a special way to get camera parameters in Vikit. While the method I've provided so far is quite simple and not perfect, it meets my needs. More contributions to improve `rpg_vikit` are hoped.

```bash
# Different from the one used in fast-livo1
cd livo_ws/src
git clone https://github.com/Robotic-Developer-Road/rpg_vikit.git 
```

Thanks to the following repositories for the code reference:

- [uzh-rpg/rpg_vikit](https://github.com/uzh-rpg/rpg_vikit)
- [xuankuzcr/rpg_vikit](https://github.com/xuankuzcr/rpg_vikit)
- [uavfly/vikit](https://github.com/uavfly/vikit)


## 3. Build

Clone the repository and colcon build:

```
cd ~/livo_ws/src
git clone  https://github.com/zhanguojian/slam_livo-2.git
cd ../
colcon build --symlink-install --continue-on-error
source ~/里vox/install/setup.bash
```

### Run the demo

Do not forget to `source` your ROS2 workspace before running the following command.

```bash
ros2 launch fast_livo mapping_aviz.launch.py use_rviz:=True

source /opt/ros/humble/setup.zsh
source ~/livo_ws/install/setup.zsh
ros2 bag play  bag数据包 # space bar controls play/pause
```
