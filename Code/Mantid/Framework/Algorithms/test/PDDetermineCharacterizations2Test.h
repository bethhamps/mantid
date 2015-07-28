#ifndef MANTID_ALGORITHMS_PDDETERMINECHARACTERIZATIONS2TEST_H_
#define MANTID_ALGORITHMS_PDDETERMINECHARACTERIZATIONS2TEST_H_

#include <cxxtest/TestSuite.h>

#include "MantidAlgorithms/PDDetermineCharacterizations2.h"

using Mantid::Algorithms::PDDetermineCharacterizations2;
using namespace Mantid::API;

class PDDetermineCharacterizations2Test : public CxxTest::TestSuite {
public:
  // This pair of boilerplate methods prevent the suite being created statically
  // This means the constructor isn't called when running other tests
  static PDDetermineCharacterizations2Test *createSuite() { return new PDDetermineCharacterizations2Test(); }
  static void destroySuite( PDDetermineCharacterizations2Test *suite ) { delete suite; }


  void test_Init()
  {
    PDDetermineCharacterizations2 alg;
    TS_ASSERT_THROWS_NOTHING( alg.initialize() )
    TS_ASSERT( alg.isInitialized() )
  }

  void test_exec()
  {
    // Name of the output workspace.
    std::string outWSName("PDDetermineCharacterizations2Test_OutputWS");

    PDDetermineCharacterizations2 alg;
    TS_ASSERT_THROWS_NOTHING( alg.initialize() )
    TS_ASSERT( alg.isInitialized() )
    TS_ASSERT_THROWS_NOTHING( alg.setPropertyValue("REPLACE_PROPERTY_NAME_HERE!!!!", "value") );
    TS_ASSERT_THROWS_NOTHING( alg.setPropertyValue("OutputWorkspace", outWSName) );
    TS_ASSERT_THROWS_NOTHING( alg.execute(); );
    TS_ASSERT( alg.isExecuted() );

    // Retrieve the workspace from data service. TODO: Change to your desired type
    Workspace_sptr ws;
    TS_ASSERT_THROWS_NOTHING( ws = AnalysisDataService::Instance().retrieveWS<Workspace>(outWSName) );
    TS_ASSERT(ws);
    if (!ws) return;

    // TODO: Check the results

    // Remove workspace from the data service.
    AnalysisDataService::Instance().remove(outWSName);
  }
  
  void test_Something()
  {
    TSM_ASSERT( "You forgot to write a test!", 0);
  }


};


#endif /* MANTID_ALGORITHMS_PDDETERMINECHARACTERIZATIONS2TEST_H_ */