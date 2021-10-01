/*
  Refer to Glocbal Platform TEE client API specification:
  https://globalplatform.org/specs-library/tee-client-api-specification/

  Global Platform Technology
  TEE Client API Specification
  Version 1.0
  Public Release
  July 2010
  Document reference: GPD_SPE_07
*/

#ifndef TEE_CLIENT_API_H
#define TEE_CLIENT_API_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "tee_errors.h"

/* --------------------------------------------------------------------------
 * Constants
 * -------------------------------------------------------------------------- */

#define TEEC_SUCCESS                  TEE_SUCCESS
#define TEEC_ERROR_GENERIC            TEE_ERROR_GENERIC
#define TEEC_ERROR_ACCESS_DENIED      TEE_ERROR_ACCESS_DENIED
#define TEEC_ERROR_CANCEL             TEE_ERROR_CANCEL
#define TEEC_ERROR_ACCESS_CONFLICT    TEE_ERROR_ACCESS_CONFLICT
#define TEEC_ERROR_EXCESS_DATA        TEE_ERROR_EXCESS_DATA
#define TEEC_ERROR_BAD_FORMAT         TEE_ERROR_BAD_FORMAT
#define TEEC_ERROR_BAD_PARAMETERS     TEE_ERROR_BAD_PARAMETERS
#define TEEC_ERROR_BAD_STATE          TEE_ERROR_BAD_STATE
#define TEEC_ERROR_ITEM_NOT_FOUND     TEE_ERROR_ITEM_NOT_FOUND
#define TEEC_ERROR_NOT_IMPLEMENTED    TEE_ERROR_NOT_IMPLEMENTED
#define TEEC_ERROR_NOT_SUPPORTED      TEE_ERROR_NOT_SUPPORTED
#define TEEC_ERROR_NO_DATA            TEE_ERROR_NO_DATA
#define TEEC_ERROR_OUT_OF_MEMORY      TEE_ERROR_OUT_OF_MEMORY
#define TEEC_ERROR_BUSY               TEE_ERROR_BUSY
#define TEEC_ERROR_COMMUNICATION      TEE_ERROR_COMMUNICATION
#define TEEC_ERROR_SECURITY           TEE_ERROR_SECURITY
#define TEEC_ERROR_SHORT_BUFFER       TEE_ERROR_SHORT_BUFFER
#define TEEC_ERROR_EXTERNAL_CANCEL    TEE_ERROR_EXTERNAL_CANCEL
#define TEEC_ERROR_TARGET_DEAD        TEE_ERROR_TARGET_DEAD

/* --------------------------------------------------------------------------
 * Data Types
 * -------------------------------------------------------------------------- */

typedef uint32_t TEEC_Result;

typedef struct {
  // imp
  uint32_t magic;
  void *data;
} TEEC_Context;

/* --------------------------------------------------------------------------
 * Functions
 * -------------------------------------------------------------------------- */

extern
TEEC_Result TEEC_InitializeContext(
  const char *name,
  TEEC_Context *context
);

extern
void TEEC_FinalizeContext(
  TEEC_Context *context
);

/* --------------------------------------------------------------------------
 * Non GP TEE function for setting a configuration file
 * This function must be use only for test purpose
 * and should not be use in production
 * -------------------------------------------------------------------------- */

extern
TEEC_Result __TEEC_Configure(const char* filename);

#ifdef __cplusplus
}
#endif

#endif  // TEE_CLIENT_API_H

// EOF
