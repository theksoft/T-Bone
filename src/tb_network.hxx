#ifndef TB_NETWORK_HXX
#define TB_NETWORK_HXX

//==============================================================================

#include <boost/asio.hpp>

namespace bio     = boost::asio;
namespace bip     = boost::asio::ip;
namespace blocal  = boost::asio::local;
namespace bsys    = boost::system;

//==============================================================================

namespace tbone {

  typedef bio::io_context                     IOContext;
  typedef bio::signal_set                     SignalSet;

  typedef blocal::stream_protocol::socket     LocalSocket;
  typedef blocal::stream_protocol::acceptor   LocalAcceptor;
  typedef blocal::stream_protocol::endpoint   LocalEndPoint;

  typedef bip::tcp::socket                    TcpSocket;
  typedef bip::tcp::acceptor                  TcpAcceptor;
  typedef bip::tcp::endpoint                  TcpEndPoint;

}

//==============================================================================

#endif  // TB_NETWORK_HXX

// End of file
