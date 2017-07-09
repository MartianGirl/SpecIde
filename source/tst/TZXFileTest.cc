#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TZXFile test
#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test.hpp>

#include <cstdint>
#include <set>
#include <vector>

#include "TZXFile.h"

using namespace std;

BOOST_AUTO_TEST_CASE(constructors_test)
{
    TZXFile tzxFile;
    tzxFile.load("TheTrapdoor.tzx");
    BOOST_CHECK_EQUAL(tzxFile.fileData[0], 'Z');
    BOOST_CHECK_EQUAL(tzxFile.fileData[1], 'X');
    BOOST_CHECK_EQUAL(tzxFile.fileData[2], 'T');
    BOOST_CHECK_EQUAL(tzxFile.fileData[3], 'a');
    BOOST_CHECK_EQUAL(tzxFile.fileData[4], 'p');
    BOOST_CHECK_EQUAL(tzxFile.fileData[5], 'e');
    BOOST_CHECK_EQUAL(tzxFile.fileData[6], '!');
    BOOST_CHECK_EQUAL(tzxFile.fileData[7], 0x1A);
}

BOOST_AUTO_TEST_CASE(block_parsing_test)
{
    vector<size_t> pulseData;
    set<size_t> indexData;
    set<size_t> stopData;

    TZXFile tzx;
    tzx.load("ThreeWeeksInParadise.tzx");
    tzx.parse(pulseData, indexData, stopData);
}

// EOF
// vim: et:sw=4:ts=4

