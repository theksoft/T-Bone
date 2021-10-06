#include "ts_server.hpp"
#include <iostream>
#include <boost/asio.hpp>

#if 1

const int max_length = 1024;

void session(boost::asio::local::stream_protocol::socket sock)
{
  try
  {
    for (;;)
    {
      char data[max_length];

      boost::system::error_code error;
      /*size_t length = */sock.read_some(boost::asio::buffer(data), error);
      if (error == boost::asio::error::eof) {
        std::cout << "--- connection closed!" << std::endl;
        break; // Connection closed cleanly by peer.
      }
      else if (error)
        throw boost::system::system_error(error); // Some other error.

//      boost::asio::write(sock, boost::asio::buffer(data, length));
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void server(std::string file)
{
  boost::asio::local::stream_protocol::acceptor a(tbone::server::TeeIOContext::get()->getIOContext(), boost::asio::local::stream_protocol::endpoint(file));
  for (;;)
  {
    std::thread(session, a.accept()).detach();
    std::cout << "--- connection accepted!" << std::endl;
  }
}

int main(int argc, char* argv[])
{
  (void)argc; (void)argv;
  try
  {
    std::string file = "/tmp/sock-ca-unit";
    ::unlink(file.c_str());
    server(file);
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

#else

using boost::asio::ip::tcp;

const int max_length = 1024;

void session(tcp::socket sock)
{
  try
  {
    for (;;)
    {
      char data[max_length];

      boost::system::error_code error;
      /*size_t length = */sock.read_some(boost::asio::buffer(data), error);
      if (error == boost::asio::error::eof) {
        std::cout << "--- connection closed!" << std::endl;
        break; // Connection closed cleanly by peer.
      }
      else if (error)
        throw boost::system::system_error(error); // Some other error.

//      boost::asio::write(sock, boost::asio::buffer(data, length));
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void server(unsigned short port)
{
  tcp::acceptor a(tbone::server::TeeIOContext::get()->getIOContext(), tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    std::thread(session, a.accept()).detach();
    std::cout << "--- connection accepted!" << std::endl;
  }
}

int main(int argc, char* argv[])
{
  (void)argc; (void)argv;
  try
  {
    boost::asio::io_context io_context;
    server(io_context, 14582);
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

#endif