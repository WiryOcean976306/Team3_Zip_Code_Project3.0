/**
 * @file HeaderRecord.cpp
 * @author Zach Houck
 * @date 2026-02-11
 * @brief Implements HeaderRecord, which builds a serialized header record for the length-indicated file format.
 */

#ifndef HEADERRECORD_CPP
#define HEADERRECORD_CPP

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>

using namespace std;

#include "../include/HeaderRecord.h"

string operator+(const string& s, int value)
{
    // Convenience overload used by this file when assembling comma-separated metadata.
    return s + to_string(value);
}

string operator+(int value, const string& s) 
{
    // Convenience overload used by this file when assembling comma-separated metadata.
    return to_string(value) + s;
}

void HeaderRecord::RebuildHeader()
{
    Header.clear();

    for (int i = 0; i < FieldCount; i++)
    {
        Header += Fields[i] + "," + FieldTypes[i] + ",";
    }

    string sizeFormatTypeString = SizeFormatType ? "Binary" : "ASCII";
    string sizeInclusionString = SizeInclusion ? "TRUE" : "FALSE";

    ostringstream body;
    body << FileStructureType << ','
         << Version << ','
         << sizeFormatTypeString << ','
         << sizeInclusionString << ','
         << PrimaryKeyIndex << ','
         << RecordCount << ','
         << FieldCount << ','
         << Header << PrimaryKey;

    Header = body.str();

    size_t size = Header.size();
    HeaderSizeBytes = static_cast<int>(size);
    if (SizeInclusion)
    {
        HeaderSizeBytes += 4;
    }

    Header = to_string(HeaderSizeBytes) + "," + Header;
}

/**
 * @brief Builds the serialized header record from static schema values and CSV-derived record count.
 * @param InFile Path to the source CSV file used to compute record count.
 */
HeaderRecord::HeaderRecord(const string& InFile)
{
    ifstream OldCSV(InFile);
    if(!OldCSV) 
    {
        cout << "Error: could not open file\n";
        return;
    }
    
    RecordCount = -1; // Start at -1 so the CSV header row is not counted as a data record.
    string line;
    while(getline(OldCSV, line))
        ++RecordCount;
    RebuildHeader();
}

/**
 * @brief Returns the generated header line.
 * @return Serialized header record string.
 */
string HeaderRecord::GetHeader() const
{
    return Header;
}

 #endif