#ifndef CA_TEE_CONNECT_HXX
#define CA_TEE_CONNECT_HXX

#include "tb_messages.hpp"
#include "tb_network.hxx"

namespace tbone::client {

// An abstract connection base for pointing to template connection.
// Cannot be instantianted directly.
//==============================================================================

class TeeConnection {

protected:
  TeeConnection() {}
public:  
  virtual ~TeeConnection() {}
  virtual bool exchange(const std::string& request, std::string& acknowledge) = 0;

};

// Template connection class - S is the type of socket
// Cannot be instantianted directly.
//------------------------------------------------------------------------------

template <typename S>
class TTeeConnection : public TeeConnection {

  enum { max_length = TB_WORK_SIZE };

protected:

  TTeeConnection() : _socket(NULL) {}

public:

  virtual ~TTeeConnection() {
    _socket = NULL;
  }

  virtual bool exchange(const std::string& request, std::string& acknowledge) {
    TBMessage m;
    if (m.generateFrom(request)) {
      bstsys::error_code error;
      bstnet::write(*_socket, bstnet::buffer(m.getMessage(), m.getSize()), error);
      if (!error) {
        size_t length = _socket->read_some(bstnet::buffer(_data, max_length), error);
        if(!error|| bstnet::error::eof == error) {
          m.clear();
          if (0 == m.prepare(_data, length) && !m.hasError()) {
            if (m.assignTo(acknowledge)) {
              return true;
            }
          }
        }
      }
    }
    return false;
  }

protected:
  S *_socket;
  char _data[max_length];

};

// Actual specialized connection class
//==============================================================================

class TeeLocalConnection
  : public TTeeConnection<bstlocal::stream_protocol::socket> {
public:
  TeeLocalConnection(const std::string address);
  virtual ~TeeLocalConnection();
};

//------------------------------------------------------------------------------

class TeeTcpConnection
  : public TTeeConnection<bstip::tcp::socket> {
public:
  TeeTcpConnection(const std::string address, int port);
  virtual ~TeeTcpConnection();
};

//==============================================================================

}   // namespace tbone::client

#endif  // CA_TEE_CONNECT_HXX

// End of file
