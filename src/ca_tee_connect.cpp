#include "ca_tee_connect.hpp"
#include "tb_errors.hpp"
#include <cassert>

namespace tbone::client {

//==============================================================================

TeeConnectorList TeeConnector::_connectors;

//==============================================================================

TeeConnector::TeeConnector(TDomain domain, const std::string& address, int port) :
  _domain(domain), _address(address), _port(port), _connection(NULL) {
  assert(LOCAL == _domain || 0 != _port);
  if (LOCAL == _domain) _port = 0;
  _owners.clear();
  _connectors.add(this);
}

TeeConnector::~TeeConnector() {
  _connectors.remove(this);
  clear();
}

void TeeConnector::clear() {
  _domain = LOCAL;
  _address.clear();
  _port = 0;
  _owners.clear();
}

bool TeeConnector::exist(Owner owner) {
  return _owners.exist(owner);
}

bool TeeConnector::operator==(const TeeConnector& other) const {
  for(;;) {
    if (_domain != other._domain) break;
    if (_domain == TCP && _port != other._port) break;
    if (0 != _address.compare(other._address)) break;
    return true;
  }
  return false;
}

bool TeeConnector::connect(Owner owner) {
  assert(owner);
  bool mustOpen = (0 == _owners.size());
  if (!_owners.add(owner)) {
    return false;
  }
  if (mustOpen && !open()) {
    _owners.remove(owner);
    return false;
  }
  return true;
}

void TeeConnector::disconnect(Owner owner) {
  _owners.remove(owner);
  if (0 == _owners.size()) {
    close();
  }
}

bool TeeConnector::open() {
  assert(!_connection);
  assert(TCP == _domain || LOCAL == _domain);

  try {

    if (LOCAL == _domain) _connection = new TeeLocalConnection(_address);
    else                  _connection = new TeeTcpConnection(_address, _port);

  } catch (bstsys::system_error &e) {
    std::cerr << TEEC_ERROR_CONNECTION << _address;
    if (TCP == _domain) {
      std::cerr <<  ":" << std::dec << _port;
    }
    std::cerr << " - " << e.what() << std::endl;
    close();
    return false;

  }

  return true;
}

void TeeConnector::close() {
  if (_connection) {
    delete _connection;
  }
  _connection = NULL;
}


//==============================================================================

TeeConnector* TeeConnector::create(const TeeSettings& settings) {
  assert(settings.isLocal() || settings.isTcp());

  TeeConnector *c = _connectors.match(settings);
  if (NULL == c) {
    TDomain domain = settings.isLocal() ? LOCAL : TCP;
    int port = settings.isTcp() ? settings.getPort() : 0;
    c = new TeeConnector(domain, settings.getAddress(), port);
  }
  return c;
}

//==============================================================================

TeeConnector* TeeConnectorList::match(const TeeSettings& settings) {
  std::lock_guard<std::recursive_mutex> lock(_guard);
  for (auto it = begin(); it != end(); it++) {
    if (*(*it) == settings) {
      return *it;
    }
  }
  return NULL;
}

//==============================================================================

bool operator== (const TeeConnector& lhs, const TeeSettings& rhs) {
  for(;;) {
    if (lhs._domain == TeeConnector::LOCAL && !rhs.isLocal()) break;
    if (lhs._domain == TeeConnector::TCP && !rhs.isTcp()) break;
    if (0 != lhs._address.compare(rhs.getAddress())) break;
    if (lhs._domain == TeeConnector::TCP && lhs._port != rhs.getPort()) break;
    return true;
  }
  return false;
}

//==============================================================================

} // namespace tbone::client

// EOF
