#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>

#include "Length_Indicated_ZipCodeBuffer.h"
#include "BlockBuffer.h"
#include "BlockedSequence.h"

void testBlockBuffer ()
{

    string zip = "43740";
    std::string state = "MN";
    double longi = 42.00;
    double lat = 65.00;
    
    // Block::block(0, 0);

    ZipCodeRecord record(zip, state, lat, longi);
    BlockBuffer Buff;
    Length_Indicated_ZipCodeBuffer LIrecBuff1("data/Length_Indicated_us_postal_codes.csv");
    // Block :: InitBuffer (Buff); // Unnecessary


    // Record Object test (ZipCodeRecord)

    cout << record.getZip() << ", " << record.getState() << ", " << record.getLatitude() <<  ", " << record.getLongitude();

    // Block Buffer Test

    
}



void testBlockedSequence () 
{

    
    Length_Indicated_ZipCodeBuffer LIBuff2("data/Length_Indicated_us_postal_codes.csv");
    BlockedSequence sequence;
    ZipCodeRecord record;

    while(LIBuff2.readNext(record))
    {    
        sequence.AppendRecord(LIBuff2.)
    }
    
    
}



int main() {


    // testBlockBuffer();





return 0;

}
