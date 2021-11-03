#include "ts_server_app.hxx"

namespace tbone::server {

//==============================================================================

uint32_t getClientID() {
  static uint32_t count = 0;
  return ++count;
}

//==============================================================================

template class AppServer<LocalSocket, LocalAcceptor>;
template class AppServer<TcpSocket, TcpAcceptor>;

//==============================================================================

} // namespace tbone::server

// EOF
