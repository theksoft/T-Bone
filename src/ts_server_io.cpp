#include "ts_server.hpp"

namespace tbone::server {

//==============================================================================

TeeIOContext *TeeIOContext::_theIOContext = NULL;

TeeIOContext* TeeIOContext::get() {
  if (NULL == _theIOContext) {
    _theIOContext = new TeeIOContext;
  }
  return _theIOContext;
}

//==============================================================================

} // namespace tbone::server

// EOF
