#include "ca_context.hpp"

namespace tbone::client {

//==============================================================================

TeeContextMap* TeeContextMap::_theMap = NULL;

//==============================================================================

TeeContextMap::TeeContextMap() {
  clear();
  _theMap = this;
}

TeeContextMap::~TeeContextMap() {
  reset();
  _theMap = NULL;
}

TeeContextMap& TeeContextMap::get() {
  if (!_theMap) {
    new TeeContextMap;
  }
  assert(_theMap);
  return *_theMap;
}

void TeeContextMap::reset() {
  std::lock_guard<std::recursive_mutex> lock(_guard);
  for (auto it = begin(); it != end(); it = begin()) {
    delete *it;
  }
}

TeeContext* TeeContextMap::match(TEEC_Context *context) {
  std::lock_guard<std::recursive_mutex> lock(_guard);
  for (auto it = begin(); it != end(); it++) {
    if (*(*it) == context) {
      return *it;
    }
  }
  return NULL;
}

//==============================================================================

TeeContext::TeeContext(TEEC_Context *context, Tee* tee) :
  _context(context), _tee(tee), _remoteID(0) {
  assert(_context);
  assert(_tee);
  TeeContextMap::get().add(this);
}

TeeContext::~TeeContext() {
  disconnect();
  TeeContextMap::get().remove(this);
  _tee = NULL;
  _context = NULL;
}

bool TeeContext::connect() {
  assert(_tee);
  _remoteID = _tee->connect(reinterpret_cast<Tee::Owner>(_context));
  return (0 != _remoteID);
}

void TeeContext::disconnect() {
  assert(_tee);
  _tee->disconnect(reinterpret_cast<Tee::Owner>(_context));
}

bool TeeContext::hasSessions() const {
  return false;
}

bool TeeContext::hasSharedMemoryBlocks() const {
  return false;
}

TeeContext* TeeContext::create(TEEC_Context *context, const char *name) {
  assert(context);
  assert(!TeeContextMap::get().match(context));
  Tee* tee = TeeMap::get().match(name);
  assert(tee);
  return new TeeContext(context, tee);
}

bool TeeContext::operator==(const TeeContext& other) const {
  return (_tee == other._tee && _context == other._context);
}
  
bool operator== (const TeeContext& lhs, const TEEC_Context* rhs) {
  return (lhs._context == rhs);
}

//==============================================================================

}   // namespace tbone::client

//EOF
