#ifndef TS_SERVER_HPP
#define TS_SERVER_HPP

#include <boost/asio.hpp>

namespace tbone::server {

//==============================================================================

class TeeIOContext;

namespace bstnet = boost::asio;
namespace bstip = boost::asio::ip;
namespace bstlocal = boost::asio::local;
namespace bstsys = boost::system;

//==============================================================================

// Singleton
class TeeIOContext {

  /*
   * Class members
   */

private:
  TeeIOContext() {}
  ~TeeIOContext() { _theIOContext = NULL; }  // Not intended to be called
public:
  bstnet::io_context& getIOContext()  { return _ioContext; }
private:
  bstnet::io_context _ioContext;

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
