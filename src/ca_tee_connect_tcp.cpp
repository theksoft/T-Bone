#include "ca_tee_connect.hpp"
#include "ca_tee_connect.hxx"

namespace tbone::client {

//==============================================================================

TeeTcpConnection::TeeTcpConnection(const std::string address, int port)
  : TTeeConnection() {
  if (NULL != (_socket = new TcpSocket(TeeIOContext::get()->getIOContext()))) {
    _socket->connect(TcpEndPoint(bip::address::from_string(address), port));
  }
}

TeeTcpConnection::~TeeTcpConnection() {
  if (NULL != _socket) {
    // Avoid exception, don't care about error => descriptor closed at the end
    bsys::error_code e;
    _socket->shutdown(TcpSocket::shutdown_both, e);
    _socket->close(e);
    delete _socket;
  }
}

//==============================================================================

} // namespace tbone::client

// EOF
