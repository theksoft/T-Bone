#include "tb_messages.hpp"
#include <algorithm>
#include <cassert>

namespace tbone {

#define TB_MESSAGE_TAG32    0x60d1c53c
#define TB_MESSAGE_HDR32    2

struct sTBMessageHeader {
  TBMessage::Type type;
  const char *banner;
} TBMessageHeaders[] = {
  { TBMessage::COMMAND,  TB_MSG_COMMAND },
  { TBMessage::OPEN,     TB_MSG_OPEN },
  { TBMessage::CLOSE,    TB_MSG_CLOSE },
  { TBMessage::HELLO,    TB_MSG_HELLO },
  { TBMessage::WELCOME,  TB_MSG_WELCOME },
  { TBMessage::BYE,      TB_MSG_BYE },
  { TBMessage::FAREWELL, TB_MSG_FAREWELL },
  { TBMessage::UNKNOWN,  NULL }
};

//==============================================================================

template void HexIntToString<>(std::string& str, uintptr_t hexInt);
template void HexIntToString<>(std::string& str, uint32_t hexInt);

//==============================================================================

TBMessage::TBMessage(TBMessage&& message)
  : _error(message._error), _size(message._size),
    _message(message._message), _current(message._current) {
  message._message = NULL;
  message.clear();
}

void TBMessage::clear() {
  _error = true;
  _size = 0;
  _current = NULL;
  if (_message) {
    delete [] reinterpret_cast<uint8_t*>(_message);
    _message = NULL;
  }
}

size_t TBMessage::prepare(size_t size) {
  assert(!_message && !_current && !_size);
  if (size) {
    uint32_t sizeMessage = size + sizeof(uint32_t)*TB_MESSAGE_HDR32;
    _message = new uint8_t[sizeMessage];
    if (_message) {
      _size = sizeMessage;
      rewind();
      writeU32(TB_MESSAGE_TAG32);
      writeU32(static_cast<uint32_t>(size));
      _error = false;
      return size;
    }
  }
  return 0;
}

size_t TBMessage::prepare(const void *buffer, size_t size) {
  assert(buffer);
  if (size > sizeof(uint32_t)*TB_MESSAGE_HDR32) {
    const uint32_t *p = reinterpret_cast<const uint32_t*>(buffer);
    uint32_t tag = readTBMessageInteger<uint32_t>(p++);
    if (TB_MESSAGE_TAG32 == tag) {
      size_t sizeMessage = static_cast<size_t>(readTBMessageInteger<uint32_t>(p++));
      if (prepare(sizeMessage) == sizeMessage) {
        return append(p, size - sizeof(uint32_t)*TB_MESSAGE_HDR32);
      }
    }
  }
  _error = true;
  return 0;
}

size_t TBMessage::append(const void *buffer, size_t size) {
  if (_error) return 0;
  assert(_message && _current);
  if (getRemain() >= size) {
    std::memcpy(_current, buffer, size);
    _current = reinterpret_cast<uint8_t*>(_current) + size;
    return getRemain();
  }
  _error = true;
  return 0;
}

bool TBMessage::generateFrom(const std::string& str) {
  if (str.length()) {
    if (prepare(str.length()) == str.length()) {
      return (0 == append(str) && !_error);
    }
  }
  _error = true;
  return false;
}

bool TBMessage::assignTo(std::string& str) {
  str.clear();
  if (_size > sizeof(uint32_t)*TB_MESSAGE_HDR32) {
    str.assign(reinterpret_cast<const char*>(getPayload()), getPayloadSize());
    return true;
  }
  return false;
}

size_t TBMessage::fill(void *buffer, size_t offset, size_t size) {
  if (_error) return false;
  assert(buffer);
  assert(_message);
  size_t sizeAtOffset = getPosition() - offset;
  if (sizeAtOffset > 0) {
    size_t sizeToFill = std::min(sizeAtOffset, size);
    uint8_t *p = reinterpret_cast<uint8_t*>(_message) + offset;
    std::memcpy(buffer, p, sizeToFill);
    return sizeToFill;
  }
  return 0;
}

const void* TBMessage::getPayload() const {
  return reinterpret_cast<uint32_t*>(_message) + TB_MESSAGE_HDR32;
}

size_t TBMessage::getPayloadSize() const {
  return _size - sizeof(uint32_t)*TB_MESSAGE_HDR32;
}

TBMessage::Type TBMessage::getType() const {
  assert(_message);
  const char *p = reinterpret_cast<const char*>(getPayload());
  for (int i = 0; TBMessageHeaders[i].banner; i++) {
    if (0 == std::strncmp(p, TBMessageHeaders[i].banner, std::strlen(TBMessageHeaders[i].banner))) {
      return TBMessageHeaders[i].type;
    }
  }
  return UNKNOWN;
}

//==============================================================================

} // namespace tbone

// EOF
