/*
**  Copyright (C) 2012 Aldebaran Robotics
**  See COPYING for the license
*/
#include <map>

#include <boost/make_shared.hpp>

#include <qitype/api.hpp>
#include <qitype/genericvalue.hpp>
#include <qitype/type.hpp>
#include <qitype/genericvalue.hpp>
#include <qitype/genericobject.hpp>
#include <qitype/functiontype.hpp>
#include <qitype/metaobject.hpp>
#include <qitype/signal.hpp>
#include <qitype/dynamicobject.hpp>


namespace qi
{

  class DynamicObjectPrivate
  {
  public:
    ~DynamicObjectPrivate();
    // get or create signal, or 0 if id is not an event
    SignalBase* createSignal(unsigned int id);
    bool                                dying;
    typedef std::map<unsigned int, SignalBase*> SignalMap;
    typedef std::map<unsigned int, GenericFunction> MethodMap;
    SignalMap           signalMap;
    MethodMap           methodMap;
    MetaObject          meta;
  };

  DynamicObjectPrivate::~DynamicObjectPrivate()
  {
    for (SignalMap::iterator it = signalMap.begin(); it!= signalMap.end(); ++it)
      delete it->second;
  }

  SignalBase* DynamicObjectPrivate::createSignal(unsigned int id)
  {
    SignalMap::iterator i = signalMap.find(id);
    if (i != signalMap.end())
      return i->second;

    MetaSignal* sig = meta.signal(id);
    if (sig)
    {
      SignalBase* sb = new SignalBase(qi::signatureSplit(sig->signature())[2]);
      signalMap[id] = sb;
      return sb;
    }
    return 0;
  }

  class DynamicObjectType: public ObjectType, public DefaultTypeImplMethods<DynamicObject>
  {
  public:
    DynamicObjectType() {}
    virtual const MetaObject& metaObject(void* instance);
    virtual qi::Future<GenericValuePtr> metaCall(void* instance, unsigned int method, const GenericFunctionParameters& params, MetaCallType callType = MetaCallType_Auto);
    virtual void metaPost(void* instance, unsigned int signal, const GenericFunctionParameters& params);
    virtual qi::Future<unsigned int> connect(void* instance, unsigned int event, const SignalSubscriber& subscriber);
    /// Disconnect an event link. Returns if disconnection was successful.
    virtual qi::Future<void> disconnect(void* instance, unsigned int linkId);
    virtual Manageable* manageable(void* intance);
    virtual const std::vector<std::pair<Type*, int> >& parentTypes();

    _QI_BOUNCE_TYPE_METHODS(DefaultTypeImplMethods<DynamicObject>);
  };

  DynamicObject::DynamicObject()
  {
    _p = boost::make_shared<DynamicObjectPrivate>();
  }

  DynamicObject::~DynamicObject()
  {
  }

  void DynamicObject::setMetaObject(const MetaObject& m)
  {
    _p->meta = m;
    // We will populate stuff on demand
  }

  MetaObject& DynamicObject::metaObject()
  {
    return _p->meta;
  }

  void DynamicObject::setMethod(unsigned int id, GenericFunction callable)
  {
    _p->methodMap[id] = callable;
  }

  GenericFunction DynamicObject::method(unsigned int id)
  {
    DynamicObjectPrivate::MethodMap::iterator i = _p->methodMap.find(id);
    if (i == _p->methodMap.end())
      return GenericFunction();
    else
      return i->second;
  }

  SignalBase* DynamicObject::signalBase(unsigned int id) const
  {
    DynamicObjectPrivate::SignalMap::iterator i = _p->signalMap.find(id);
    if (i == _p->signalMap.end())
      return 0;
    else
      return i->second;
  }

  qi::Future<GenericValuePtr> DynamicObject::metaCall(unsigned int method, const GenericFunctionParameters& params, MetaCallType callType)
  {
    qi::Promise<GenericValuePtr> out;
    DynamicObjectPrivate::MethodMap::iterator i = _p->methodMap.find(method);
    if (i == _p->methodMap.end())
    {
      std::stringstream ss;
      ss << "Can't find methodID: " << method;
      out.setError(ss.str());
      return out.future();
    }

    return ::qi::metaCall(eventLoop(), i->second, params, callType);
  }

  void DynamicObject::metaPost(unsigned int event, const GenericFunctionParameters& params)
  {
    SignalBase * s = _p->createSignal(event);
    if (s)
    { // signal is declared, create if needed
      s->trigger(params);
    }
    else
    {
      // Allow emit on a method
      // FIXME: call errors are lost
      if (metaObject().method(event))
        metaCall(event, params, MetaCallType_Auto);
      else
        qiLogError("object") << "No such event " << event;
    }
  }

  qi::Future<unsigned int> DynamicObject::metaConnect(unsigned int event, const SignalSubscriber& subscriber)
  {
    SignalBase * s = _p->createSignal(event);
    if (!s)
      return qi::makeFutureError<unsigned int>("Cannot find signal");
    SignalBase::Link l = s->connect(subscriber);
    if (l > 0xFFFF)
      qiLogError("object") << "Signal id too big";
    return qi::Future<unsigned int>((event << 16) + l);
  }

