#ifndef MANTID_DATAOBJECTS_MDFRAMESTOSPECIALCOORDINATESYTEMTEST_H_
#define MANTID_DATAOBJECTS_MDFRAMESTOSPECIALCOORDINATESYTEMTEST_H_

#include <cxxtest/TestSuite.h>

#include "MantidDataObjects/MDFramesToSpecialCoordinateSystem.h"
#include "MantidTestHelpers/FakeObjects.h"
#include "MantidGeometry/MDGeometry/QLab.h"
#include "MantidGeometry/MDGeometry/QSample.h"
#include "MantidGeometry/MDGeometry/HKL.h"
#include "MantidGeometry/MDGeometry/GeneralFrame.h"
#include "MantidKernel/MDUnit.h"
#include "MantidGeometry/MDGeometry/MDHistoDimension.h"
#include "MantidDataObjects/MDHistoWorkspace.h"

#include "boost/make_shared.hpp"

using namespace Mantid::Geometry;

class MDFramesToSpecialCoordinateSystemTest : public CxxTest::TestSuite {
public:
  void test_that_throws_for_non_md_workspace() {
    // Arrange
    boost::shared_ptr<MatrixWorkspace> ws(new WorkspaceTester());
    Mantid::DataObjects::MDFramesToSpecialCoordinateSystem converter;
    // Act + Assert
    TSM_ASSERT_THROWS(
        "Should throw as only MDEvent and MDHisto workspaces are allowed",
        converter(ws), std::invalid_argument);
  }

  void test_that_throws_for_non_uniform_coodinate_system() {
    // Arrange
    Mantid::Geometry::QLab frame1;
    Mantid::Geometry::QSample frame2;
    Mantid::coord_t min = 0;
    Mantid::coord_t max = 10;
    size_t bins = 2;
    auto dimension1 = boost::make_shared<MDHistoDimension>(
        "QLabX", "QLabX", frame1, min, max, bins);
    auto dimension2 = boost::make_shared<MDHistoDimension>(
        "QSampleY", "QSampleY", frame2, min, max, bins);
    auto ws = boost::make_shared<Mantid::DataObjects::MDHistoWorkspace>(
        dimension1, dimension2);
    Mantid::DataObjects::MDFramesToSpecialCoordinateSystem converter;

    // Act + Assert
    TSM_ASSERT_THROWS("Should throw as coordinate system is mixed.",
                      converter(ws), std::invalid_argument);
  }

  void
  test_that_returns_correct_equivalent_special_coordinate_system_for_QLab() {
    // Arrange
    Mantid::Geometry::QLab frame1;
    Mantid::Geometry::QLab frame2;
    Mantid::coord_t min = 0;
    Mantid::coord_t max = 10;
    size_t bins = 2;
    auto dimension1 = boost::make_shared<MDHistoDimension>(
        "QLabX", "QLabX", frame1, min, max, bins);
    auto dimension2 = boost::make_shared<MDHistoDimension>(
        "QLabY", "QLabY", frame2, min, max, bins);
    auto ws = boost::make_shared<Mantid::DataObjects::MDHistoWorkspace>(
        dimension1, dimension2);
    Mantid::DataObjects::MDFramesToSpecialCoordinateSystem converter;

    // Act + Assert
    Mantid::Kernel::SpecialCoordinateSystem coordinateSystem;
    TS_ASSERT_THROWS_NOTHING(coordinateSystem = converter(ws));
    TSM_ASSERT_EQUALS("Should be Qlab", coordinateSystem,
                      Mantid::Kernel::SpecialCoordinateSystem::QLab);
  }

  void
  test_that_returns_correct_equivalent_special_coordinate_system_for_QSample() {
    // Arrange
    Mantid::Geometry::QSample frame1;
    Mantid::Geometry::QSample frame2;
    Mantid::coord_t min = 0;
    Mantid::coord_t max = 10;
    size_t bins = 2;
    auto dimension1 = boost::make_shared<MDHistoDimension>(
        "QSampleX", "QSampleX", frame1, min, max, bins);
    auto dimension2 = boost::make_shared<MDHistoDimension>(
        "QSampleY", "QSampleY", frame2, min, max, bins);
    auto ws = boost::make_shared<Mantid::DataObjects::MDHistoWorkspace>(
        dimension1, dimension2);
    Mantid::DataObjects::MDFramesToSpecialCoordinateSystem converter;

    // Act + Assert
    Mantid::Kernel::SpecialCoordinateSystem coordinateSystem;
    TS_ASSERT_THROWS_NOTHING(coordinateSystem = converter(ws));
    TSM_ASSERT_EQUALS("Should be QSample", coordinateSystem,
                      Mantid::Kernel::SpecialCoordinateSystem::QSample);
  }

  void
  test_that_returns_correct_equivalent_special_coordinate_system_for_HKL() {
    // Arrange
    Mantid::Geometry::HKL frame1(new Mantid::Kernel::ReciprocalLatticeUnit);
    Mantid::Geometry::HKL frame2(new Mantid::Kernel::ReciprocalLatticeUnit);
    Mantid::coord_t min = 0;
    Mantid::coord_t max = 10;
    size_t bins = 2;
    auto dimension1 =
        boost::make_shared<MDHistoDimension>("H", "H", frame1, min, max, bins);
    auto dimension2 =
        boost::make_shared<MDHistoDimension>("K", "K", frame2, min, max, bins);
    auto ws = boost::make_shared<Mantid::DataObjects::MDHistoWorkspace>(
        dimension1, dimension2);
    Mantid::DataObjects::MDFramesToSpecialCoordinateSystem converter;

    // Act + Assert
    Mantid::Kernel::SpecialCoordinateSystem coordinateSystem;
    TS_ASSERT_THROWS_NOTHING(coordinateSystem = converter(ws));
    TSM_ASSERT_EQUALS("Should be HKL", coordinateSystem,
                      Mantid::Kernel::SpecialCoordinateSystem::HKL);
  }

  void
  test_that_returns_correct_equivalent_special_coordinate_system_for_GeneralFrame() {
    // Arrange
    Mantid::Geometry::GeneralFrame frame1("a", "b");
    Mantid::Geometry::GeneralFrame frame2("a", "b");
    Mantid::coord_t min = 0;
    Mantid::coord_t max = 10;
    size_t bins = 2;
    auto dimension1 =
        boost::make_shared<MDHistoDimension>("H", "H", frame1, min, max, bins);
    auto dimension2 =
        boost::make_shared<MDHistoDimension>("K", "K", frame2, min, max, bins);
    auto ws = boost::make_shared<Mantid::DataObjects::MDHistoWorkspace>(
        dimension1, dimension2);
    Mantid::DataObjects::MDFramesToSpecialCoordinateSystem converter;

    // Act + Assert
    Mantid::Kernel::SpecialCoordinateSystem coordinateSystem;
    TS_ASSERT_THROWS_NOTHING(coordinateSystem = converter(ws));
    TSM_ASSERT_EQUALS("Should be None", coordinateSystem,
                      Mantid::Kernel::SpecialCoordinateSystem::None);
  }
};

#endif