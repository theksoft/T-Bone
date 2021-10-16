#include "tb_errors.hpp"
#include "ts_server.hpp"
#include "ts_settings.hpp"
#include <iostream>

//==============================================================================

namespace tbone::server {

class Options {

public:
  Options(int argc, char* argv[])
    : _command(""),
      _error(false),
      _help(false),
      _errorMessage(""),
      _configurationFile(TEES_DEFAULT_CONFIG_FILENAME) {

    if (argc) { _command = argv[0]; }

    int c;
    while (!_error && -1 != (c = getopt (argc, argv, "hc:"))) {
      switch (c) {
      case 'h': _help = true; break;
      case 'c':
        _configurationFile = optarg;
        if (!strlen(optarg)) {
          _errorMessage = "ERROR Option 'c' requires a valid argument.";
          _error = true;
        }
        break;
      case '?':
        if (optopt == 'c')
          _errorMessage = "ERROR Option '-" + std::string(1, (char)optopt) + "' requires an argument.";
        else
          _errorMessage = "Unknown option '-" + std::string(1, (char)optopt) + "'.";
        _error = true;
        break;
      default:
        _errorMessage = "Invalid command line!";
        _error = true;
        break;
      }
    }
  }

  ~Options() {
    clear();
  }

  void printError() {
    std::cerr << std::endl << _errorMessage << std::endl;
  }

  void printUsage() {
    std::cout << std::endl;
    std::cout << "Usage: " << _command << " [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -c <filepath>  <filepath> for configuration file." << std::endl
              << "                 Default is \"" << TEES_DEFAULT_CONFIG_FILENAME << "\".)" << std::endl;
    std::cout << "  -h             Display this help and exit." << std::endl;
    std::cout << std::endl;
  }

  bool isValid() const                            { return !_error; }
  bool needHelp() const                           { return _help; }
  const std::string& getConfigurationFile() const { return _configurationFile; }

private:
  void clear() {
    _command.clear();
    _error = _help = false;
    _errorMessage.clear();
    _configurationFile.clear();
  }

private:
  std::string _command;
  bool _error;
  bool _help;
  std::string _errorMessage;
  std::string _configurationFile;
};

}   // namespace tbone::server

//==============================================================================

int main(int argc, char* argv[])
{
  using namespace tbone::server;

  Options opt(argc, argv);
  if (opt.needHelp() || !opt.isValid()) {
    if (!opt.isValid())
      opt.printError();
    opt.printUsage();
    return 0;
  }

  ServerSettings settings;
  if (!settings.readFile(opt.getConfigurationFile())) {
    return 0;
  }

  AppLocalServer *als = NULL;
  AppTcpServer *ats = NULL;

  try
  {
    bool oneOfAppServiceSet = false;
    std::string file = settings.getApplicationLocalFile();
    if (!file.empty()) {
      ::unlink(file.c_str());
      als = new AppLocalServer(
        TeeIOContext::get()->getIOContext(),
        bstlocal::stream_protocol::endpoint(file)
      );
      oneOfAppServiceSet = true;
    }

    unsigned short port = settings.getApplicationPort();
    if (0 != port) {
      ats = new AppTcpServer(
        TeeIOContext::get()->getIOContext(),
        bstip::tcp::endpoint(bstip::tcp::v4(), port)
      );
      oneOfAppServiceSet = true;
    }

    port = settings.getInpectUrlPort();
    if (0 != port) {
      std::cout << "INFO Inspect service not implemented!" << std::endl;
    }

    if (oneOfAppServiceSet) {
      TeeIOContext::get()->getIOContext().run();
    } else {
      std::cerr << TEES_ERROR_CONFIG_NO_SERVICE << std::endl;
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  if (als) delete als;
  if (ats) delete ats;
  std::cout << std::endl << "<<<=== TEE services stopped! ===>>>" << std::endl << std::endl;
  return 0;
}

//==============================================================================

// EOF
