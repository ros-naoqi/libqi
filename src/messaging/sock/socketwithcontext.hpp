#pragma once
#ifndef _QI_SOCK_SOCKETWITHCONTEXT_HPP
#define _QI_SOCK_SOCKETWITHCONTEXT_HPP
#include <ka/src.hpp>
#include <ka/mutablestore.hpp>
#include <boost/asio/io_context.hpp>
#include "traits.hpp"
#include "sslcontextptr.hpp"

#if BOOST_VERSION >= 107000
#define GET_IO_SERVICE(s) ((boost::asio::io_context&)(s).get_executor().context())
#else
#define GET_IO_SERVICE(s) ((s).get_io_service())
#endif

namespace qi { namespace sock {

  /// Socket bound to an ssl context.
  ///
  /// The purpose is to ensure that the ssl context has the same lifetime as
  /// the socket.
  ///
  /// Network N
  template<typename N>
  class SocketWithContext
  {
    using socket_t = SslSocket<N>;
    using io_service_t = IoService<N>;
    using io_context_t = IoContext<N>;

    SslContextPtr<N> context;
    socket_t socket;

  public:
  // NetSslSocket:
    using handshake_type = HandshakeSide<socket_t>;
    using lowest_layer_type = Lowest<socket_t>;
    using next_layer_type = typename socket_t::next_layer_type;
    using executor_type = typename socket_t::executor_type;

    SocketWithContext(io_service_t& io, const SslContextPtr<N>& ctx)
      : context(ctx)
      , socket(io, *ctx)
    {
    }

    io_service_t& get_io_service()
    {
      return GET_IO_SERVICE(socket);
    }

    executor_type get_executor()
    {
      return socket.get_executor();
    }

    void set_verify_mode(decltype(N::sslVerifyNone()) x)
    {
      socket.set_verify_mode(x);
    }

    template<typename H>
    void async_handshake(handshake_type x, H h)
    {
      socket.async_handshake(x, h);
    }

    lowest_layer_type& lowest_layer()
    {
      return socket.lowest_layer();
    }

    next_layer_type& next_layer()
    {
      return socket.next_layer();
    }

  // Custom:
    template<typename T, typename U>
    void async_read_some(const T& buffers, const U& handler)
    {
      socket.async_read_some(buffers, handler);
    }

    template<typename T, typename U>
    void async_write_some(const T& buffers, const U& handler)
    {
      socket.async_write_some(buffers, handler);
    }
  };
}} // namespace qi::sock

#endif // _QI_SOCK_SOCKETWITHCONTEXT_HPP
