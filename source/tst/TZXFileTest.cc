#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TZXFile test
#include <boost/test/unit_test.hpp>
//#include <boost/test/included/unit_test.hpp>

#include <cstdint>
#include <vector>

#include "TZXFile.h"

using namespace std;

BOOST_AUTO_TEST_CASE(constructors_test)
{
    TZXFile tzxFile;
    bool success = tzxFile.load("TheTrapdoor.tzx");
    BOOST_CHECK_EQUAL(success, true);
    BOOST_CHECK_EQUAL(tzxFile.data[0], 'Z');
    BOOST_CHECK_EQUAL(tzxFile.data[1], 'X');
    BOOST_CHECK_EQUAL(tzxFile.data[2], 'T');
    BOOST_CHECK_EQUAL(tzxFile.data[3], 'a');
    BOOST_CHECK_EQUAL(tzxFile.data[4], 'p');
    BOOST_CHECK_EQUAL(tzxFile.data[5], 'e');
    BOOST_CHECK_EQUAL(tzxFile.data[6], '!');
    BOOST_CHECK_EQUAL(tzxFile.data[7], 0x1A);
}

BOOST_AUTO_TEST_CASE(block_parsing_test)
{
    TZXFile tzx;
    bool success = tzx.load("ThreeWeeksInParadise.tzx");
    BOOST_CHECK_EQUAL(success, true);

    if (success)
    {
        while (tzx.isLastBlock() == false)
        {
            tzx.getBlock();
            tzx.dumpBlockInfo();
            tzx.advance();
        }

        tzx.rewind();
        while (tzx.isLastBlock() == false)
        {
            tzx.getBlock();
            tzx.dumpBlockInfo();
            tzx.advance();
        }
    }
}

// EOF
// vim: et:sw=4:ts=4

