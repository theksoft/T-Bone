#ifndef TS_TEE_SETTINGS_HPP
#define TS_TEE_SETTINGS_HPP

#include "tb_tee_settings.hpp"

#include <string>

namespace tbone::server {

//==============================================================================

class TeeSettings;

//==============================================================================

class TeeSettings {

private:
  std::string _name;
  std::string _appPipe;
  int _appPort;
  int _urlPort;

public:
  TeeSettings();
  virtual ~TeeSettings();

  const std::string& getName() const                  { return _name; }
  const std::string& getApplicationLocalFile() const  { return _appPipe; }
  int getApplicationPort() const                      { return _appPort; }
  int getInpectUrlPort() const                        { return _urlPort; }

  void clear();
  bool readFile(const std::string filename = TEE_DEFAULT_CONFIG_FILENAME);

#if 0
private:
  std::string _domain;
  std::string _pipe;
  int _port;

public:
  TeeSettings(
    const std::string& pipe = TEE_CONNECTION_LOCAL_FILE
  );
  TeeSettings(TeeSettings& settings);
  virtual ~TeeSettings();

  TeeSettings& operator=(const TeeSettings& other);

  bool setLocalDomain(const std::string& pipe);
  bool setTcpDomain(int port);

  const std::string& getDomain() const  { return _domain; }
  const std::string& getPipe() const    { return _pipe; }
  int getPort() const                   { return _port; }

  bool isLocal() const  { return (0 == _domain.compare(TEE_CONNECTION_LOCAL)); }
  bool isTcp() const    { return (0 == _domain.compare(TEE_CONNECTION_TCP)); }

  void clear();
  bool readFile(const std::string filename = TEE_DEFAULT_CONFIG_FILENAME);
#endif

};

//==============================================================================

}   // namespace tbone::server

#endif  // TS_TEE_SETTINGS_HPP

// EOF
