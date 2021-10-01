#ifndef AC_TEE_SETTINGS_HPP
#define AC_TEE_SETTINGS_HPP

#include <string>
#include <vector>
#include <map>

namespace tbone::client {

//==============================================================================

class TeeSettings;
class TeeSettingsMap;

#define TEEC_DEFAULT_CONFIG_FILENAME  "tbone-client.cfg"
#define TEEC_DEFAULT_TEE_NAME         "*"
#define TEEC_CONNECTION_TCP           "net-tcp"
#define TEEC_CONNECTION_LOCAL         "local"
#define TEEC_CONNECTION_LOCAL_ADDRESS "sock-default-tee-connection"

//------------------------------------------------------------------------------

class TeeSettings {

private:
  std::string _domain;
  std::string _address;
  int _port;

public:
  TeeSettings(
    const std::string& domain = TEEC_CONNECTION_LOCAL,
    const std::string& address = TEEC_CONNECTION_LOCAL_ADDRESS,
    int port = 0
  );
  TeeSettings(TeeSettings& settings);
  virtual ~TeeSettings();

  TeeSettings& operator=(const TeeSettings& other);

  const std::string& getDomain() const  { return _domain; }
  const std::string& getAddress() const { return _address; }
  int getPort() const                   { return _port; }

  bool isLocal() const  { return (0 == _domain.compare(TEEC_CONNECTION_LOCAL)); }
  bool isTcp() const    { return (0 == _domain.compare(TEEC_CONNECTION_TCP)); }

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

#endif  // AC_TEE_SETTINGS_HPP

// EOF
