#include "ts_context.hpp"
#include "tb_messages.hpp"
#include <cassert>

namespace tbone::server {

//==============================================================================

void ContextMap::cleanup() {
  // There may be session command in progress
  // TODO .... for savage disconnection
}

Context* ContextMap::add(
  uint32_t localID,
  const std::string& remoteID, const std::string& remoteName) {
  WLocker locker(_guard);
  if (find(remoteID) == end()) {
    Context *ctxt = new Context(localID, remoteID, remoteName);
    if (NULL != ctxt) {
      insert(std::pair<std::string, Context*>(remoteID, ctxt));
    }
    return ctxt;
  }
  return NULL;
}

uint32_t ContextMap::remove(const std::string& remoteID) {
  WLocker locker(_guard);
  uint32_t id = 0;
  iterator it = find(remoteID);
  if (end() != it) {
    Context* c = it->second;
    if (!c->hasSessions() && !c->hasSharedMemoryBlocks()) {
      id = c->getLocalID();
      erase(it);
      delete c;
    }
  }
  return id;
}

//==============================================================================

Context::Context(
  uint32_t localID,
  const std::string& remoteID, const std::string& remoteName
) : _localID(localID), _remoteID(remoteID), _remoteName(remoteName) {}

Context::~Context() {
  cleanup();
  clear();
}

void Context::cleanup() {
  // TODO .... for savage disconnection
}

void Context::clear() {
  _localID = 0;
  _remoteID.clear();
  _remoteName.clear();
}

//==============================================================================

} // namespace tbone::server

// EOF
