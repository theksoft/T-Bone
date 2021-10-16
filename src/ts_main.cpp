#include "ts_server.hpp"
#include "ts_tee_settings.hpp"
#include <iostream>

using namespace tbone::server;

int main(int argc, char* argv[])
{
  (void)argc; (void)argv;
  TeeSettings settings;
  if (!settings.readFile()) {
    return 0;
  }
  try
  {
    std::string file = settings.getApplicationLocalFile();
    ::unlink(file.c_str());

    AppLocalServer  als (
      TeeIOContext::get()->getIOContext(),
      bstlocal::stream_protocol::endpoint(file)
    );

    AppTcpServer ats(
      TeeIOContext::get()->getIOContext(),
      bstip::tcp::endpoint(bstip::tcp::v4(), settings.getApplicationPort())
    );

    TeeIOContext::get()->getIOContext().run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  std::cout << std::endl << "<<<=== TEE services stopped! ===>>>" << std::endl << std::endl;
  return 0;
}
