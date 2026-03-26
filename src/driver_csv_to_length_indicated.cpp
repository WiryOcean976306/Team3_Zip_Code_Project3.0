/**
*@file driver_csv_to_length_indicated.cpp
*@author Ken, Zach, Gabriel, Robert
*@date 2026-02-10
*@brief driver for csv to length indicated file
 */

#include <iostream>
#include <fstream>
#include <string>
#include "../include/CsvToLength.h"

using namespace std;

int main() {

    CsvToLength Order("data/us_postal_codes.csv");
    cout << "Success!";
    return 0;
}



//     Analyzer analyzer;
//     ZipCodeBuffer buffer("data/fixed_us_postal_codes.csv");
//     Printer printer;
//     PrinterCSV printerCSV;
//     ZipCodeRecord r;

//     std::ofstream outFile1("zipcodes_len.dat");
//     std::ofstream outFile2("rand_zipcodes_len.dat");

    
//     while(buffer.readNext(r))
//     {
//         analyzer.consume(r);
//     }

//     Analyzer analyzer2;
//     ZipCodeBuffer buffer2("data/us_postal_codes_ROWS_RANDOMIZED.csv");
//     Printer printer2;
//     PrinterCSV printerCSV2;
//     ZipCodeRecord r2;

    
//     while(buffer2.readNext(r2))
//     {
//         analyzer2.consume(r2);
//     }
        
//    printerCSV.printCSV(outFile1, analyzer.getResults());
//    printerCSV2.printCSV(outFile2, analyzer2.getResults());

//     outFile1.close();
//     outFile2.close();
//