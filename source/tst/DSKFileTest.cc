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

    if (file.stdMagicOk) {
        cout << "Standard DSK file detected." << endl;
    } else if (file.extMagicOk) {
        cout << "Extended DSK file detected." << endl;
    } else {
        cout << "Not a valid DSK file." << endl;
    }

    for (uint32_t ii = 0; ii < file.trackSizeTable.size(); ++ii) {
        cout << "Track " << ii << ": " << file.trackSizeTable[ii] << " - ";
        for (uint32_t jj = 0; jj < file.tracks[ii].sectors.size(); ++jj) {
            cout << static_cast<uint32_t>(file.tracks[ii].sectors[jj].sectorSize) << " ";
        }
        cout << endl;
    }
}

// EOF
// vim: et:sw=4:ts=4

