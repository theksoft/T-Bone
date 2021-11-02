//==============================================================================
// Asynchronous application server.
// Allows to define same behavior for local and TCP service.
//==============================================================================

#ifndef TS_SERVER_APP_HXX
#define TS_SERVER_APP_HXX

#include "tb_messages.hpp"
#include "tb_network.hxx"
#include "ts_context.hpp"
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
      bio::buffer(_rData, max_length),
      [this, self](bsys::error_code error, std::size_t length) {
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
        } else if (error == bio::error::eof) {
          // TODO Replace with log
          std::cout << "--- Connection closed!" << std::endl;
        } else if (error) {
          // TODO Replace with log
          std::cerr << "--- ERROR Connection issue! " << error.message() << std::endl;
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
      if(NULL != _contexts.add(id, hello.getClientContextID(), hello.getClientTeeName())) {
        TBMessageWelcome welcome;
        welcome.build(id);
        TBMessage msg;
        msg.generateFrom(welcome.getMessage());
        bsys::error_code error;
        bio::write(_socket, bio::buffer(msg.getMessage(), msg.getSize()), error);
        if (!error) {
          // TODO Replace with log
          std::cout << "------ TEE context created #" << id << std::endl;
        } else {
          // TODO Replace with log
          std::cerr << "--- ERROR Sending welcome message failed!" << std::endl;
        }
      } else {
        TBMessageRejected rejected;
        rejected.build(id);
        TBMessage msg;
        msg.generateFrom(rejected.getMessage());
        // Don't care about error management!
        bio::write(_socket, bio::buffer(msg.getMessage(), msg.getSize()));
        // TODO Replace with log
        std::cerr << "--- ERROR Client context already present!" << std::endl;
      }
    }
  }

  void farewell(std::string& str) {
    TBMessageBye bye(str);
    if (bye.parse()) {
      uint32_t id = _contexts.remove(bye.getClientContextID());
      if (id) {
        TBMessageFarewell farewell;
        farewell.build(id);
        TBMessage msg;
        msg.generateFrom(farewell.getMessage());
        bsys::error_code error;
        bio::write(_socket, bio::buffer(msg.getMessage(), msg.getSize()), error);
        if (!error) {
          // TODO Replace with log
          std::cout << "------ TEE context remove #" << id << std::endl;
        } else {
          // TODO Replace with log
          std::cerr << "--- ERROR Sending farewell message failed!" << std::endl;
        }
      } else {
        TBMessageRejected rejected;
        rejected.build(id);
        TBMessage msg;
        msg.generateFrom(rejected.getMessage());
        // Don't care about error management!
        bio::write(_socket, bio::buffer(msg.getMessage(), msg.getSize()));
        // TODO Replace with log
        std::cerr << "--- ERROR Client context cannot be removed!" << std::endl;
      }
    }
  }

private:
  S _socket;

  bool _rInProgress;
  TBMessage _rMessage;
  char _rData[max_length];

  ContextMap _contexts;
};

// T template parameter is the the acceptor class type.
// S template parameter is the socket class type.
// It must have the following traits: endpoint_type, protocol_type
//------------------------------------------------------------------------------

template <class S, class T>
class AppServer {
public:
  AppServer(IOContext& ioContext, const typename T::endpoint_type& endpoint)
    : _acceptor(ioContext, endpoint)
  {
    accept();
  }

  virtual ~AppServer() {}

  void accept() {
    _acceptor.async_accept(
      [this]( bsys::error_code error, S socket) {
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

extern template class AppServer<LocalSocket,LocalAcceptor>;
extern template class AppServer<TcpSocket, TcpAcceptor>;

typedef AppServer<LocalSocket, LocalAcceptor> AppLocalServer;
typedef AppServer<TcpSocket, TcpAcceptor> AppTcpServer;

//==============================================================================

} // namespace tbone::server

#endif// TS_SERVER_APP_HXX

// End of file
