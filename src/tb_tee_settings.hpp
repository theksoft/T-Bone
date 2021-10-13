/*
  T-Bone implementation.
  This file defines all default settings for server and client.
*/

#ifndef TB_TEE_SETTINGS_HPP
#define TB_TEE_SETTINGS_HPP

// Common settings keys

#define TEE_SETTING_NAME             "name"
#define TEE_SETTING_DOMAIN           "domain"
#define TEE_SETTING_PIPE             "local-file"
#define TEE_SETTING_PORT             "port"

// Common settings values

#define TEE_DEFAULT_TEE_NAME          "*"
#define TEE_CONNECTION_TCP            "net-tcp"
#define TEE_CONNECTION_LOCAL          "local"
#define TEE_CONNECTION_PORT           14582
#define TEE_CONNECTION_LOCAL_ADDRESS  "sock-default-tee-connection"

// Server setting keys

// Server settings values

#define TEE_DEFAULT_CONFIG_FILENAME   "tbone-server.cfg"
#define TEE_CONNECTION_LOCAL_FILE     TEE_CONNECTION_LOCAL_ADDRESS

// Client setting keys

#define TEEC_SETTING_LIST             "TEEs"
#define TEEC_SETTING_NAME             TEE_SETTING_NAME
#define TEEC_SETTING_DOMAIN           TEE_SETTING_DOMAIN
#define TEEC_SETTING_ADDRESS          "address"
#define TEEC_SETTING_PIPE             TEE_SETTING_PIPE
#define TEEC_SETTING_PORT             TEE_SETTING_PORT

// Client settings values

#define TEEC_DEFAULT_CONFIG_FILENAME  "tbone-client.cfg"

#define TEEC_DEFAULT_TEE_NAME         TEE_DEFAULT_TEE_NAME
#define TEEC_CONNECTION_TCP           TEE_CONNECTION_TCP
#define TEEC_CONNECTION_LOCAL         TEE_CONNECTION_LOCAL
#define TEEC_CONNECTION_LOCAL_ADDRESS TEE_CONNECTION_LOCAL_ADDRESS
#define TEEC_CONNECTION_PORT          TEE_CONNECTION_PORT

#endif  // TB_TEE_SETTINGS_HPP

//EOF
