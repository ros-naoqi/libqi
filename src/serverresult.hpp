#pragma once
/*
**  Copyright (C) 2012 Aldebaran Robotics
**  See COPYING for the license
*/

#ifndef _SRC_SERVERRESULT_HPP_
#define _SRC_SERVERRESULT_HPP_

#include <qi/future.hpp>
#include "message.hpp"
#include "transportsocket.hpp"

namespace qi {

  namespace detail
  {
    static inline void _genericobject_noop(GenericObject*)
    {}
  }
  // second bounce when returned type is a future
  inline void serverResultAdapterNext(AnyReference val,// the future
    Signature targetSignature,
    ObjectHost* host,
    TransportSocketPtr socket, const qi::MessageAddress &replyaddr)
  {
    qi::Message ret(Message::Type_Reply, replyaddr);
    try {
      TemplateTypeInterface* futureType = QI_TEMPLATE_TYPE_GET(val.type(), Future);
      ObjectTypeInterface* onext = dynamic_cast<ObjectTypeInterface*>(futureType->next());
      GenericObject gfut(onext, val.rawValue());
      // Need a live shared_ptr for shared_from_this() to work.
      detail::ManagedObjectPtr ao(&gfut, &detail::_genericobject_noop);
      if (gfut.call<bool>("hasError", 0))
      {
        ret.setType(qi::Message::Type_Error);
        ret.setError(gfut.call<std::string>("error", 0));
      }
      else
      {
        // Future<void>::value() give a void* so we need a special handling to
        // produce a real void
        if (futureType->templateArgument()->kind() == TypeKind_Void)
          ret.setValue(AnyValue(qi::typeOf<void>()), targetSignature, host);
        else
        {
          AnyValue v = gfut.call<AnyValue>("value", 0);
          ret.setValue(v, targetSignature, host);
        }
      }
    } catch (const std::exception &e) {
      //be more than safe. we always want to nack the client in case of error
      ret.setType(qi::Message::Type_Error);
      ret.setError(std::string("Uncaught error:") + e.what());
    } catch (...) {
      //be more than safe. we always want to nack the client in case of error
      ret.setType(qi::Message::Type_Error);
      ret.setError("Unknown error caught while forwarding the answer");
    }
    if (!socket->send(ret))
      qiLogError("qimessaging.serverresult") << "Can't generate an answer for address:" << replyaddr;
  }

  inline void serverResultAdapter(qi::Future<AnyReference> future,
    const qi::Signature& targetSignature,
    ObjectHost* host, TransportSocketPtr socket, const qi::MessageAddress &replyaddr) {
    qi::Message ret(Message::Type_Reply, replyaddr);

    if (future.hasError()) {
      ret.setType(qi::Message::Type_Error);
      ret.setError(future.error());
    } else {
      try {
        qi::AnyReference val = future.value();
        TemplateTypeInterface* futureType = QI_TEMPLATE_TYPE_GET(val.type(), Future);
        if (futureType)
        { // Return value is a future, bounce
          TypeInterface* next = futureType->next();
          ObjectTypeInterface* onext = dynamic_cast<ObjectTypeInterface*>(next);
          GenericObject gfut(onext, val.rawValue());
          // Need a live sha@red_ptr for shared_from_this() to work.
          detail::ManagedObjectPtr ao(&gfut, &detail::_genericobject_noop);
          boost::function<void()> cb = boost::bind(serverResultAdapterNext, val, targetSignature, host, socket, replyaddr);
          gfut.call<void>("_connect", cb);
          return;
        }
        ret.setValue(val, targetSignature, host);
        //may leak if something throw inbetween.
        val.destroy();
      } catch (const std::exception &e) {
        //be more than safe. we always want to nack the client in case of error
        ret.setType(qi::Message::Type_Error);
        ret.setError(std::string("Uncaught error:") + e.what());
      } catch (...) {
        //be more than safe. we always want to nack the client in case of error
        ret.setType(qi::Message::Type_Error);
        ret.setError("Unknown error caught while sending the answer");
      }
    }
    if (!socket->send(ret))
      qiLogError("qimessaging.serverresult") << "Can't generate an answer for address:" << replyaddr;
  }
}

#endif  // _SRC_SERVERRESULT_HPP_