  qi::Future<void> DynamicObject::metaDisconnect(unsigned int linkId)
  {
    unsigned int event = linkId >> 16;
    unsigned int link = linkId & 0xFFFF;
    //TODO: weird to call createSignal in disconnect
    SignalBase* s = _p->createSignal(event);
    if (!s)
      return qi::makeFutureError<void>("Cannot find local signal connection.");
    bool b = s->disconnect(link);
    if (!b) {
      return qi::makeFutureError<void>("Cannot find local signal connection.");
    }
    return qi::Future<void>(0);
  }

  class MFunctorCall
  {
  public:
    MFunctorCall(GenericFunction& func, GenericFunctionParameters& params,
       qi::Promise<GenericValuePtr>* out, bool noCloneFirst)
    : noCloneFirst(noCloneFirst)
    {
      this->out = out;
      std::swap(this->func, func);
      std::swap((std::vector<GenericValuePtr>&) params,
        (std::vector<GenericValuePtr>&) this->params);
    }
    MFunctorCall(const MFunctorCall& b)
    {
      (*this) = b;
    }
    void operator = (const MFunctorCall& b)
    {
      std::swap( (std::vector<GenericValuePtr>&) params,
        (std::vector<GenericValuePtr>&) b.params);
      std::swap(func, const_cast<MFunctorCall&>(b).func);
      this->out = b.out;
      noCloneFirst = b.noCloneFirst;
    }
    void operator()()
    {
      try
      {
        out->setValue(func.call(params));
      }
      catch(const std::exception& e)
      {
        out->setError(e.what());
      }
      catch(...)
      {
        out->setError("Unknown exception caught.");
      }
      params.destroy(noCloneFirst);
      delete out;
    }
    qi::Promise<GenericValuePtr>* out;
    GenericFunctionParameters params;
    GenericFunction func;
    bool noCloneFirst;
  };
  qi::Future<GenericValuePtr> metaCall(EventLoop* el,
    GenericFunction func, const GenericFunctionParameters& params, MetaCallType callType, bool noCloneFirst)
  {
    bool synchronous = true;
    switch (callType)
    {
    case qi::MetaCallType_Direct:
      break;
    case qi::MetaCallType_Auto:
      synchronous = !el ||  el->isInEventLoopThread();
      break;
    case qi::MetaCallType_Queued:
      synchronous = !el;
      break;
    }
    if (synchronous)
    {
      qi::Promise<GenericValuePtr> out;
      out.setValue(func.call(params));
      return out.future();
    }
    else
    {
      qi::Promise<GenericValuePtr>* out = new qi::Promise<GenericValuePtr>();
      GenericFunctionParameters pCopy = params.copy(noCloneFirst);
      qi::Future<GenericValuePtr> result = out->future();
      el->post(MFunctorCall(func, pCopy, out, noCloneFirst));
      return result;
    }
  }

  //DynamicObjectType implementation: just bounces everything to metaobject

  const MetaObject& DynamicObjectType::metaObject(void* instance)
  {
    return reinterpret_cast<DynamicObject*>(instance)->metaObject();
  }

  qi::Future<GenericValuePtr> DynamicObjectType::metaCall(void* instance, unsigned int method, const GenericFunctionParameters& params, MetaCallType callType)
  {
    return reinterpret_cast<DynamicObject*>(instance)
      ->metaCall(method, params, callType);
  }

  void DynamicObjectType::metaPost(void* instance, unsigned int signal, const GenericFunctionParameters& params)
  {
    reinterpret_cast<DynamicObject*>(instance)
      ->metaPost(signal, params);
  }

  qi::Future<unsigned int> DynamicObjectType::connect(void* instance, unsigned int event, const SignalSubscriber& subscriber)
  {
    return reinterpret_cast<DynamicObject*>(instance)
      ->metaConnect(event, subscriber);
  }

  qi::Future<void> DynamicObjectType::disconnect(void* instance, unsigned int linkId)
  {
    return reinterpret_cast<DynamicObject*>(instance)
      ->metaDisconnect(linkId);
  }

  const std::vector<std::pair<Type*, int> >& DynamicObjectType::parentTypes()
  {
    static std::vector<std::pair<Type*, int> > empty;
    return empty;
  }

  Manageable* DynamicObjectType::manageable(void* instance)
  {
    return reinterpret_cast<DynamicObject*>(instance);
  }

  static void cleanupDynamicObject(GenericObject *obj) {
    delete reinterpret_cast<DynamicObject*>(obj->value);
    delete obj;
  }

  ObjectPtr     makeDynamicObjectPtr(DynamicObject *obj, bool destroyObject)
  {
    ObjectPtr op;
    static DynamicObjectType* type = new DynamicObjectType();
    if (destroyObject)
      op = ObjectPtr(new GenericObject(type, obj), &cleanupDynamicObject);
    else
      op = ObjectPtr(new GenericObject(type, obj));
    return op;
  }

}
