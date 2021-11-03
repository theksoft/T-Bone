#include "ts_settings.hpp"
#include <libconfig.h++>
#include "tb_errors.hpp"

namespace tbone::server {

//==============================================================================

ServerSettings::ServerSettings()
  : _name(""), _appPipe(""), _appPort(0), _urlPort(0)
{}

ServerSettings::~ServerSettings() {
  clear();
}

void ServerSettings::clear(void) {
  _name.clear();
  _appPipe.clear();
  _appPort = 0;
  _urlPort = 0;
}

bool ServerSettings::readFile(const std::string filename) {

  libconfig::Config config;

  // Read configuration file

  try {
    config.readFile(filename.c_str());
  } catch (const libconfig::FileIOException &e) {
    std::cerr << TEES_ERROR_CONFIG_READ
              << " \"" << filename << "\"" << std::endl;
    return false;
  } catch (const libconfig::ParseException &e) {
    std::cerr << TEES_ERROR_CONFIG_PARSE
              << "\"" << e.getFile() << "\" : " << e.getLine() << " - " << e.getError() << std::endl;
    return false;
  }

  // Parse read configuration

  try {

    config.getRoot().lookupValue(TEES_SETTING_KEY_NAME, _name);

    const libconfig::Setting &services = (config.getRoot())[TEES_SETTING_KEY_SERVICES];
    int count = services.getLength();

    for (int i = 0; i < count; i++) {

      const libconfig::Setting &service = services[i];
      std::string type = "", protocol = "", pipe = "";
      int port = 0;

      // Check if all settings have been found & correct
      for(;;) {

        if (  service.lookupValue(TEES_SETTING_KEY_SERVICE_TYPE, type)
              && service.lookupValue(TEES_SETTING_KEY_PROTOCOL, protocol)) {

          // Application service
          if (0 == type.compare(TEES_SETTING_VAL_SERVICE_APPLICATION)) {
            // Local connection
            if (0 == protocol.compare(TEES_SETTING_VAL_SERVICE_LOCAL)) {
              if (service.lookupValue(TEES_SETTING_KEY_PIPE, pipe) && !pipe.empty()) {
                if ( _appPipe.empty() ) {
                  _appPipe = pipe;
                  break;
                }
              }
            }
            // TCP connection
            if (0 == protocol.compare(TEES_SETTING_VAL_SERVICE_TCP)) {
              if (service.lookupValue(TEES_SETTING_KEY_PORT, port) && 0 != port) {
                if (0 == _appPort) {
                  _appPort = port;
                  break;
                }
              }
            }
          }

          // Inspect service
          if (0 == type.compare(TEES_SETTING_VAL_SERVICE_INSPECT)) {
            // HTTP connection
            if (0 == protocol.compare(TEES_SETTING_VAL_SERVICE_HTTP)) {
              if (service.lookupValue(TEES_SETTING_KEY_PORT, port) && 0 != port) {
                if (0 == _urlPort) {
                  _urlPort = port;
                  break;
                }
                break;
              }
            }
          }

        }

        std::cerr << TEES_ERROR_CONFIG_BAD
                  << type << " : " << protocol << " : " << pipe << " | " << port << std::endl;
        return false;
      }
    }
  } catch (const libconfig::SettingException &e) {
    std::cerr << TEES_ERROR_CONFIG_MISSING << " : " << e.getPath() << std::endl;
    return false;
  }

  return true;
}

//==============================================================================

} // namespace tbone::server

// EOF
