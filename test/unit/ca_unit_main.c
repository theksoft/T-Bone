/*
   Main file for T-Bone TEE client library unitary testing
    + Aggregate all test suites to manage
    + Define main entry point
*/

/* --------------------------------------------------------------------------
 * Test suites
 * -------------------------------------------------------------------------- */

#include "ca_unit_tests.h"

static tCuwSuiteGetter testSuites[] = {
  getCAUnitContextSuite,
  CUW_SUITE_END     // End of test suite
};

/* --------------------------------------------------------------------------
 * Test program
 * -------------------------------------------------------------------------- */

int main(int argc, char* argv[]) {
  int e = 0;
  tCuwContext c;
  if (0 != (e = cuwGetContext(&c, argc, argv)))
    return (1 == e) ? EXIT_SUCCESS : EXIT_FAILURE;
  return (!cuwProcess(&c, testSuites, NULL)) ? EXIT_FAILURE : EXIT_SUCCESS;
}

//EOF
