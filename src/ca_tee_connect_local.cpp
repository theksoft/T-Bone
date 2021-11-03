#include "ca_tee_connect.hpp"
#include "ca_tee_connect.hxx"

namespace tbone::client {

//==============================================================================

TeeLocalConnection::TeeLocalConnection(const std::string address)
  : TTeeConnection() {
  if (NULL != (_socket = new LocalSocket(TeeIOContext::get()->getIOContext()))) {
    _socket->connect(LocalEndPoint(address));
  }
}

TeeLocalConnection::~TeeLocalConnection() {
  if (NULL != _socket) {
    // Avoid exception, don't care about error => descriptor closed at the end
    bsys::error_code e;
    _socket->shutdown(LocalSocket::shutdown_both, e);
    _socket->close(e);
    delete _socket;
  }
}

//==============================================================================

} // namespace tbone::client

// EOF
