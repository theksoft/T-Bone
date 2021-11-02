#include "tb_messages.hpp"
#include <cassert>

namespace tbone {

//==============================================================================

TBMessageAcknowledge::TBMessageAcknowledge(const char *banner)
  : TBMessageString(), _id(0), _banner(banner) {}

TBMessageAcknowledge::TBMessageAcknowledge(const char *banner, std::string& str)
  : TBMessageString(str), _id(0), _banner(banner) {}

TBMessageAcknowledge::~TBMessageAcknowledge() {
  _banner.clear();
}

void TBMessageAcknowledge::clear() {
  TBMessageString::clear();
  _id = 0;
}

bool TBMessageAcknowledge::build(uint32_t id) {
  assert(id);
  clear();
  _id = id;
  std::string sid;
  HexIntToString(sid, _id);
  size_t lg = _banner.length() + sid.length() + /*reserve*/10;
  _message.reserve(lg);
  _message = _banner;
  _message += sid;
  return true;
}

bool TBMessageAcknowledge::parse() {
  bool vRtn = false;
  size_t pos = _message.find(TB_MSG_TAG);
  if (std::string::npos != pos && 0 == _message.compare(0, pos+1, _banner)) {
    try {
      _id = std::stoul(_message.substr(pos+1), NULL, 16);
      vRtn = true;
    } catch(...) {
      _id = 0;
    }
  }
  return vRtn;
}

//==============================================================================

} // namespace tbone

//EOF
