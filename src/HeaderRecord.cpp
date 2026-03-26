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

    for(int i = 0; i < FieldCount; i++)
        Header += Fields[i] + "," + FieldTypes[i] + ",";

    string HeaderFront = "";
    string comma = ",";
    string SizeFormatTypeString;
    string SizeInclutionString;

    if(SizeFormatType)
        SizeFormatTypeString = "Binary";
    else
        SizeFormatTypeString = "ASCII";

    if(SizeInclusion)
        SizeInclutionString = "TRUE";
    else
        SizeInclutionString = "FALSE";

    HeaderFront += FileStructureType + comma +
                   Version + comma +
                   SizeFormatTypeString + comma +
                   SizeInclutionString + comma +
                   PrimaryKeyIndex + comma +
                   RecordCount + comma +
                   FieldCount + comma;              //https://www.youtube.com/watch?v=-teyuf0rARk

    Header = string(",") + HeaderFront + Header + PrimaryKey;

    if(SizeFormatType) //binary count
    {
        
    }
    else //ASCII count
    {
        size_t size = Header.size();
        HeaderSizeBytes = static_cast<int>(size);
        if(SizeInclusion)
            HeaderSizeBytes += 4;
    }

    Header = to_string(HeaderSizeBytes) + Header;
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