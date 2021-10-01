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

#include "tee_client_api.h"
#include "ac_context.hpp"
#include "ac_tee.hpp"
#include "tb_errors.hpp"
#include <cassert>
#include <iostream>

// Just get a unique 32-bit ID
static uint32_t getMagic() {
  return (uint32_t)(uintptr_t)getMagic;
}

// __TEEC_Configure entry point
// -----------------------------------------------------------------------------

extern "C"
TEEC_Result __TEEC_Configure(const char* filename) {
  using namespace tbone::client;
  return (TeeMap::get().create(filename)) ? TEEC_SUCCESS : TEEC_ERROR_GENERIC;
}

// TEEC_InitializeContext entry point
// -----------------------------------------------------------------------------

extern "C"
TEEC_Result TEEC_InitializeContext(const char *name, TEEC_Context *context) {
  using namespace tbone::client;

  // Checking method context & arguments
  if (!context) {
    return TEEC_ERROR_BAD_PARAMETERS;
  }

  if (TeeContextMap::get().match(context)) {
    std::cerr << TEEC_ERROR_CONTEXT_REINIT << " " << std::hex << context << std::endl;
    return TEEC_ERROR_GENERIC;
  }

  // Looks for TEE name in available configured TEE
  Tee* tee = TeeMap::get().match(name);
  if (!tee) {
    return TEEC_ERROR_ITEM_NOT_FOUND;
  }

  // Add a new context
  TeeContext *c = TeeContext::create(context, name);
  if (NULL == c) {
    return TEEC_ERROR_GENERIC;
  }

  // Connect TEE
  if (!c->connect()) {
    delete c;
    return TEEC_ERROR_COMMUNICATION;
  }

  // Assign it
  context->magic = getMagic();
  context->data = reinterpret_cast<void*>(c);

  return TEEC_SUCCESS;
}

// TEEC_FinalizeContext entry point
// -----------------------------------------------------------------------------

extern "C"
void TEEC_FinalizeContext(TEEC_Context *context) {
  using namespace tbone::client;

  // Check parameters
  if (!context) {
    return;
  }

  if (!context->data || getMagic() != context->magic) {
    std::cerr << TEEC_ERROR_CONTEXT_REMOVE << " " << std::hex << context << std::endl;
    return;
  }

  // Reinterpret imp field
  TeeContext *c = NULL;
  try {
    c = static_cast<TeeContext*>(context->data);
    assert(*c == context);
    delete c;
  } catch(...) {
    std::cerr << TEEC_ERROR_CONTEXT_REMOVE << " " << std::hex << context << std::endl;
    return;
  }

  context->magic = 0;
  context->data = NULL;
}

// EOF
