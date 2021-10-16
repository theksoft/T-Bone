#include "ca_tee_settings.hpp"
#include <cassert>
#include <libconfig.h++>
#include "tb_errors.hpp"

namespace tbone::client {

//==============================================================================

TeeSettings::TeeSettings(
  const std::string& protocol,
  const std::string& address,
  int port
) :
  _protocol(protocol),
  _address(address),
  _port(port)
{
  assert(
    0 == protocol.compare(TEEC_CONNECTION_LOCAL) ||
    (0 == protocol.compare(TEEC_CONNECTION_TCP) && 0 != port)
  );
}

TeeSettings::TeeSettings(TeeSettings& settings) :
  _protocol(settings._protocol),
  _address(settings._address),
  _port(settings._port)
{}

TeeSettings::~TeeSettings() {
  _protocol.clear();
  _address.clear();
  _port = 0;
}

TeeSettings& TeeSettings::operator=(const TeeSettings& other) {
  if (this != &other) {
    _protocol = other._protocol;
    _address = other._address;
    _port = other._port;
  }
  return *this;
}

//==============================================================================

TeeSettingsMap::TeeSettingsMap() {}
TeeSettingsMap::~TeeSettingsMap() {
  reset();
  clear();
}

void TeeSettingsMap::reset() {
  for (auto it = begin(); it != end(); it = begin()) {
    TeeSettings *v = it->second;
    assert(v);
    erase(it);
    delete v;
  }
}

bool TeeSettingsMap::isPresent(const std::string& name) const {
  return (cend() != find(name));
}

bool TeeSettingsMap::hasDefault() const {
  return isPresent(TEEC_DEFAULT_TEE_NAME);
}

bool TeeSettingsMap::createDefault() {
  if (hasDefault()) {
    return false;
  }
  try {
    insert({ TEEC_DEFAULT_TEE_NAME, new TeeSettings });
  } catch (const std::exception &e) {
    std::cerr << TEEC_ERROR_CONFIG_ERROR << " : " << e.what() << std::endl;
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
// Reading from configuration file
//------------------------------------------------------------------------------

bool TeeSettingsMap::addFile(const std::string filename) {

  libconfig::Config config;

  // Read configuration file

  try {
    config.readFile(filename.c_str());
  } catch (const libconfig::FileIOException &e) {
    std::cerr << TEEC_ERROR_CONFIG_READ
              << " \"" << filename << "\"" << std::endl;
    return false;
  } catch (const libconfig::ParseException &e) {
    std::cerr << TEEC_ERROR_CONFIG_PARSE
              << "\"" << e.getFile() << "\" : " << e.getLine() << " - " << e.getError() << std::endl;
    return false;
  }

  // Parse read configuration

  try {

    // Lookup Tee objects in configuration
    const libconfig::Setting &tees = (config.getRoot())[TEEC_SETTING_LIST];
    int count = tees.getLength();
    if (!count) {
      std::cerr << TEEC_ERROR_CONFIG_EMPTY << std::endl;
      return false;
    }

    for (int i = 0; i < count; i++) {

      const libconfig::Setting &tee = tees[i];
      std::string name = "", protocol = "", address = "";
      int port = 0;

      // Check if all settings have been found & correct
      for(;;) {

        if (  tee.lookupValue(TEEC_SETTING_NAME, name)
              && tee.lookupValue(TEEC_SETTING_PROTOCOL, protocol) )
        {
          if (!name.empty()) {
            // Local connection
            if (0 == protocol.compare(TEEC_CONNECTION_LOCAL)) {
              if (tee.lookupValue(TEEC_SETTING_PIPE, address) && !address.empty()) {
                break;
              }
            }
            // TCP connection
            if (0 == protocol.compare(TEEC_CONNECTION_TCP)) {
              if ( (tee.lookupValue(TEEC_SETTING_ADDRESS, address) && !address.empty())
                && (tee.lookupValue(TEEC_SETTING_PORT, port) && 0 != port) ) {
                break;
              }
            }
          }
        }

        std::cerr << TEEC_ERROR_CONFIG_BAD
                  << name << " : " << protocol << " : " << address << " : " << port << std::endl;
        return false;
      }

      // Check for duplicate
      if (isPresent(name)) {
        std::cerr << TEEC_ERROR_CONFIG_DUPLICATE << name;
      }

      // Map new element
      try {
        insert({ name, new TeeSettings(protocol, address, port) });
      } catch (const std::exception &e) {
        std::cerr << TEEC_ERROR_CONFIG_ERROR << " : " << e.what() << std::endl;
        return false;
      }
    }
  } catch (const libconfig::SettingException &e) {
    std::cerr << TEEC_ERROR_CONFIG_MISSING << " : " << e.getPath() << std::endl;
    return false;
  }
  return true;
}

//==============================================================================

} // namespace tbone::client

// EOF
