/*
  T-Bone implementation.
  This file defines all messages exchange between TEE server and client applications.
*/

#include <boost/endian/conversion.hpp>
#include <iomanip>
#include <sstream>
#include <string>

#ifndef TB_MESSAGES_HPP
#define TB_MESSAGES_HPP

#define TB_WORK_SIZE        2048

#define TB_MSG_TAG          '#'
#define TB_MSG_HELLO        "HELLO#"
#define TB_MSG_WELCOME      "WELCOME#"
#define TB_MSG_BYE          "BYE#"
#define TB_MSG_FAREWELL     "FAREWELL#"
#define TB_MSG_OPEN         "OPEN#"
#define TB_MSG_CLOSE        "CLOSE#"
#define TB_MSG_COMMAND      "COMMAND#"
#define TB_MSG_REJECTED     "REJECTED#"

namespace tbone {

//==============================================================================

template <typename T>
void HexIntToString(std::string& str, T hexInt) {
  str.clear();
  std::stringstream ss;
  ss << std::setfill ('0')
     << std::setw(sizeof(T)*2) 
     << std::hex
     << hexInt;
  str = ss.str();
}

extern template void HexIntToString<>(std::string& str, uintptr_t hexInt);
extern template void HexIntToString<>(std::string& str, uint32_t hexInt);

//==============================================================================

template <typename T>
void writeTBMessageInteger(void *buffer, T data) {
  T *p = reinterpret_cast<T*>(buffer);
  *p = boost::endian::native_to_big(data);
}

template <typename T>
T readTBMessageInteger(const void *buffer) {
  return boost::endian::big_to_native(*(reinterpret_cast<const T*>(buffer)));
}

//==============================================================================

class TBMessage {
public:

  typedef enum {
    UNKNOWN,
    HELLO, WELCOME,
    BYE, FAREWELL,
    OPEN, CLOSE, COMMAND,
    REJECTED
  } Type;

  TBMessage() : _error(true), _size(0), _message(NULL), _current(NULL) {}
  TBMessage(TBMessage&& message);
  virtual ~TBMessage() { clear(); }
  virtual void clear();

  bool generateFrom(const std::string& str);
  bool assignTo(std::string& str);
  size_t prepare(size_t size);
  size_t prepare(const void *buffer, size_t size);
  size_t append(const void *buffer, size_t size);
  size_t append(const std::string& str) { return append(str.c_str(), str.length()); }
  size_t fill(void *buffer, size_t offset, size_t size);
  bool isComplete() const               { return getPosition() == _size; }
  bool hasError() const                 { return _error; }
  size_t getSize() const                { return _size; }
  const void* getMessage() const        { return _message; }
  inline const void* getPayload() const;
  inline size_t getPayloadSize() const;
  Type getType() const;

protected:
  void rewind()                         { _current = _message; }
  size_t getRemain() const              { return _size - getPosition(); }
  size_t getPosition() const {
    return static_cast<size_t>(
      reinterpret_cast<uintptr_t>(_current) - reinterpret_cast<uintptr_t>(_message)
    );
  }
  void writeU32(uint32_t u32) {
    writeTBMessageInteger(_current, u32);
    _current = reinterpret_cast<uint32_t*>(_current) + 1;
  }
  uint32_t readU32() {
    uint32_t u32 = readTBMessageInteger<uint32_t>(_current);
    _current = reinterpret_cast<uint32_t*>(_current) + 1;
    return u32;
  }
protected:
  bool _error;
  size_t _size;
  void *_message;
  void *_current;
};

//==============================================================================

class TBMessageString {
public:
  TBMessageString() : _message("") {}
  TBMessageString(std::string& str) : _message(std::move(str)) {}
  virtual ~TBMessageString()            { clear(); }
  virtual void clear()                  { _message.clear(); }
  const std::string& getMessage() const { return _message; }
protected:
  std::string _message;
};

//==============================================================================

class TBMessageRequest : public TBMessageString {
public:
  TBMessageRequest(const char *banner);
  TBMessageRequest(const char *banner, std::string& str);
  virtual ~TBMessageRequest();
  virtual void clear();
  bool build(uintptr_t id, std::string& name);
  bool parse();
  const std::string& getClientContextID() const { return _id; }
  const std::string& getClientTeeName() const   { return _name; }
private:
  std::string _id, _name;
protected:
  std::string _banner;
};

//==============================================================================

class TBMessageAcknowledge : public TBMessageString {
public:
  TBMessageAcknowledge(const char *banner);
  TBMessageAcknowledge(const char *banner, std::string& str);
  virtual ~TBMessageAcknowledge();
  virtual void clear();
  bool build(uint32_t id);
  bool parse();
  uint32_t getClientPairID() const { return _id; }
private:
  uint32_t _id;
protected:
  std::string _banner;
};

//==============================================================================

class TBMessageHello : public TBMessageRequest {
public:
  TBMessageHello() : TBMessageRequest(TB_MSG_HELLO) {}
  TBMessageHello(std::string& str) : TBMessageRequest(TB_MSG_HELLO, str) {}
};

//==============================================================================

class TBMessageBye : public TBMessageRequest {
public:
  TBMessageBye() : TBMessageRequest(TB_MSG_BYE) {}
  TBMessageBye(std::string& str) : TBMessageRequest(TB_MSG_BYE, str) {}
};

//==============================================================================

class TBMessageWelcome : public TBMessageAcknowledge {
public:
  TBMessageWelcome() : TBMessageAcknowledge(TB_MSG_WELCOME) {}
  TBMessageWelcome(std::string& str) : TBMessageAcknowledge(TB_MSG_WELCOME, str) {}
};

//==============================================================================

class TBMessageFarewell : public TBMessageAcknowledge {
public:
  TBMessageFarewell() : TBMessageAcknowledge(TB_MSG_FAREWELL) {}
  TBMessageFarewell(std::string& str) : TBMessageAcknowledge(TB_MSG_FAREWELL, str) {}
};

//==============================================================================

class TBMessageRejected : public TBMessageAcknowledge {
public:
  TBMessageRejected() : TBMessageAcknowledge(TB_MSG_REJECTED) {}
  TBMessageRejected(std::string& str) : TBMessageAcknowledge(TB_MSG_REJECTED, str) {}
};

//==============================================================================

} // namespace tbone

#endif  // TB_ERRORS_HPP

//EOF
