# naoqi_libqi

This fork is used to define the __naoqi_libqi__ ROS package, based on [__libqi__](https://github.com/aldebaran/libqi).

libqi is a middle-ware framework that provides RPC, type-erasure,
cross-language interoperability, OS abstractions, logging facilities,
asynchronous task management, dynamic module loading.

## Compilation
To compile __naoqi_libqi__, clone this repository in a ROS workspace and use the `catkin_make` command.

Please note that you should checkout the branch corresponding to your ROS distro (eg. `melodic-devel` for Melodic, `noetic-devel` for Noetic, etc...)

## Status 

The project can currently be successfully built for `Noetic (focal)` and `Melodic (bionic)`. Please refer to the [workflows](https://github.com/ros-naoqi/libqi/actions) for more information

| ROS Build Status  |
|-------------------|
| [![ros-noetic-focal](https://github.com/ros-naoqi/libqi/actions/workflows/noetic_focal.yml/badge.svg)](https://github.com/ros-naoqi/libqi/actions/workflows/noetic_focal.yml) |
| [![ros-melodic-bionic](https://github.com/ros-naoqi/libqi/actions/workflows/melodic_bionic.yml/badge.svg)](https://github.com/ros-naoqi/libqi/actions/workflows/melodic_bionic.yml) |
| [![ros-melodic-stretch](https://github.com/ros-naoqi/libqi/actions/workflows/melodic_stretch.yml/badge.svg)](https://github.com/ros-naoqi/libqi/actions/workflows/melodic_stretch.yml) |
| [![ros-kinetic-xenial](https://github.com/ros-naoqi/libqi/actions/workflows/kinetic_xenial.yml/badge.svg)](https://github.com/ros-naoqi/libqi/actions/workflows/kinetic_xenial.yml) |