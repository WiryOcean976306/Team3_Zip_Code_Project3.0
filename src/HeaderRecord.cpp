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
            << Header << PrimaryKey << ','
            << BlockSize << ','
            << MinBlockCapacity << ','
            << BlockCount << ','
            << availListHeadRBN << ','
            << ActiveListHeadRBN << ','
            << (StaleFlag ? "TRUE" : "FALSE");

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

void HeaderRecord::ConfigureBlockedSequenceHeader(
    const string& indexFileName,
    int recordCount,
    int blockCount,
    int blockSize,
    int minBlockCapacity,
    int availHeadRBN,
    int activeHeadRBN,
    bool staleFlag)
{
    FileStructureType = "BLOCKED_SEQUENCE_SET";
    Version = "3.0";
    SizeFormatType = 0;
    SizeInclusion = true;
    PrimaryKeyIndex = indexFileName;
    RecordCount = recordCount;

    // Project 3.0 record schema used by generated block payloads.
    FieldCount = 4;
    Fields = {"ZipCode", "State", "Lat", "Long"};
    FieldTypes = {"STRING", "STRING", "FLOAT", "FLOAT"};
    PrimaryKey = 1;

    BlockSize = blockSize;
    MinBlockCapacity = minBlockCapacity;
    BlockCount = blockCount;
    availListHeadRBN = availHeadRBN;
    ActiveListHeadRBN = activeHeadRBN;
    StaleFlag = staleFlag;

    RebuildHeader();
}

 #endif