/**
*@file SearchIndex.cpp
*@author Zach
*@date 2026-02-10
*@brief SearchIndex.cpp searches for a zip code in the length indicated file and prints the record to the console.
        Compile and run with the command: make && ./bin/main -Z<zip>
 */

#include <iostream>
#include <fstream>
#include "analyzer.h"
#include "Length_Indicated_ZipCodeBuffer.h"
#include "PrimaryKeyIndex.h"
#include "Printer.h"
#include "PrinterCSV.h"


int main(int argc, char* argv[]) {

    PrimaryKeyIndex pki("data/Length_Indicated_us_postal_codes.csv");

    if (argc <= 1)
    {
        std::cout << "Usage: " << argv[0] << " -Z<zip> [-Z<zip> ...]\n";
        return 0;
    }

    vector<string> args;
    for (int i = 1; i < argc; ++i)
    {
        args.emplace_back(argv[i]);
    }

    pki.find(args);

    return 0;
}
