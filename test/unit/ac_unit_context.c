/*
  T-Bone TEE Client API implementation unitary testing.
  This file is related to TEE context management unitary testing.
*/

#include "ac_unit_tests.h"
#include "tee_client_api.h"

#include "c_cppstream.hpp"
#include "tb_errors.hpp"
#include <string.h>

/* --------------------------------------------------------------------------
 * Test forward declarations
 * -------------------------------------------------------------------------- */

static void initializeBadContext(void);
static void initializeManagedContext(void);

static void finalizeBadContext(void);
static void finalizeUninitializedContext(void);

static void initialize2finalizeContext(void);
static void initializeInexistentLocalCon(void);
static void initializeInexistentTcpCon(void);

/* --------------------------------------------------------------------------
 * Test groups definition
 * -------------------------------------------------------------------------- */

static tCuwTest tests[] = {
  { "Initialize bad TEE context", initializeBadContext },
  { "Finalize bad TEE context", finalizeBadContext },
  { "Finalize uninitialized TEE context", finalizeUninitializedContext },
  { "Initialize & finalize TEE context", initialize2finalizeContext },
  { "Initialize managed TEE context", initializeManagedContext },
  { "Initialize using inexistent local connection", initializeInexistentLocalCon },
  { "Initialize using inexistent tcp connection", initializeInexistentTcpCon },
  { NULL, NULL }  // End of test table
};

/* --------------------------------------------------------------------------
 * Test suites definition and exported getter
 * -------------------------------------------------------------------------- */

static tCuwSuite suite = {
  .reg = { "TEE Client API - Context management", NULL, NULL },
  .tests = tests
};

tCuwSuite* getACUnitContextSuite(void) {
  return &suite;
}

/* --------------------------------------------------------------------------
 * Test(s) implementation for TEEC_InitializeContext()
 * -------------------------------------------------------------------------- */

static void initializeBadContext(void) {
  CU_ASSERT(TEEC_ERROR_BAD_PARAMETERS == TEEC_InitializeContext(NULL, NULL));
}

static void doInitializeManagedContext(void) {
  TEEC_Context c;
  TEEC_Result r;
  CU_ASSERT(TEEC_SUCCESS == (r = TEEC_InitializeContext(NULL, &c)));
  if (TEEC_SUCCESS == r) {
    CU_ASSERT(TEEC_ERROR_GENERIC == TEEC_InitializeContext("other", &c));
    TEEC_FinalizeContext(&c);
  }
}

static void initializeManagedContext(void) {
  // Indirection - Checking stderr
  CU_ASSERT(
    checkCppStream(
      CPP_STREAM_ERR, doInitializeManagedContext,
      TEEC_ERROR_CONTEXT_REINIT, strlen(TEEC_ERROR_CONTEXT_REINIT)
    )
  );
}

/* --------------------------------------------------------------------------
 * Test(s) implementation for TEEC_FinalizeContext()
 * -------------------------------------------------------------------------- */

static void finalizeBadContext(void) {
  TEEC_FinalizeContext(NULL);
  CU_PASS("Check function does not crash!");
}

static void doFinalizeUnitializedContext(void) {
  TEEC_Context c;
  TEEC_FinalizeContext(&c);
}

static void finalizeUninitializedContext(void) {
  // Indirection - Checking stderr
  CU_ASSERT(
    checkCppStream(
      CPP_STREAM_ERR, doFinalizeUnitializedContext,
      TEEC_ERROR_CONTEXT_REMOVE, strlen(TEEC_ERROR_CONTEXT_REMOVE)
    )
  );
}

/* --------------------------------------------------------------------------
 * Test(s) implementation for both
 * TEEC_InitializeContext() & TEEC_FinalizeContext()
 * -------------------------------------------------------------------------- */

static void initialize2finalizeContext(void) {
  TEEC_Context c;
  TEEC_Result r;
  CU_ASSERT(TEEC_SUCCESS == (r = TEEC_InitializeContext(NULL, &c)));
  if (TEEC_SUCCESS == r) {
    TEEC_FinalizeContext(&c);
    // Redo => Check availability
    CU_ASSERT(TEEC_SUCCESS == TEEC_InitializeContext(NULL, &c));
    TEEC_FinalizeContext(&c);
  }
}

/* --------------------------------------------------------------------------
 * Test(s) implementation for TEE connection errors
 * -------------------------------------------------------------------------- */

static void doInitializeInexistentLocalCon(void) {
  TEEC_Context c;
  CU_ASSERT(TEEC_ERROR_COMMUNICATION == TEEC_InitializeContext("ERROR-1", &c));
  // Check further availability
  CU_ASSERT(TEEC_SUCCESS == TEEC_InitializeContext(NULL, &c));
  TEEC_FinalizeContext(&c);
}

static void initializeInexistentLocalCon(void) {
  // Indirection - Checking stderr
  CU_ASSERT(
    checkCppStream(
      CPP_STREAM_ERR, doInitializeInexistentLocalCon,
      TEEC_ERROR_CONNECTION, strlen(TEEC_ERROR_CONNECTION)
    )
  );
}

static void doInitializeInexistentTcpCon(void) {
  TEEC_Context c;
  CU_ASSERT(TEEC_ERROR_COMMUNICATION == TEEC_InitializeContext("ERROR-2", &c));
  // Check further availability
  CU_ASSERT(TEEC_SUCCESS == TEEC_InitializeContext(NULL, &c));
  TEEC_FinalizeContext(&c);
}

static void initializeInexistentTcpCon(void) {
  // Indirection - Checking stderr
  CU_ASSERT(
    checkCppStream(
      CPP_STREAM_ERR, doInitializeInexistentTcpCon,
      TEEC_ERROR_CONNECTION, strlen(TEEC_ERROR_CONNECTION)
    )
  );
}

// EOF
