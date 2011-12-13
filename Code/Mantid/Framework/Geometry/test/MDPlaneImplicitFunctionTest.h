#ifndef MANTID_MDGEOMETRY_MDIMPLICITFUNCTIONTEST_H_
#define MANTID_MDGEOMETRY_MDIMPLICITFUNCTIONTEST_H_

#include "MantidGeometry/MDGeometry/MDPlaneImplicitFunction.h"
#include "MantidGeometry/MDGeometry/MDPlane.h"
#include <cxxtest/TestSuite.h>

using namespace Mantid::Geometry;
using namespace Mantid;

class MDPlaneImplicitFunctionTest : public CxxTest::TestSuite
{
public:
  void test_addPlane()
  {
    MDPlaneImplicitFunction f;

    coord_t normal[3] = {1.234, 4.56, 6.78};
    coord_t point[3] = {1,2,3};
    MDPlane p1(3, normal, point);
    MDPlane p2(3, normal, point);

    TS_ASSERT_EQUALS( f.getNumDims(), 0 );
    TS_ASSERT_THROWS_NOTHING( f.addPlane(p1) );
    TS_ASSERT_EQUALS( f.getNumDims(), 3 );
    TS_ASSERT_THROWS_ANYTHING( f.addPlane(p2) );
    TS_ASSERT_EQUALS( f.getNumPlanes(), 1 );
  }
};

#endif // MANTID_MDGEOMETRY_MDIMPLICITFUNCTIONTEST_H_
