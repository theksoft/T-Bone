#ifndef CA_TEE_SETTINGS_HPP
#define CA_TEE_SETTINGS_HPP

#include "tb_settings.hpp"

#include <string>
#include <vector>
#include <map>

namespace tbone::client {

//==============================================================================

class TeeSettings;
class TeeSettingsMap;

//------------------------------------------------------------------------------

class TeeSettings {

private:
  std::string _protocol;
  std::string _address;
  int _port;

public:
  TeeSettings(
    const std::string& protocol = TEEC_SETTING_VAL_CONNECTION_LOCAL,
    const std::string& address = TEEC_SETTING_VAL_CONNECTION_LOCAL_ADDRESS,
    int port = 0
  );
  TeeSettings(TeeSettings& settings);
  virtual ~TeeSettings();

  TeeSettings& operator=(const TeeSettings& other);

  const std::string& getProtocol() const  { return _protocol; }
  const std::string& getAddress() const   { return _address; }
  int getPort() const                     { return _port; }

  bool isLocal() const  { return (0 == _protocol.compare(TEEC_SETTING_VAL_CONNECTION_LOCAL)); }
  bool isTcp() const    { return (0 == _protocol.compare(TEEC_SETTING_VAL_CONNECTION_TCP)); }

};

//------------------------------------------------------------------------------

class TeeSettingsMap : public std::map<std::string, TeeSettings*> {

public:
  TeeSettingsMap();
  virtual ~TeeSettingsMap();

  void reset();
  bool isPresent(const std::string& name) const;
  bool hasDefault() const;
  bool createDefault();
  bool addFile(const std::string filename = TEEC_DEFAULT_CONFIG_FILENAME);

};

//==============================================================================

}   // namespace tbone::client

#endif  // CA_TEE_SETTINGS_HPP

// EOF
