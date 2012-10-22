/*
**
** Author(s):
**  - Pierre Roullon <proullon@aldebaran-robotics.com>
**
** Copyright (C) 2012 Aldebaran Robotics
*/

#include <qi/os.hpp>

#include <testsession/testsession.hpp>
#include <testsession/testsessionpair.hpp>

TestSessionPair::TestSessionPair()
{

  // #1 Get active mode.
  _mode = TestMode::getTestMode();

  // #2 Listen.
  _sd.listen("tcp://0.0.0.0:0");

  // #3 Get client and server sessions.
  _client = new TestSession(_sd.endpoints()[0].str(), false, _mode);
  _server = new TestSession(_sd.endpoints()[0].str(), true, _mode);
}

TestSessionPair::TestSessionPair(TestMode::Mode mode)
{

  // #0 Set active mode.
  _mode = mode;

  // #1 Listen.
  _sd.listen("tcp://0.0.0.0:0");

  // #2 Get client and server sessions.
  _client = new TestSession(_sd.endpoints()[0].str(), false, mode);
  _server = new TestSession(_sd.endpoints()[0].str(), true, mode);
}

TestSessionPair::TestSessionPair(TestSessionPair &other)
{
  // #1 Get active mode.
  _mode = TestMode::getTestMode();

  // #2 Get client and server sessions using other pair service directory.
  _client = new TestSession(other._sd.endpoints()[0].str(), false, _mode);
  _server = new TestSession(other._sd.endpoints()[0].str(), true, _mode);
}

TestSessionPair::~TestSessionPair()
{
  delete _client;
  delete _server;
}

qi::Session* TestSessionPair::client() const
{
  // #0 If activated test mode is 'Direct', cheat.
  if (_mode == TestMode::Mode_Direct)
    return _server->session();

  return _client->session();
}

qi::Session* TestSessionPair::server() const
{
  return _server->session();
}
