#include "ac_tee_connect.hpp"

namespace tbone::client {

//==============================================================================

TeeIOContext *TeeIOContext::_theIOContext = NULL;

TeeIOContext* TeeIOContext::get() {
  if (NULL == _theIOContext) {
    _theIOContext = new TeeIOContext;
  }
  return _theIOContext;
}

//==============================================================================

} // namespace tbone::client

// EOF
