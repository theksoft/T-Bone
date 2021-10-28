#include "ts_server_app.hxx"

namespace tbone::server {

//==============================================================================

uint32_t getClientID() {
  static uint32_t count = 0;
  return ++count;
}

//==============================================================================

template
class AppServer<
  bstlocal::stream_protocol::socket,
  bstlocal::stream_protocol::acceptor
>;

template 
class AppServer<
  bstip::tcp::socket,
  bstip::tcp::acceptor
>;

//==============================================================================

} // namespace tbone::server

// EOF
