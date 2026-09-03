#pragma once
// Backward-compatible Boost.Asio aliases.
// Boost 1.66 renamed io_service -> io_context and work -> executor_work_guard;
// the deprecated io_service / io_service::work were removed in Boost 1.87.
// These aliases keep libqi building on both old and new Boost with no behaviour
// change on older Boost (the pre-1.66 branch is identical to the previous code).
#include <boost/version.hpp>
#if BOOST_VERSION >= 106600
# include <boost/asio/io_context.hpp>
# include <boost/asio/executor_work_guard.hpp>
#else
# include <boost/asio/io_service.hpp>
#endif

namespace qi
{
#if BOOST_VERSION >= 106600
  using AsioIoService = boost::asio::io_context;
  using AsioWork = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;
  inline AsioWork* newAsioWork(AsioIoService& io)
  {
    return new AsioWork(boost::asio::make_work_guard(io));
  }
#else
  using AsioIoService = boost::asio::io_service;
  using AsioWork = boost::asio::io_service::work;
  inline AsioWork* newAsioWork(AsioIoService& io) { return new AsioWork(io); }
#endif
}
