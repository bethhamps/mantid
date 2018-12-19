// Mantid Repository : https://github.com/mantidproject/mantid
//
// Copyright &copy; 2018 ISIS Rutherford Appleton Laboratory UKRI,
//     NScD Oak Ridge National Laboratory, European Spallation Source
//     & Institut Laue - Langevin
// SPDX - License - Identifier: GPL - 3.0 +
#ifndef MANTID_GEOMETRY_PEAKTRANSFORMHKL_H_
#define MANTID_GEOMETRY_PEAKTRANSFORMHKL_H_

#include "MantidGeometry/Crystal/ConcretePeakTransformFactory.h"
#include "MantidGeometry/Crystal/PeakTransform.h"
#include "MantidKernel/System.h"

namespace Mantid {
namespace Geometry {
/**
@class PeakTransformHKL
Used to remap coordinates into a form consistent with an axis reordering.
*/
class DLLExport PeakTransformHKL : public PeakTransform {
public:
  static std::string name() { return "HKL"; }
  /// Constructor
  PeakTransformHKL();
  /// Constructor
  PeakTransformHKL(const std::string &xPlotLabel,
                   const std::string &yPlotLabel);
  /// Virtual constructor
  PeakTransform_sptr clone() const override;
  /// Transform peak.
  Mantid::Kernel::V3D
  transformPeak(const Mantid::Geometry::IPeak &peak) const override;
  /// Getter for a friendly name to describe the transform type.
  std::string getFriendlyName() const override { return name(); }
  /// Getter for the special coordinate representation of this transform type.
  Mantid::Kernel::SpecialCoordinateSystem getCoordinateSystem() const override;
};

/// Typedef a factory for type of PeaksTransform.
using PeakTransformHKLFactory = ConcretePeakTransformFactory<PeakTransformHKL>;
} // namespace Geometry
} // namespace Mantid

#endif /* MANTID_GEOMETRY_PEAKTRANSFORMHKL_H_ */
