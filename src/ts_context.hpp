#ifndef TS_CONTEXT_HPP
#define TS_CONTEXT_HPP

#include "tb_locks.hpp"
#include <string>
#include <map>

namespace tbone::server {

//==============================================================================

class ContextMap;
class Context;

//==============================================================================

class ContextMap : public std::map<std::string, Context*> {
public:
  ContextMap() {}
  ~ContextMap() { cleanup(); clear(); }
  void cleanup();

  Context* add(
    uint32_t localID,
    const std::string& remoteID, const std::string& remoteName
  );
  uint32_t remove(const std::string& remoteID);

private:
  RWLock _guard;
};

//==============================================================================

class Context {
private:
  Context(
    uint32_t localID,
    const std::string& remoteID, const std::string& remoteName
  );
public:
  ~Context();
  void cleanup();
  void clear();

  uint32_t getLocalID() const               { return _localID; }
  const std::string& getRemoteID() const    { return _remoteID; }
  const std::string& getRemoteName() const  { return _remoteName; }
  bool hasSessions() const                  { return false; }
  bool hasSharedMemoryBlocks() const        { return false; }

private:
  uint32_t _localID;
  std::string _remoteID, _remoteName;

  friend class ContextMap;
};

//==============================================================================

}   // namespace tbone::server

#endif  // TS_CONTEXT_HPP

// EOF
