#include "ac_tee_connect.hpp"
#include <iostream>

namespace tbone::client {

//==============================================================================

TeeLocalConnection::TeeLocalConnection(const std::string address) {
  _socket = NULL;
  if (NULL != (_socket = new bstlocal::stream_protocol::socket(TeeIOContext::get()->getIOContext()))) {
    _socket->connect(bstlocal::stream_protocol::endpoint(address));
  }
}

TeeLocalConnection::~TeeLocalConnection() {
  if (NULL != _socket) {
    // Avoid exception, don't care about error => descriptor closed at the end
    bstsys::error_code e;
    _socket->shutdown(bstlocal::stream_protocol::socket::shutdown_both, e);
    _socket->close(e);
    delete _socket;
    _socket = NULL;
  }
}

//==============================================================================

} // namespace tbone::client

// EOF
