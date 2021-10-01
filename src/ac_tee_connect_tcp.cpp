#include "ac_tee_connect.hpp"

#include <iostream>

namespace tbone::client {

//==============================================================================

TeeTcpConnection::TeeTcpConnection(const std::string address, int port) {
  _socket = NULL;
  if (NULL != (_socket = new bstip::tcp::socket(TeeIOContext::get()->getIOContext()))) {
    _socket->connect(bstip::tcp::endpoint(bstip::address::from_string(address), port));
  }
}

TeeTcpConnection::~TeeTcpConnection() {
  if (NULL != _socket) {
    // Avoid exception, don't care about error => descriptor closed at the end
    bstsys::error_code e;
    _socket->shutdown(bstip::tcp::socket::shutdown_both, e);
    _socket->close(e);
    delete _socket;
    _socket = NULL;
  }
}

//==============================================================================

} // namespace tbone::client

// EOF
