# naoqi_libqi [![build](https://github.com/ros-naoqi/libqi/actions/workflows/build.yml/badge.svg?branch=ros)](https://github.com/ros-naoqi/libqi/actions/workflows/build.yml)

This fork is used to define the __naoqi_libqi__ ROS package, based on [__libqi__](https://github.com/aldebaran/libqi).

libqi is a middle-ware framework that provides RPC, type-erasure,
cross-language interoperability, OS abstractions, logging facilities,
asynchronous task management, dynamic module loading.

## Compilation
To compile __naoqi_libqi__, clone this repository in a ROS workspace and use the `catkin_make` command.

Please note that you should checkout the branch corresponding to your ROS distro (eg. `melodic-devel` for Melodic, `noetic-devel` for Noetic, etc...)

## Status 

The project can currently be successfully built for `Noetic (focal)` and `Melodic (bionic)`. Please refer to the [build workflow](https://github.com/ros-naoqi/libqi/actions/workflows/build.yml) for more information

<!-- [melodic-stretch]: https://github.com/ros-naoqi/libqi/actions/workflows/build.yml/badge.svg?branch=ros -->


<!-- | ROS Distribution  | Status             |
|-------------------|--------------------|
| Noetic            | ![noetic]          |
| Melodic           | ![melodic]         |
| Melodic (stretch) | ![melodic-stretch] |
| Kinetic           | ![kinetic]         | -->