/*
  T-Bone implementation.
  This file defines all messages exchange between TEE server and client applications.
*/

#include <string>

#ifndef TB_MESSAGES_HPP
#define TB_MESSAGES_HPP

#define TB_WORK_SIZE    2048

namespace tbone {

//==============================================================================

class MsgRequest {

public:
  MsgRequest();
  virtual ~MsgRequest();
  void clear();

  bool build(uintptr_t id, std::string& name);
  bool parse(std::string& msg);
  bool parse(const char *msg, size_t length);

  const std::string& getMessage() const         { return _message; }
  const std::string& getClientContextID() const { return _id; }
  const std::string& getClientTeeName() const   { return _name; }

private:
  bool parse();

private:
  std::string _message, _id, _name;
protected:
  std::string _banner;

};

//==============================================================================

class MsgHello : public MsgRequest {
public:
  MsgHello();
  virtual ~MsgHello();
};

//==============================================================================

class MsgBye : public MsgRequest {
public:
  MsgBye();
  virtual ~MsgBye();
};

//==============================================================================

class MsgAcknowledge {

public:
  MsgAcknowledge();
  virtual ~MsgAcknowledge();
  void clear();

  bool build(uint32_t id);
  bool parse(std::string& msg);
  bool parse(const char *msg, size_t length);

  const std::string& getMessage() const         { return _message; }
  uint32_t getClientPairID() const              { return _id; }

private:
  bool parse();

private:
  std::string _message;
  uint32_t _id;
protected:
  std::string _banner;

};

//==============================================================================

class MsgWelcome : public MsgAcknowledge {
public:
  MsgWelcome();
  virtual ~MsgWelcome();
};

//==============================================================================

class MsgFarewell : public MsgAcknowledge {
public:
  MsgFarewell();
  virtual ~MsgFarewell();
};

//==============================================================================

} // namespace tbone

// EOF

#endif  // TB_ERRORS_HPP

//EOF
