# ROS 2 port for libQi

libQi is a C++ middleware that provides RPC, type-erasure,
cross-language interoperability, OS abstractions, logging facilities,
asynchronous task management, dynamic module loading.

## Compilation

Clone this project in your ROS 2 workspace (under `src/`),
and run `colcon build`.

## C++ Example

The following example shows some features of the framework, please refer to the
documentation for further details.

```cpp
#include <boost/make_shared.hpp>
#include <qi/log.hpp>
#include <qi/applicationsession.hpp>
#include <qi/anyobject.hpp>

qiLogCategory("myapplication");

class MyService
{
public:
  void myFunction(int val) {
    qiLogInfo() << "myFunction called with " << val;
  }
  qi::Signal<int> eventTriggered;
  qi::Property<float> angle;
};

// register the service to the type-system
QI_REGISTER_OBJECT(MyService, myFunction, eventTriggered, angle);

void print()
{
  qiLogInfo() << "print was called";
}

int main(int argc, char* argv[])
{
  qi::ApplicationSession app(argc, argv);

  // connect the session included in the app
  app.start();

  qi::SessionPtr session = app.session();

  // register our service
  session->registerService("MyService", boost::make_shared<MyService>());

  // get our service through the middleware
  qi::AnyObject obj = session->service("MyService").value();

  // call myFunction
  obj.call<void>("myFunction", 42);

  // call print in 2 seconds
  qi::async(&print, qi::Seconds(2));

  // block until ctrl-c
  app.run();
}
```

You can then run the program with:

```bash
./myservice --qi-standalone # for a standalone server
./myservice --qi-url tcp://somemachine:9559 # to connect to another galaxy of sessions
```

## Links

Upstream repository:
http://github.com/aldebaran/libqi

Documentation:
http://doc.aldebaran.com/libqi/

IRC Channel:
#qi on freenode.

Upstream Maintainers:

- Joël Lamotte <jlamotte@aldebaran.com>
- Jérémy Monnon <jmonnon@aldebaran.com>
- Matthieu Paindavoine <matthieu.paindavoine@softbankrobotics.com>
- Vincent Palancher <vincent.palancher@external.softbankrobotics.com>

See the [`package.xml`](package.xml) for the ROS 2 maintainers.

ROS Distro| Binary Status | Source Status | Github Build |
|-------------------|-------------------|-------------------|-------------------|
Humble | [![ros2-humble-jammy-bin-status-badge](https://build.ros2.org/job/Hbin_uJ64__naoqi_libqi__ubuntu_jammy_amd64__binary/badge/icon)](https://build.ros2.org/job/Hbin_uJ64__naoqi_libqi__ubuntu_jammy_amd64__binary) | [![ros2-humble-jammy-src-status-badge](https://build.ros2.org/job/Hsrc_uJ__naoqi_libqi__ubuntu_jammy__source/badge/icon)](https://build.ros2.org/job/Hsrc_uJ__naoqi_libqi__ubuntu_jammy__source) | [![ros2-humble-jammy](https://github.com/ros-naoqi/libqi/actions/workflows/humble_jammy.yml/badge.svg?branch=ros2)](https://github.com/ros-naoqi/libqi/actions/workflows/humble_jammy.yml)
Iron | [![ros2-iron-jammy-bin-status-badge](https://build.ros2.org/job/Ibin_uJ64__naoqi_libqi__ubuntu_jammy_amd64__binary/badge/icon)](https://build.ros2.org/job/Ibin_uJ64__naoqi_libqi__ubuntu_jammy_amd64__binary) | [![ros2-iron-jammy-src-status-badge](https://build.ros2.org/job/Isrc_uJ__naoqi_libqi__ubuntu_jammy__source/badge/icon)](https://build.ros2.org/job/Isrc_uJ__naoqi_libqi__ubuntu_jammy__source) | [![ros2-iron-jammy](https://github.com/ros-naoqi/libqi/actions/workflows/iron_jammy.yml/badge.svg?branch=ros2)](https://github.com/ros-naoqi/libqi/actions/workflows/iron_jammy.yml)
