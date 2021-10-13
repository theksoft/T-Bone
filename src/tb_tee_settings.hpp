/*
  T-Bone implementation.
  This file defines all default settings for server and client.
*/

#ifndef TB_TEE_SETTINGS_HPP
#define TB_TEE_SETTINGS_HPP

// Common settings

#define TEE_DEFAULT_TEE_NAME          "*"
#define TEE_CONNECTION_TCP            "net-tcp"
#define TEE_CONNECTION_LOCAL          "local"
#define TEE_CONNECTION_PORT           14582
#define TEE_CONNECTION_LOCAL_ADDRESS  "sock-default-tee-connection"

// Server settings

#define TEE_DEFAULT_CONFIG_FILENAME   "tbone-server.cfg"

// Client settings

#define TEEC_DEFAULT_CONFIG_FILENAME  "tbone-client.cfg"

#define TEEC_DEFAULT_TEE_NAME         TEE_DEFAULT_TEE_NAME
#define TEEC_CONNECTION_TCP           TEE_CONNECTION_TCP
#define TEEC_CONNECTION_LOCAL         TEE_CONNECTION_LOCAL
#define TEEC_CONNECTION_LOCAL_ADDRESS TEE_CONNECTION_LOCAL_ADDRESS
#define TEEC_CONNECTION_PORT          TEE_CONNECTION_PORT

#endif  // TB_TEE_SETTINGS_HPP

//EOF
