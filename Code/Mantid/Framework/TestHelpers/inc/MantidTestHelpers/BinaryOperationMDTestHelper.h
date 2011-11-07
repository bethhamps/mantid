#ifndef MANTID_MDALGORITHMS_BINARYOPERATIONMDTESTHELPER_H_
#define MANTID_MDALGORITHMS_BINARYOPERATIONMDTESTHELPER_H_

#include "MantidMDEvents/MDHistoWorkspace.h"

namespace BinaryOperationMDTestHelper
{
  /// Run a binary algorithm.
  Mantid::MDEvents::MDHistoWorkspace_sptr doTest(std::string algoName, std::string lhs, std::string rhs, std::string outName,
      bool succeeds=true);

} // (end namespace)

#endif
