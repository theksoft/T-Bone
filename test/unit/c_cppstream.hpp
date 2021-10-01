#ifndef C_CPPSTREAM_HPP
#define C_CPPSTREAM_HPP

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  CPP_STREAM_OUT,
  CPP_STREAM_ERR
} tCppStreamType;

typedef void (*tCVoidProc)(void);

extern
int checkCppStream(
  tCppStreamType type,
  tCVoidProc f,
  const char* expected,
  size_t len
);

#ifdef __cplusplus
}
#endif

#endif  // C_CPPSTREAM_HPP

// EOF
