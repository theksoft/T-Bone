/*
  T-Bone implementation.
  This file defines all errors messages logged during execution.
*/

#ifndef TB_ERRORS_HPP
#define TB_ERRORS_HPP

#ifdef __cplusplus
#include <iostream>
#endif

// T-bone general errors

#define TB_ERROR_CONFIG_READ              "00000000 ERROR CONFIGURATION Cannot read configuration file"
#define TB_ERROR_CONFIG_PARSE             "00000001 ERROR CONFIGURATION Cannot parse at "
#define TB_ERROR_CONFIG_EMPTY             "00000002 ERROR CONFIGURATION No TEE defined"
#define TB_ERROR_CONFIG_MISSING           "00000003 ERROR CONFIGURATION Missing setting"
#define TB_ERROR_CONFIG_BAD               "00000004 ERROR CONFIGURATION Bad TEE definition for "
#define TB_ERROR_CONFIG_DUPLICATE         "00000005 ERROR CONFIGURATION Duplicate definition "
#define TB_ERROR_CONFIG_NO_SERVICE        "00000006 ERROR CONFIGURATION No application service defined"
#define TB_ERROR_CONFIG_ERROR             "00000007 ERROR CONFIGURATION "

#define TB_ERROR_CONNECTION               "00000010 ERROR CONNECTION Cannot connect to "
#define TB_ERROR_DISCONNECTION            "00000011 ERROR CONNECTION Clean discconnection failed for "

// Server errors

#define TEES_ERROR_CONFIG_READ            TB_ERROR_CONFIG_READ
#define TEES_ERROR_CONFIG_PARSE           TB_ERROR_CONFIG_PARSE
#define TEES_ERROR_CONFIG_MISSING         TB_ERROR_CONFIG_MISSING
#define TEES_ERROR_CONFIG_BAD             TB_ERROR_CONFIG_BAD
#define TEES_ERROR_CONFIG_NO_SERVICE      TB_ERROR_CONFIG_NO_SERVICE

// Client errors

#define TEEC_ERROR_CONFIG_READ            TB_ERROR_CONFIG_READ
#define TEEC_ERROR_CONFIG_PARSE           TB_ERROR_CONFIG_PARSE
#define TEEC_ERROR_CONFIG_EMPTY           TB_ERROR_CONFIG_EMPTY
#define TEEC_ERROR_CONFIG_MISSING         TB_ERROR_CONFIG_MISSING
#define TEEC_ERROR_CONFIG_BAD             TB_ERROR_CONFIG_BAD
#define TEEC_ERROR_CONFIG_DUPLICATE       TB_ERROR_CONFIG_DUPLICATE
#define TEEC_ERROR_CONFIG_ERROR           TB_ERROR_CONFIG_ERROR

#define TEEC_ERROR_CONNECTION             TB_ERROR_CONNECTION
#define TEEC_ERROR_DISCONNECTION          TB_ERROR_DISCONNECTION

#define TEEC_ERROR_CONTEXT_REINIT         "00010000 ERROR Attempting to intialize again a managed TEEC_Context"
#define TEEC_ERROR_CONTEXT_REMOVE         "00010001 ERROR Attempting to remove an unmanaged TEEC context"
#define TEEC_ERROR_CONTEXT_RM_SESSIONS    "00010002 ERROR Attempting to remove a TEEC context with remaining opened sessions"
#define TEEC_ERROR_CONTEXT_RM_SHM         "00010003 ERROR Attempting to remove a TEEC context with remaining shared memory blocks"

#endif  // TB_ERRORS_HPP

//EOF
