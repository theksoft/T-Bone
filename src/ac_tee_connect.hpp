#ifndef AC_TEE_CONNECT_HPP
#define AC_TEE_CONNECT_HPP

#include "ac_tee_settings.hpp"
#include "tb_ptr_vector.hxx"
#include <boost/asio.hpp>

namespace tbone::client {

//==============================================================================

class TeeIOContext;
class TeeConnection;
class TeeLocalConnection;
class TeeTcpConnection;
class TeeConnector;
class TeeConnectorList;

namespace bstnet = boost::asio;
namespace bstip = boost::asio::ip;
namespace bstlocal = boost::asio::local;
namespace bstsys = boost::system;

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

class TeeConnection {
public:
  TeeConnection() {}
  virtual ~TeeConnection() {}
};

//------------------------------------------------------------------------------

class TeeLocalConnection : public TeeConnection {
public:
  TeeLocalConnection(const std::string address);
  virtual ~TeeLocalConnection();
private:
  bstlocal::stream_protocol::socket *_socket;
};

//------------------------------------------------------------------------------

class TeeTcpConnection : public TeeConnection {
public:
  TeeTcpConnection(const std::string address, int port);
  virtual ~TeeTcpConnection();
private:
  bstip::tcp::socket *_socket;
};

//------------------------------------------------------------------------------

class TeeConnector {

public:
  typedef const void *Owner;
  typedef enum { LOCAL, TCP } TDomain;

private:
  TeeConnector(TDomain domain, const std::string& address, int port);
  void clear();
public:
  virtual ~TeeConnector();

public:
  bool operator==(const TeeConnector& other) const;
  inline bool operator!=(const TeeConnector& other) const { return !(*this == other); }

  bool exist(Owner owner);
  bool connect(Owner owner);
  void disconnect(Owner owner);
  bool open();
  void close();

  static TeeConnector* create(const TeeSettings& settings);
  
private:
  TDomain _domain;
  std::string _address;
  int _port;

  TeeConnection *_connection;
  GuardedPtrVector<Owner> _owners;
  static TeeConnectorList _connectors;

  friend bool operator== (const TeeConnector& lhs, const TeeSettings& rhs);
};

bool operator== (const TeeConnector& lhs, const TeeSettings& rhs);
inline bool operator!= (const TeeConnector& lhs, const TeeSettings& rhs) { return !(lhs == rhs); }
inline bool operator== (const TeeSettings& lhs, const TeeConnector& rhs) { return (rhs == lhs); }
inline bool operator!= (const TeeSettings& lhs, const TeeConnector& rhs) { return !(rhs == lhs); }

//==============================================================================

class TeeConnectorList : public GuardedPtrVector<TeeConnector*> {
public:
  TeeConnectorList() {}
  virtual ~TeeConnectorList() {}
  TeeConnector* match(const TeeSettings& settings);
};

//==============================================================================

}   // namespace tbone::client

#endif  // AC_TEE_CONNECT_HPP

// EOF
