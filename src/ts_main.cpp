#include "ts_server.hpp"
#include "ts_tee_settings.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
  (void)argc; (void)argv;
  tbone::server::TeeSettings settings;
  if (!settings.readFile()) {
    return 0;
  }
  try
  {
    std::string file = settings.getApplicationLocalFile();
    ::unlink(file.c_str());

    tbone::server::AppServer<bstlocal::stream_protocol::socket, bstlocal::stream_protocol::acceptor> s1(
      tbone::server::TeeIOContext::get()->getIOContext(),
      bstlocal::stream_protocol::endpoint(file)
    );

    tbone::server::AppServer<bstip::tcp::socket, bstip::tcp::acceptor> s2(
      tbone::server::TeeIOContext::get()->getIOContext(),
      bstip::tcp::endpoint(bstip::tcp::v4(), settings.getApplicationPort())
    );

    tbone::server::TeeIOContext::get()->getIOContext().run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  std::cout << std::endl << "### TEE services stopped! ###" << std::endl;
  return 0;
}
