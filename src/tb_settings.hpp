/*
  T-Bone implementation.
  This file defines all default settings for server and client.
*/

#ifndef TB_SETTINGS_HPP
#define TB_SETTINGS_HPP

#define TEES_DEFAULT_CONFIG_FILENAME            "tbone-server.cfg"
#define TEEC_DEFAULT_CONFIG_FILENAME            "tbone-client.cfg"

// Common settings keys

#define TB_SETTING_KEY_NAME                     "name"
#define TB_SETTING_KEY_PROTOCOL                 "protocol"
#define TB_SETTING_KEY_PIPE                     "local-file"
#define TB_SETTING_KEY_PORT                     "port"

// Common settings values

#define TB_SETTING_VAL_DEFAULT_TEE_NAME          "*"
#define TB_SETTING_VAL_CONNECTION_TCP            "net-tcp"
#define TB_SETTING_VAL_CONNECTION_LOCAL          "local"
#define TB_SETTING_VAL_CONNECTION_PORT           14501
#define TB_SETTING_VAL_CONNECTION_LOCAL_ADDRESS  "sock-default-tee-connection"

// Server setting keys

#define TEES_SETTING_KEY_NAME                     TB_SETTING_KEY_NAME
#define TEES_SETTING_KEY_SERVICES                 "services"
#define TEES_SETTING_KEY_SERVICE_TYPE             "type"
#define TEES_SETTING_KEY_PROTOCOL                 TB_SETTING_KEY_PROTOCOL
#define TEES_SETTING_KEY_PIPE                     TB_SETTING_KEY_PIPE
#define TEES_SETTING_KEY_PORT                     TB_SETTING_KEY_PORT

// Server settings values

#define TEES_SETTING_VAL_SERVICE_APPLICATION      "application"
#define TEES_SETTING_VAL_SERVICE_INSPECT          "inspect"
#define TEES_SETTING_VAL_SERVICE_HTTP             "net-http"
#define TEES_SETTING_VAL_SERVICE_TCP              TB_SETTING_VAL_CONNECTION_TCP
#define TEES_SETTING_VAL_SERVICE_LOCAL            TB_SETTING_VAL_CONNECTION_LOCAL
#define TEES_SETTING_VAL_SERVICE_LOCAL_FILE       TB_SETTING_VAL_CONNECTION_LOCAL_ADDRESS
#define TEES_SETTING_VAL_SERVICE_TCP_PORT         TB_SETTING_VAL_CONNECTION_PORT
#define TEES_SETTING_VAL_SERVICE_HTTP_PORT        9999

// Client setting keys

#define TEEC_SETTING_KEY_LIST                     "TEEs"
#define TEEC_SETTING_KEY_NAME                     TB_SETTING_KEY_NAME
#define TEEC_SETTING_KEY_PROTOCOL                 TB_SETTING_KEY_PROTOCOL
#define TEEC_SETTING_KEY_PIPE                     TB_SETTING_KEY_PIPE
#define TEEC_SETTING_KEY_ADDRESS                  "address"
#define TEEC_SETTING_KEY_PORT                     TB_SETTING_KEY_PORT

// Client settings values

#define TEEC_SETTING_VAL_DEFAULT_TEE_NAME         TB_SETTING_VAL_DEFAULT_TEE_NAME
#define TEEC_SETTING_VAL_CONNECTION_TCP           TB_SETTING_VAL_CONNECTION_TCP
#define TEEC_SETTING_VAL_CONNECTION_LOCAL         TB_SETTING_VAL_CONNECTION_LOCAL
#define TEEC_SETTING_VAL_CONNECTION_LOCAL_ADDRESS TB_SETTING_VAL_CONNECTION_LOCAL_ADDRESS
#define TEEC_SETTING_VAL_CONNECTION_PORT          TB_SETTING_VAL_CONNECTION_PORT

#endif  // TB_SETTINGS_HPP

//EOF
