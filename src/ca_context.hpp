#ifndef CA_CONTEXT_HPP
#define CA_CONTEXT_HPP

#include "tee_client_api.h"
#include "tb_ptr_vector.hxx"
#include "ca_tee.hpp"

namespace tbone::client {

//==============================================================================

class TeeContext;
class TeeContextMap;

//==============================================================================

class TeeContextMap : public GuardedPtrVector<TeeContext*> {
private:
  TeeContextMap();
public:
  virtual ~TeeContextMap();
  void reset();
  TeeContext* match(TEEC_Context *context);
  static TeeContextMap& get();

private:
  static TeeContextMap* _theMap;
};

//==============================================================================

class TeeContext {
private:
  TeeContext(TEEC_Context *context, Tee* tee);
public:
  virtual ~TeeContext();

  bool connect();
  void disconnect();

  bool operator==(const TeeContext& other) const;
  inline bool operator!=(const TeeContext& other) const { return !(*this == other); }

  bool hasSessions() const;
  bool hasSharedMemoryBlocks() const;

  static TeeContext* create(TEEC_Context *context, const char *name);

private:
  TEEC_Context* _context;
  Tee*  _tee;
  uint32_t _remoteID;

  friend bool operator== (const TeeContext& lhs, const TEEC_Context* rhs);
};

bool operator== (const TeeContext& lhs, const TEEC_Context* rhs);
inline bool operator!= (const TeeContext& lhs, const TEEC_Context* rhs) { return !(lhs == rhs); }
inline bool operator== (const TEEC_Context* lhs, const TeeContext& rhs) { return (rhs == lhs); }
inline bool operator!= (const TEEC_Context* lhs, const TeeContext& rhs) { return !(rhs == lhs); }

//==============================================================================

}   // namespace tbone::client

#endif  // CA_CONTEXT_HPP

// EOF
