#include <string>
#include <iostream>
#include <sstream>

#include "c_cppstream.hpp"

extern "C"
int checkCppStream(
  tCppStreamType type,
  tCVoidProc f,
  const char* expected,
  size_t len
) {
  std::stringstream buffer;
  std::ostream& stdstr = (type == CPP_STREAM_OUT) ? std::cout : std::cerr;

  // Redirect std::cout to buffer
  std::streambuf* prevcoutbuf = stdstr.rdbuf(buffer.rdbuf());

  f();

  // Use the string value of buffer to compare against expected output
  std::string text = buffer.str();
  int result = (0 == ((len) ? text.compare(0, len, expected) : text.compare(expected)));

  // Restore original buffer before exiting
  stdstr.rdbuf(prevcoutbuf);

  return result;
}

// EOF
