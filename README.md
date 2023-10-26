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
