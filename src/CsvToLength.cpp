/**
 * @file CsvToLength.cpp
 * @author Zach Houck
 * @date 2026-02-10
 * @brief Implements CSV-to-length-indicated conversion logic.
 */

#ifndef  CSVTOLENGTH_CPP
#define  CSVTOLENGTH_CPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <memory>
#include "../include/CsvToLength.h"
#include "../include/HeaderRecord.h"

using namespace std;

CsvToLength::CsvToLength(const string& FilePath) 
{
    ifstream InFile;
    InFile.open(FilePath);

    // Keep only the source filename so the output can be written to the data folder
    // with a predictable prefix.
    filesystem::path p(FilePath);
    string filename = p.filename().string();

    ofstream Out("data/Length_Indicated_" + filename);

    // Write the generated metadata header as the first record in the output file.
    HeaderRecord NewHeader(FilePath);
    Out << NewHeader.GetHeader() << "\n";
    
    string line, column;
    int digitCount;
    getline(InFile, line); // Skip the original CSV header row.
    while(getline(InFile, line))
    {
        // Current wire format: <recordLength>,<comma-separated payload>

        // Credit to Ken and Gabriel for inspiration from ZipCodeBuffer.cpp
        stringstream ss(line);
        getline(ss, column, ',');
        digitCount = column.size();
        if(digitCount == 5)
        {}
        else if(digitCount == 4)
        {
            line = string("0") + line;
        }
        else if(digitCount == 3)
        {
            line = "00" + line;
        }

        line += "\n";
        Out << line.size() + 3 << "," << line;
    }
}

#endif 