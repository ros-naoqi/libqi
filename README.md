# naoqi_libqi

This fork is used to define the __naoqi_libqi__ ROS package, based on [__libqi__](https://github.com/aldebaran/libqi).

libqi is a middle-ware framework that provides RPC, type-erasure,
cross-language interoperability, OS abstractions, logging facilities,
asynchronous task management, dynamic module loading.

## Compilation
To compile __naoqi_libqi__, clone this repository in a ROS workspace and use the `catkin_make` command.

Please note that you should checkout the branch corresponding to your ROS distro (eg. `melodic-devel` for Melodic, `noetic-devel` for Noetic, etc...)

## Status 

The project can currently be successfully built for `Noetic (focal)` and `Melodic (bionic)` from source. Please refer to the [workflows](https://github.com/ros-naoqi/libqi/actions) for more information. The package binary status column details wether the package has been released for a specific distro.

ROS Distro | Binary Status | Source status | Github Build |
|-------------------|-------------------|-------------------|-------------------|
Noetic | [![Build Status](https://build.ros.org/job/Nbin_uf64__naoqi_libqi__ubuntu_focal_amd64__binary/badge/icon)](https://build.ros.org/job/Nbin_uf64__naoqi_libqi__ubuntu_focal_amd64__binary/) | [![Build Status](https://build.ros.org/job/Nsrc_uF__naoqi_libqi__ubuntu_focal__source/badge/icon)](https://build.ros.org/job/Nsrc_uF__naoqi_libqi__ubuntu_focal__source/) | [![ros-noetic-focal](https://github.com/ros-naoqi/libqi/actions/workflows/noetic_focal.yml/badge.svg)](https://github.com/ros-naoqi/libqi/actions/workflows/noetic_focal.yml) 
Melodic | [![Build Status](https://build.ros.org/job/Mbin_ub64__naoqi_libqi__ubuntu_bionic_amd64__binary/badge/icon)](https://build.ros.org/job/Mbin_ub64__naoqi_libqi__ubuntu_bionic_amd64__binary/) | [![Build Status](https://build.ros.org/job/Msrc_uB__naoqi_libqi__ubuntu_bionic__source/badge/icon)](https://build.ros.org/job/Msrc_uB__naoqi_libqi__ubuntu_bionic__source/) | [![ros-melodic-bionic](https://github.com/ros-naoqi/libqi/actions/workflows/melodic_bionic.yml/badge.svg)](https://github.com/ros-naoqi/libqi/actions/workflows/melodic_bionic.yml)
Kinetic | ![passing](https://raw.githubusercontent.com/jenkinsci/embeddable-build-status-plugin/7c7eedc7617851f07a1f09629c33fee11cff50ab/src/doc/flat_unconfigured.svg) | ![passing](https://raw.githubusercontent.com/jenkinsci/embeddable-build-status-plugin/7c7eedc7617851f07a1f09629c33fee11cff50ab/src/doc/flat_unconfigured.svg) | [![ros-kinetic-xenial](https://github.com/ros-naoqi/libqi/actions/workflows/kinetic_xenial.yml/badge.svg)](https://github.com/ros-naoqi/libqi/actions/workflows/kinetic_xenial.yml) |