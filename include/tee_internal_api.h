/*
  Refer to Glocbal Platform TEE internal core API specification:
  https://globalplatform.org/specs-library/tee-internal-core-api-specification/

  Global Platform Technology
  TEE Internal Core API Specification
  Version 1.3
  Public Release
  February 2021
  Document reference: GPD_SPE_10
*/

#ifndef TEE_INTERNAL_API_H
#define TEE_INTERNAL_API_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------
 * Constants
 * -------------------------------------------------------------------------- */

#include "tee_errors.h"

/* --------------------------------------------------------------------------
 * Data Types
 * -------------------------------------------------------------------------- */

typedef uint32_t TEE_Result;

/* --------------------------------------------------------------------------
 * Functions
 * -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif  // TEE_INTERNAL_API_H

// EOF
