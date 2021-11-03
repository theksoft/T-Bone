#ifndef TS_SETTINGS_HPP
#define TS_SETTINGS_HPP

#include "tb_settings.hpp"

#include <string>

namespace tbone::server {

//==============================================================================

class ServerSettings;

//==============================================================================

class ServerSettings {

private:
  std::string _name;
  std::string _appPipe;
  int _appPort;
  int _urlPort;

public:
  ServerSettings();
  virtual ~ServerSettings();

  const std::string& getName() const                  { return _name; }
  const std::string& getApplicationLocalFile() const  { return _appPipe; }
  int getApplicationPort() const                      { return _appPort; }
  int getInpectUrlPort() const                        { return _urlPort; }

  void clear();
  bool readFile(const std::string filename = TEES_DEFAULT_CONFIG_FILENAME);
};

//==============================================================================

}   // namespace tbone::server

#endif  // TS_SETTINGS_HPP

// EOF
