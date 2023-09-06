# naoqi_libqi

This fork is used to define the __naoqi_libqi__ ROS2 package, based on [__libqi__](https://github.com/aldebaran/libqi).

libqi is a middle-ware framework that provides RPC, type-erasure,
cross-language interoperability, OS abstractions, logging facilities,
asynchronous task management, dynamic module loading.

## Compilation
To compile __naoqi_libqi__, clone this repository in a ROS2 workspace and use the `colcon build` command.

Please note that you should checkout the branch corresponding to your ROS distro (eg. `galactic-devel` for Galactic, `foxy-devel` for Foxy, etc...)

## Working from container

You can work on this project from a dev container:

```bash
docker build -t ros2-naoqi-libqi --target dev_with_deps .
docker run --volume=.:/home/user/ws/src/naoqi-libqi -it ros2-naoqi-libqi
```

Edit the [`Dockerfile`](./Dockerfile) to set the target distro you want to work on.

## Status

The project can currently be successfully built for `Galactic (focal)` and `Foxy (focal)` from source. Please refer to the [build workflow](https://github.com/ros-naoqi/libqi/actions) for more information. The package binary status column details wether the package has been released for a specific distro.


ROS Distro | Binary Status | Source Status | Github Build
|-------------------|-------------------|-------------------|-------------------|
Humble | | | [![ros2-humble-jammy](https://github.com/ros-naoqi/libqi/actions/workflows/humble_jammy.yml/badge.svg)](https://github.com/ros-naoqi/libqi/actions/workflows/humble_jammy.yml)
Galactic | [![Build Status](https://build.ros2.org/job/Gbin_uF64__naoqi_libqi__ubuntu_focal_amd64__binary/badge/icon)](https://build.ros2.org/job/Gbin_uF64__naoqi_libqi__ubuntu_focal_amd64__binary/) | [![Build Status](https://build.ros2.org/job/Gsrc_uF__naoqi_libqi__ubuntu_focal__source/badge/icon)](https://build.ros2.org/job/Gsrc_uF__naoqi_libqi__ubuntu_focal__source/) | [![ros2-galactic-focal](https://github.com/ros-naoqi/libqi/actions/workflows/galactic_focal.yml/badge.svg)](https://github.com/ros-naoqi/libqi/actions/workflows/galactic_focal.yml)
Foxy | [![Build Status](https://build.ros2.org/job/Fbin_uF64__naoqi_libqi__ubuntu_focal_amd64__binary/badge/icon)](https://build.ros2.org/job/Fbin_uF64__naoqi_libqi__ubuntu_focal_amd64__binary/) | [![Build Status](https://build.ros2.org/job/Fsrc_uF__naoqi_libqi__ubuntu_focal__source/badge/icon)](https://build.ros2.org/job/Fsrc_uF__naoqi_libqi__ubuntu_focal__source/) | [![ros2-foxy-focal](https://github.com/ros-naoqi/libqi/actions/workflows/foxy_focal.yml/badge.svg)](https://github.com/ros-naoqi/libqi/actions/workflows/foxy_focal.yml)