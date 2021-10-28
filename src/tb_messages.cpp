#include "tb_messages.hpp"
#include <cassert>
#include <cstring>
#include <iomanip>
#include <sstream>

namespace tbone {

#define TB_MSG_TAG          '#'
#define TB_MSG_HELLO        "HELLO TEE#"
#define TB_MSG_WELCOME      "WELCOME APP#"
#define TB_MSG_BYE          "BYE TEE#"
#define TB_MSG_FAREWELL     "FAREWELL APP#"
#define TB_MSG_OPEN         "OPEN#"
#define TB_MSG_CLOSE        "CLOSE#"
#define TB_MSG_COMMAND      "COMMAND#"


//==============================================================================

template<typename T>
void HexIntToString(std::string& str, T hexInt) {
  str.clear();
  std::stringstream ss;
  ss << std::setfill ('0')
     << std::setw(sizeof(T)*2) 
     << std::hex
     << hexInt;
  str = ss.str();
}

//==============================================================================

MsgRequest::MsgRequest()
  : _message(""), _id(""), _name(""), _banner("") {}

MsgRequest::~MsgRequest() {
  clear();
  _banner.clear();
}

void MsgRequest::clear() {
  _message.clear();
  _id.clear();
  _name.clear();
}

bool MsgRequest::build(uintptr_t id, std::string& name) {
  clear();
  assert(id);

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

bool MsgRequest::parse(std::string& msg) {
  clear();
  _message = msg;
  return parse();
}

bool MsgRequest::parse(const char *msg, size_t length) {
  clear();
  assert(msg && length);
  _message.assign(msg, length);
  return parse();
}

bool MsgRequest::parse() {
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

MsgHello::MsgHello()
  : MsgRequest() {
    _banner = TB_MSG_HELLO;
}

MsgHello::~MsgHello() {}

//==============================================================================

MsgBye::MsgBye()
  : MsgRequest() {
    _banner = TB_MSG_BYE;
}

MsgBye::~MsgBye() {}

//==============================================================================

MsgAcknowledge::MsgAcknowledge()
  : _message(""), _id(0), _banner("") {}

MsgAcknowledge::~MsgAcknowledge() {
  clear();
}

void MsgAcknowledge::clear() {
  _message.clear();
  _id = 0;
}

bool MsgAcknowledge::build(uint32_t id) {
  clear();
  assert(id);

  _id = id;
  std::string sid;
  HexIntToString(sid, _id);

  size_t lg = _banner.length() + sid.length() + /*reserve*/10;
  _message.reserve(lg);

  _message = _banner;
  _message += sid;
  return true;
}

bool MsgAcknowledge::parse(std::string& msg) {
  clear();
  _message = msg;
  return parse();
}

bool MsgAcknowledge::parse(const char *msg, size_t length) {
  clear();
  assert(msg && length);
  _message.assign(msg, length);
  return parse();
}

bool MsgAcknowledge::parse() {
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

MsgWelcome::MsgWelcome()
  : MsgAcknowledge() {
    _banner = TB_MSG_WELCOME;
}

MsgWelcome::~MsgWelcome() {}

//==============================================================================

MsgFarewell::MsgFarewell()
  : MsgAcknowledge() {
    _banner = TB_MSG_FAREWELL;
}

MsgFarewell::~MsgFarewell() {}

//==============================================================================

} // namespace tbone

// EOF
