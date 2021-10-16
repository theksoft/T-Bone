#include "ts_server_app.hxx"

namespace tbone::server {

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
