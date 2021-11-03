#ifndef CA_TEE_HPP
#define CA_TEE_HPP

#include "ca_tee_connect.hpp"
#include <string>
#include <memory>
#include <map>

namespace tbone::client {

//==============================================================================

class TeeMap;
class Tee;

//==============================================================================

class TeeMap : public std::map<std::string, Tee*> {

private:
  TeeMap();
public:
  virtual ~TeeMap();

public:
  bool create(const char *filename = NULL);
  Tee* match(const char *name = NULL);
  void reset();
  static TeeMap& get();

private:
  bool _isConfigured;
  static TeeMap* _theMap;

};

//==============================================================================

class Tee {
private:
  Tee(const std::string& name, TeeConnector *connector);
  virtual ~Tee();

public:
  const std::string& getName() { return _name; }

  typedef const void *Owner;
  uint32_t connect(Owner owner);
  void disconnect(Owner owner);

private:
  std::string _name;
  std::shared_ptr<TeeConnector> _connector; 

  friend class TeeMap;
};

//==============================================================================

}   // namespace tbone::client

#endif  // CA_TEE_HPP

// EOF
