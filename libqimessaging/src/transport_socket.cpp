/*
** transport-client.cpp
** Login : <hcuche@hcuche-de>
** Started on  Thu Jan  5 15:21:13 2012 Herve Cuche
** $Id$
**
** Author(s):
**  - Herve Cuche <hcuche@aldebaran-robotics.com>
**
** Copyright (C) 2012 Herve Cuche
*/

#include <iostream>
#include <cstring>
#include <map>


#include <qi/log.hpp>

#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <qimessaging/transport_socket.hpp>
#include "src/message_p.hpp"
#include "src/buffer_p.hpp"
#include "src/transport_socket_libevent_p.hpp"
#include "src/transport_socket_dummy_p.hpp"

#include <qimessaging/message.hpp>
#include <qimessaging/datastream.hpp>
#include <qimessaging/buffer.hpp>

namespace qi
{
  TransportSocketInterface::~TransportSocketInterface()
  {
  }

  TransportSocket::TransportSocket()
    : _p(new TransportSocketDummyPrivate(this))
  {
  }

  TransportSocket::~TransportSocket()
  {
    {
      boost::recursive_mutex::scoped_lock l(_p->mtxCallback);
      _p->tcd.clear();
    }
    _p->destroy();
  }

  qi::FutureSync<bool> TransportSocket::connect(const qi::Url &url, qi::EventLoop* ctx)
  {
    TransportSocketPrivate *save = _p;
    _p->status = 0;
    if (url.protocol() != "tcp") {
      qiLogError("TransportSocket") << "Unrecognized protocol to create the TransportSocket: " << url.protocol();
      return Future<bool>(false);
    }

    _p = new qi::TransportSocketLibEvent(this);
    _p->tcd = save->tcd;
    _p->connected = save->connected;
    _p->readHdr = save->readHdr;
    _p->msg = save->msg;
    _p->self = save->self;
    _p->url = url;
    delete save;
    return _p->connect(url, ctx);
  }

  qi::Url TransportSocket::url() const {
    return _p->url;
  }

  qi::FutureSync<void> TransportSocket::disconnect()
  {
    return _p->disconnect();
  }

  int TransportSocket::status() const {
    return _p->status;
  }

  bool TransportSocket::waitForId(int id, int msecs)
  {
    return _p->waitForId(id, msecs);
  }

  bool TransportSocket::read(int id, qi::Message *msg)
  {
    return _p->read(id, msg);
  }

  bool TransportSocket::send(const qi::Message &msg)
  {
    return _p->send(msg);
  }

  void TransportSocket::addCallbacks(TransportSocketInterface *delegate, void *data)
  {
    _p->addCallbacks(delegate, data);
  }

  void TransportSocket::removeCallbacks(TransportSocketInterface *delegate)
  {
    if (_p == NULL)
    {
      qiLogError("qimessaging.TransportSocket") << "socket is not connected.";
      return;
    }
    _p->removeCallbacks(delegate);
  }

  bool TransportSocket::isConnected() const
  {
    return _p->isConnected();
  }

}

