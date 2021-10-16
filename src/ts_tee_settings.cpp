#include "ts_tee_settings.hpp"
//#include <cassert>
#include <libconfig.h++>
#include "tb_errors.hpp"

namespace tbone::server {

//==============================================================================

TeeSettings::TeeSettings()
  : _name(""), _appPipe(""), _appPort(0), _urlPort(0)
{}

TeeSettings::~TeeSettings() {
  clear();
}

void TeeSettings::clear(void) {
  _name.clear();
  _appPipe.clear();
  _appPort = 0;
  _urlPort = 0;
}

bool TeeSettings::readFile(const std::string filename) {

  libconfig::Config config;

  // Read configuration file

  try {
    config.readFile(filename.c_str());
  } catch (const libconfig::FileIOException &e) {
    std::cerr << TEE_ERROR_CONFIG_READ
              << " \"" << filename << "\"" << std::endl;
    return false;
  } catch (const libconfig::ParseException &e) {
    std::cerr << TEE_ERROR_CONFIG_PARSE
              << "\"" << e.getFile() << "\" : " << e.getLine() << " - " << e.getError() << std::endl;
    return false;
  }

  // Parse read configuration

  try {

    config.getRoot().lookupValue(TEE_SETTING_NAME, _name);

    const libconfig::Setting &services = (config.getRoot())[TEE_SETTING_SERVICES];
    int count = services.getLength();

    for (int i = 0; i < count; i++) {

      const libconfig::Setting &service = services[i];
      std::string type = "", protocol = "", pipe = "";
      int port = 0;

      // Check if all settings have been found & correct
      for(;;) {

        if (  service.lookupValue(TEE_SETTING_SERVICE_TYPE, type)
              && service.lookupValue(TEE_SETTING_PROTOCOL, protocol)) {

          // Application service
          if (0 == type.compare(TEE_SERVICE_APPLICATION)) {
            // Local connection
            if (0 == protocol.compare(TEE_SERVICE_LOCAL)) {
              if (service.lookupValue(TEE_SETTING_PIPE, pipe) && !pipe.empty()) {
                if ( _appPipe.empty() ) {
                  _appPipe = pipe;
                  break;
                }
              }
            }
            // TCP connection
            if (0 == protocol.compare(TEE_SERVICE_TCP)) {
              if (service.lookupValue(TEE_SETTING_PORT, port) && 0 != port) {
                if (0 == _appPort) {
                  _appPort = port;
                  break;
                }
              }
            }
          }

          // Inspect service
          if (0 == type.compare(TEE_SERVICE_INSPECT)) {
            // HTTP connection
            if (0 == protocol.compare(TEE_SERVICE_HTTP)) {
              if (service.lookupValue(TEE_SETTING_PORT, port) && 0 != port) {
                if (0 == _urlPort) {
                  _urlPort = port;
                  break;
                }
                break;
              }
            }
          }

        }

        std::cerr << TEE_ERROR_CONFIG_BAD
                  << type << " : " << protocol << " : " << pipe << " | " << port << std::endl;
        return false;
      }
    }
  } catch (const libconfig::SettingException &e) {
    std::cerr << TEE_ERROR_CONFIG_MISSING << " : " << e.getPath() << std::endl;
    return false;
  }

  return true;
}

//==============================================================================

} // namespace tbone::server

// EOF
