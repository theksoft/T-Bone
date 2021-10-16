//==============================================================================
// Asynchronous application server.
// Allows to define same behavior for local and TCP service.
//==============================================================================

#ifndef TS_SERVER_APP_HXX
#define TS_SERVER_APP_HXX

#include "tb_network.hxx"
#include <memory>
#include <utility>
#include <iostream>

namespace tbone::server {

//==============================================================================

const int max_length = 1024;  // TODO Temporary for dummy reception

// S template parameter is the socket class type.
//------------------------------------------------------------------------------

template <class S>
class AppHandler : public std::enable_shared_from_this<AppHandler<S>> {

public:
  AppHandler(S socket) : _socket(std::move(socket)) {}
  virtual ~AppHandler() {}

  void start() {
    auto self(this->shared_from_this());
    // TODO Temporary dummy reception - to detect connection close
    _socket.async_read_some(
      bstnet::buffer(_data, max_length),
      [this, self](bstsys::error_code error, std::size_t length) {
        (void)length;
        if (error == bstnet::error::eof) {
          std::cout << "--- connection closed!" << std::endl; // TODO To be removed
        }
        else if (error) {
          throw bstsys::system_error(error); // Some other error.
        }
      }
    );
  }

private:
  S _socket;
  char _data[max_length]; // TODO Temporary for dummy reception
};

// T template parameter is the the acceptor class type.
// S template parameter is the socket class type.
// It must have the following traits: endpoint_type, protocol_type
//------------------------------------------------------------------------------

template <class S, class T>
class AppServer {
public:
  AppServer(bstnet::io_context& ioContext, const typename T::endpoint_type& endpoint)
    : _acceptor(ioContext, endpoint)
  {
    accept();
  }

  virtual ~AppServer() {}

  void accept() {
    _acceptor.async_accept(
      [this]( bstsys::error_code error, S socket) {
        if (!error) {
          std::cout << "--- connection accepted!" << std::endl; // TODO To be removed
          std::make_shared<AppHandler<S>>(std::move(socket))->start();
        }
        accept();
      }
    );
  }

private:
  T _acceptor;
};

//==============================================================================
// Explicit instanciation references
//==============================================================================

extern template
class AppServer<
  bstlocal::stream_protocol::socket,
  bstlocal::stream_protocol::acceptor
>;

extern template 
class AppServer<
  bstip::tcp::socket,
  bstip::tcp::acceptor
>;

typedef AppServer<
  bstlocal::stream_protocol::socket,
  bstlocal::stream_protocol::acceptor
> AppLocalServer;

typedef AppServer<
  bstip::tcp::socket,
  bstip::tcp::acceptor
> AppTcpServer;

//==============================================================================

} // namespace tbone::server

#endif// TS_SERVER_APP_HXX

// End of file
