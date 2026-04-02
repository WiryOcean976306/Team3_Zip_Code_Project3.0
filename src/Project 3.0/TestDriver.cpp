#include <fstream>
#include <string>
#include <iomanip>

#include "ZipCodeRecord.h"
#include "BlockBuffer.h"

void testBlockBuffer ()
{

    string zip = "43740";
    std::string state = "MN";
    double longi = 42.00;
    double lat = 65.00;
    
    ZipCodeRecord record(zip, state, lat, longi);
    BlockBuffer Buff;
    // Block :: InitBuffer (Buff); // Unnecessary

    cout << record.getState() << ", " << record.getLatitude() <<  ", " << record.getLongitude();


       

}


int main() {


    testBlockBuffer();



return 1;

}
