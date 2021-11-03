#ifndef TS_SERVER_HPP
#define TS_SERVER_HPP

#include "tb_network.hxx"
#include "ts_server_app.hxx"

namespace tbone::server {

//==============================================================================

class TeeIOContext;

//==============================================================================

// Singleton
class TeeIOContext {

  /*
   * Class members
   */

private:
  TeeIOContext();
  ~TeeIOContext();
public:
  IOContext& getIOContext()  { return _ioContext; }
private:
  IOContext _ioContext;
  SignalSet _signals;

  /*
   * Singleton management
   */

public:
  static TeeIOContext* get();
private:
  static TeeIOContext *_theIOContext;

};

//==============================================================================

}   // namespace tbone::server

#endif  // TS_SERVER_HPP

// EOF
