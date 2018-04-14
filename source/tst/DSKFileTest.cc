#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE DSKFile test
#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test.hpp>

#include <cstdint>
#include <set>
#include <vector>

#include "DSKFile.h"

using namespace std;

BOOST_AUTO_TEST_CASE(constructors_test)
{
    DSKFile file;
    file.load(boost::unit_test::framework::master_test_suite().argv[1]);
}

// EOF
// vim: et:sw=4:ts=4

