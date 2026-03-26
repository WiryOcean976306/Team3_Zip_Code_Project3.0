/**
 * @file PrimaryKeyIndex.cpp
 * @author Zach Houck
 * @date 2026-03-14
 * @brief Implements a primary key index that can be used to display the Zip Code data for all Zip Codes listed on the command line.
 */

#ifndef  PRIMARYKEYINDEX_CPP
#define  PRIMARYKEYINDEX_CPP

#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include "PrimaryKeyIndex.h"
#include "ZipCodeRecord.h"

using namespace std;

bool PrimaryKeyIndex::WriteToFile(const string& path)
{
    filesystem::path p(path);
    string filename = p.filename().string();

    ofstream Out("PKI/PKI_" + filename, ios::out | ios::trunc);
    if (!Out.is_open())
    {
        return false;
    }

    for (const auto& pair : Index)
    {
        Out << pair.first << "," << pair.second << "\n";
    }

    Out.close();
    Index.clear();
    return true;
}

bool PrimaryKeyIndex::ReadFromFile(const string& path)
{
    Index.clear();

    filesystem::path p(path);
    string filename = p.filename().string();

    ifstream InFile("PKI/PKI_" + filename);
    if (!InFile.is_open())
    {
        return false;
    }

    string line;
    while (getline(InFile, line))
    {
        string zip;
        string byteStr;
        int byteCount = 0;

        stringstream ss(line);
        if (!getline(ss, zip, ','))
            continue;
        if (!getline(ss, byteStr, ','))
            continue;

        try
        {
            byteCount = stoi(byteStr);
        }
        catch (...) {}

        Index.insert({zip, byteCount});
    }

    InFile.close();
    return true;
}

PrimaryKeyIndex::PrimaryKeyIndex(const string& FilePath)
{
    File = FilePath;
    ifstream InFile;
    InFile.open(FilePath);

    int ByteCount, b;
    string Zip, line, column;
    getline(InFile, line);
    header = line;
    stringstream ss(line);
    ss >> ByteCount;

    while(getline(InFile, line))
    {
        stringstream ss(line);
        getline(ss, column, ',');
        stringstream byte(column);
        getline(ss, column, ',');
        byte >> b;
        Zip = column;
        Index.insert({Zip, ByteCount});
        ByteCount += b;
    }

    WriteToFile(File);

    Index.clear();

    InFile.close();
}

ZipCodeRecord PrimaryKeyIndex::find(vector<string> Zips)
{
    ZipCodeRecord found;
    bool foundAny = false;

    vector<string> zipCodes;
    for (size_t i = 0; i < Zips.size(); ++i)
    {
        const string& arg = Zips[i];
        if (arg.rfind("-Z", 0) == 0 || arg.rfind("-z", 0) == 0)
        {
            if (arg.size() > 2)
            {
                zipCodes.push_back(arg.substr(2));
            }
            else if (i + 1 < Zips.size())
            {
                zipCodes.push_back(Zips[++i]);
            }
        }
        else
        {
            zipCodes.push_back(arg);
        }
    }

    if (!ReadFromFile(File))
    {
        cout << "Primary key index not found for file: " << File << "\n";
        return found;
    }

    ifstream InFile(File, ios::binary);
    if (!InFile.is_open())
    {
        cout << "Could not open data file: " << File << "\n";
        return found;
    }

    for (const string& Zip : zipCodes)
    {
        auto it = Index.find(Zip);
        if (it == Index.end())
        {
            cout << "Zip Code " << Zip << " not found.\n";
            continue;
        }

        int offset = it->second;
        InFile.clear();
        InFile.seekg(offset, ios::beg);
        if (!InFile.good())
        {
            cout << "Failed to seek to offset " << offset << " for zip " << Zip << "\n";
            continue;
        }

        string line;
        if (!getline(InFile, line))
        {
            cout << "Failed to read record at offset " << offset << " for zip " << Zip << "\n";
            continue;
        }

        string lengthStr, zipStr, city, state, county, latStr, longStr;
        stringstream ss(line);
        getline(ss, lengthStr, ',');
        getline(ss, zipStr, ',');
        getline(ss, city, ',');
        getline(ss, state, ',');
        getline(ss, county, ',');
        getline(ss, latStr, ',');
        getline(ss, longStr, ',');

        double latitude = 0.0;
        double longitude = 0.0;
        try { latitude = stod(latStr); } catch (...) {}
        try { longitude = stod(longStr); } catch (...) {}

        ZipCodeRecord record(zipStr, state, latitude, longitude);

        cout << record.getZip() << ", " << record.getState() << ", "
             << record.getLatitude() << ", " << record.getLongitude() << "\n";

        if (!foundAny)
        {
            found = record;
            foundAny = true;
        }
    }

    InFile.close();
    return found;
}

#endif