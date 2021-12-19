# naoqi_libqi [![build](https://github.com/ros-naoqi/libqi/actions/workflows/build.yml/badge.svg?branch=ros2)](https://github.com/ros-naoqi/libqi/actions/workflows/build.yml)

This fork is used to define the __naoqi_libqi__ ROS2 package, based on [__libqi__](https://github.com/aldebaran/libqi).

libqi is a middle-ware framework that provides RPC, type-erasure,
cross-language interoperability, OS abstractions, logging facilities,
asynchronous task management, dynamic module loading.

## Compilation
To compile __naoqi_libqi__, clone this repository in a ROS2 workspace and use the `colcon build` command.

Please note that you should checkout the branch corresponding to your ROS distro (eg. `galactic-devel` for Galactic, `foxy-devel` for Foxy, etc...)

## Status 

The project can currently be successfully built for `Galactic (focal)` and `Foxy (focal)`. Please refer to the [build workflow](https://github.com/ros-naoqi/libqi/actions/workflows/build.yml) for more information