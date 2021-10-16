#include "ca_tee.hpp"
#include "ca_tee_settings.hpp"
#include "tb_errors.hpp"
#include <cassert>

namespace tbone::client {

//==============================================================================

TeeMap* TeeMap::_theMap = NULL;

//==============================================================================

TeeMap::TeeMap() :
  _isConfigured(false) {
  clear();
  _theMap = this;
}

TeeMap::~TeeMap() {
  reset();
  _theMap = NULL;
}

TeeMap& TeeMap::get() {
  if (!_theMap) {
    new TeeMap;
  }
  assert(_theMap);
  return *_theMap;
}

bool TeeMap::create(const char *filename) {
  // Already done
  if (_isConfigured) {
    return true;
  }

  // Create TEE settings
  TeeSettingsMap settingsMap;
  if (NULL != filename) {
    if (!settingsMap.addFile(filename)) {
      return false;
    }
  } else if (!settingsMap.addFile()) {
    settingsMap.createDefault();
  }

  // Create TEE list
  bool error = false;
  for (auto& [name, settings] : settingsMap ) {
    assert(settings);
    // Create TEE connector
    TeeConnector *tc = NULL;
    if (NULL == (tc = TeeConnector::create(*settings))) {
      std::cerr << TEEC_ERROR_CONFIG_BAD << name << std::endl;
      error = true;
      break;
    }
    // Create TEE
    Tee *tee = new Tee(name.c_str(), tc);
    if (NULL == tee) {
      error = true;
      break;
    }
    // Register it in the TEE map
    insert({ name, tee });
  }

  if (error) {
    reset();
  }
  return !error;
}

Tee* TeeMap::match(const char *name) {
  create();
  std::string teeName = (name) ? name : TEEC_SETTING_VAL_DEFAULT_TEE_NAME;
  auto it = find(teeName);
  return (end() == it) ? NULL : it->second;
}

void TeeMap::reset() {
  for (auto it = begin(); it != end(); it++) {
    delete it->second;
  }
  clear();
  _isConfigured = false;
}

//==============================================================================

Tee::Tee(const std::string& name, TeeConnector *connector) :
  _name(name), _connector(connector) {
  assert(!_name.empty());
  assert(_connector);
}

Tee::~Tee() {
  _name.clear();
}

bool Tee::connect(Owner owner) {
  assert(_connector);
  bool connected = false;
  if (_connector->connect(owner)) {
    // TODO: Send hello and wait answer
    connected = true;
  }
  return connected;
}

void Tee::disconnect(Owner owner) {
  assert(_connector);
  // TODO: Send goodbye
  _connector->disconnect(owner);
}

//==============================================================================

} // namespace tbone::client

// EOF
