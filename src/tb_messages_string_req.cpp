#include "tb_messages.hpp"
#include <cassert>

namespace tbone {

//==============================================================================

TBMessageRequest::TBMessageRequest(const char *banner)
  : TBMessageString(), _id(""), _name(""), _banner(banner) {}

TBMessageRequest::TBMessageRequest(const char *banner, std::string& str)
  : TBMessageString(str), _id(""), _name(""), _banner(banner) {}

TBMessageRequest::~TBMessageRequest() {
  _banner.clear();
}

void TBMessageRequest::clear() {
  TBMessageString::clear();
  _id.clear();
  _name.clear();
}

bool TBMessageRequest::build(uintptr_t id, std::string& name) {
  assert(id);
  clear();
  HexIntToString(_id, id);
  _name = name;
  size_t lg = _banner.length() + _id.length() + _name.length() + /*reserve*/10;
  _message.reserve(lg);
  _message = _banner;
  _message += _id;
  _message += TB_MSG_TAG;
  _message += _name;
  return true;
}

bool TBMessageRequest::parse() {
  bool vRtn = false;
  size_t pos = _message.find(TB_MSG_TAG);
  size_t pos2 = _message.rfind(TB_MSG_TAG);
  if ( std::string::npos != pos
    && std::string::npos != pos2
    && 0 == _message.compare(0, pos+1, _banner)) {
    _id = _message.substr(pos+1, pos2 - (pos+1));
    _name = _message.substr(pos2+1);
    if (_id.length() && _name.length()) {
      vRtn = true;
    }
  }
  return vRtn;
}

//==============================================================================

} // namespace tbone

//EOF
