//==============================================================================
// Asynchronous application server.
// Allows to define same behavior for local and TCP service.
//==============================================================================

#ifndef TS_SERVER_APP_HXX
#define TS_SERVER_APP_HXX

#include "tb_messages.hpp"
#include "tb_network.hxx"
#include <iostream>
#include <memory>
#include <string>
#include <utility>

namespace tbone::server {

//==============================================================================

extern uint32_t getClientID();

//==============================================================================

// S template parameter is the socket class type.
//------------------------------------------------------------------------------

template <class S>
class AppHandler : public std::enable_shared_from_this<AppHandler<S>> {

  enum { max_length = TB_WORK_SIZE };

public:
  AppHandler(S socket)
    : _socket(std::move(socket)),
//      _remoteName(""), _remoteID(""), _localID(0),
      _rInProgress(false)
  {}
  virtual ~AppHandler() {}

  void handle() {
    auto self(this->shared_from_this());
    _socket.async_read_some(
      bstnet::buffer(_rData, max_length),
      [this, self](bstsys::error_code error, std::size_t length) {
        if (!error) {
          // Manage reception
          if (!_rInProgress) {
            _rMessage.clear();
            _rMessage.prepare(_rData, length);
          } else {
            _rMessage.append(_rData, length);
          }
          _rInProgress = true;
          // Process reception if complete
          if (!_rMessage.hasError()) {
            if (_rMessage.isComplete()) {
              _rInProgress = false;
              TBMessage m(std::move(_rMessage));
              process(m);
            }
          } else {
            _rMessage.clear();
            _rInProgress = false;
            // TODO Replace both with log
            std::cerr << "--- ERROR data reception" << std::endl;
          }
          // Next reception
          handle();
        } else if (error == bstnet::error::eof) {
          // TODO Replace with log
          std::cout << "--- Connection closed!" << std::endl;
        } else if (error) {
          // TODO Replace with log
          throw bstsys::system_error(error); // Some other error.
        }
      }
    );
  }

protected:

  void process(TBMessage& message) {
    std::string s;
    switch (message.getType()) {
    case TBMessage::COMMAND:
    case TBMessage::OPEN:
    case TBMessage::CLOSE:
      std::cerr << "--- UNSUPPORTED MESSAGE RECEIVED!" << std::endl;
      break;
    case TBMessage::HELLO:
      message.assignTo(s);
      welcome(s);
      break;
    case TBMessage::BYE:
      message.assignTo(s);
      farewell(s);
      break;
    case TBMessage::WELCOME:
    case TBMessage::FAREWELL:
      // TODO Replace with log
      std::cerr << "--- UNEXPECTED MESSAGE RECEIVED!" << std::endl;
      break;
    case TBMessage::UNKNOWN:
    default:
      // TODO Replace with log
      std::cerr << "--- UNKNOWN MESSAGE RECEIVED!" << std::endl;
      break;
    }
  }

  void welcome(std::string& str) {
    TBMessageHello hello(str);
    if (hello.parse()) {
      uint32_t id = getClientID();
#if 0
      _remoteID = hello.getClientContextID();
      _remoteName = hello.getClientTeeName();
      _localID = getClientID();
#endif
      TBMessageWelcome welcome;
      welcome.build(id);
      TBMessage msg;
      msg.generateFrom(welcome.getMessage());
      bstsys::error_code error;
      bstnet::write(_socket, bstnet::buffer(msg.getMessage(), msg.getSize()), error);
      if (error) {
      // TODO Replace with log
        std::cerr << "--- ERROR Sending welcome message failed!" << std::endl;
      }
    }
  }

  void farewell(std::string& str) {
    (void)str;
  }

private:
  S _socket;

#if 0
  std::string _remoteName, _remoteID;
  uint32_t _localID;
#endif

  bool _rInProgress;
  TBMessage _rMessage;
  char _rData[max_length];
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
          std::make_shared<AppHandler<S>>(std::move(socket))->handle();
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
