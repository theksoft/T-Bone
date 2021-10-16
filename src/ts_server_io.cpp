#include "ts_server.hpp"

namespace tbone::server {

//==============================================================================

TeeIOContext *TeeIOContext::_theIOContext = NULL;

//==============================================================================

TeeIOContext::TeeIOContext()
: _ioContext(),
  _signals(_ioContext, SIGINT, SIGTERM) {
  _signals.async_wait([&](auto, auto) { _ioContext.stop(); });
}

TeeIOContext::~TeeIOContext() {
  // Not intended to be called
  _theIOContext = NULL;
}

//==============================================================================

TeeIOContext* TeeIOContext::get() {
  if (NULL == _theIOContext) {
    _theIOContext = new TeeIOContext;
  }
  return _theIOContext;
}

//==============================================================================

} // namespace tbone::server

// EOF
