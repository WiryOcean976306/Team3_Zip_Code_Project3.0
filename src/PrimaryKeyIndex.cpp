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
#include <algorithm>

#include "PrimaryKeyIndex.h"
#include "ZipCodeRecord.h"

using namespace std;

namespace {

/**
 * @brief Checks whether a path matches the expected block filename pattern.
 * @param path Candidate filesystem path.
 * @return true for files named block_<rbn>.blk; otherwise false.
 */
bool IsBlockFile(const filesystem::path& path)
{
    return path.extension() == ".blk" && path.filename().string().rfind("block_", 0) == 0;
}

/**
 * @brief Extracts the numeric RBN suffix from a block filename.
 * @param path Block file path with stem block_<rbn>.
 * @return Parsed RBN value, or -1 when parsing fails.
 */
int ExtractRbnFromFileName(const filesystem::path& path)
{
    string stem = path.stem().string();
    size_t underscore = stem.find('_');
    if (underscore == string::npos)
    {
        return -1;
    }

    try
    {
        return stoi(stem.substr(underscore + 1));
    }
    catch (...)
    {
        return -1;
    }
}

/**
 * @brief Parses ZIP code key from one packed record string.
 * @param packedRecord Packed record in format <len>,<zip>,...
 * @param zipOut Output ZIP key.
 * @return true when both separators are found and key extraction succeeds.
 */
bool ParsePackedZip(const string& packedRecord, string& zipOut)
{
    size_t firstComma = packedRecord.find(',');
    if (firstComma == string::npos)
    {
        return false;
    }

    size_t secondComma = packedRecord.find(',', firstComma + 1);
    if (secondComma == string::npos)
    {
        return false;
    }

    zipOut = packedRecord.substr(firstComma + 1, secondComma - firstComma - 1);
    return true;
}

} // namespace

/**
 * @brief Extracts ZIP key text from one packed record.
 * @param packedRecord Packed record string.
 * @return ZIP key text, or empty string when parsing fails.
 */
string PrimaryKeyIndex::ExtractZipKey(const string& packedRecord)
{
    string zip;
    if (!ParsePackedZip(packedRecord, zip))
    {
        return "";
    }
    return zip;
}

/**
 * @brief Builds the on-disk block file path for one RBN.
 * @param rbn Relative block number.
 * @return Full path under the configured block directory.
 */
string PrimaryKeyIndex::GetBlockFilePath(int rbn) const
{
    return BlockDirectory + string("/block_") + to_string(rbn) + string(".blk");
}

/**
 * @brief Writes the current simple index map to disk.
 * @param path Output index file path.
 * @return true when write succeeds; otherwise false.
 */
bool PrimaryKeyIndex::WriteToFile(const string& path)
{
    filesystem::path outPath(path);
    if (outPath.has_parent_path())
    {
        filesystem::create_directories(outPath.parent_path());
    }

    ofstream Out(path, ios::out | ios::trunc);
    if (!Out.is_open())
    {
        return false;
    }

    Out << Dump();

    Out.close();
    return true;
}

/**
 * @brief Builds an in-memory simple index from physical block files.
 * @param blockDirectory Directory containing block_<rbn>.blk files.
 * @return true when directory scan and index build complete; otherwise false.
 * @details Stores pairs <highest ZIP key in block, RBN> for active blocks.
 */
bool PrimaryKeyIndex::BuildFromBlocks(const string& blockDirectory)
{
    Index.clear();
    BlockDirectory = blockDirectory;

    filesystem::path dir(blockDirectory);
    if (!filesystem::exists(dir) || !filesystem::is_directory(dir))
    {
        return false;
    }

    vector<filesystem::path> blockFiles;
    for (const auto& entry : filesystem::directory_iterator(dir))
    {
        if (entry.is_regular_file() && IsBlockFile(entry.path()))
        {
            blockFiles.push_back(entry.path());
        }
    }

    sort(blockFiles.begin(), blockFiles.end(), [](const filesystem::path& left, const filesystem::path& right)
    {
        return ExtractRbnFromFileName(left) < ExtractRbnFromFileName(right);
    });

    for (const auto& path : blockFiles)
    {
        ifstream in(path);
        if (!in.is_open())
        {
            continue;
        }

        string headerLine;
        if (!getline(in, headerLine))
        {
            continue;
        }

        stringstream hs(headerLine);
        string token;
        vector<int> values;
        while (getline(hs, token, ','))
        {
            try
            {
                values.push_back(stoi(token));
            }
            catch (...)
            {
                values.clear();
                break;
            }
        }

        if (values.size() < 2)
        {
            continue;
        }

        int recordCount = values[0];
        int rbn = values[1];
        if (recordCount <= 0)
        {
            continue;
        }

        string recordLine;
        string highestZip;
        while (getline(in, recordLine))
        {
            if (recordLine.empty())
            {
                continue;
            }

            string zip;
            if (ParsePackedZip(recordLine, zip) && !zip.empty())
            {
                highestZip = zip;
            }
        }

        if (!highestZip.empty())
        {
            Index[highestZip] = rbn;
        }
    }

    return true;
}

/**
 * @brief Reads a previously written simple index file into memory.
 * @param path Index file path.
 * @return true when parse succeeds; otherwise false.
 */
bool PrimaryKeyIndex::ReadFromFile(const string& path)
{
    Index.clear();

    ifstream InFile(path);
    if (!InFile.is_open())
    {
        return false;
    }

    string line;
    while (getline(InFile, line))
    {
        stringstream ss(line);
        string zip;
        string rbnStr;
        if (!getline(ss, zip, ','))
            continue;
        if (!getline(ss, rbnStr, ','))
            continue;

        try
        {
            Index[zip] = stoi(rbnStr);
        }
        catch (...)
        {
            continue;
        }
    }

    InFile.close();
    return true;
}

/**
 * @brief Constructs an index object rooted at a block directory.
 * @param FilePath Block directory path.
 * @details If the directory exists, attempts immediate in-memory build.
 */
PrimaryKeyIndex::PrimaryKeyIndex(const string& FilePath)
{
    if (filesystem::exists(FilePath) && filesystem::is_directory(FilePath))
    {
        BuildFromBlocks(FilePath);
    }
}

/**
 * @brief Searches requested ZIP keys using the simple block index.
 * @param Zips CLI-style ZIP arguments (supports -Z<zip> or raw zip values).
 * @return First found ZipCodeRecord; default/empty record when none found.
 * @details Uses index to select candidate block, then scans that block only.
 */
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

    if (Index.empty())
    {
        if (filesystem::exists(BlockDirectory) && filesystem::is_directory(BlockDirectory))
        {
            if (!BuildFromBlocks(BlockDirectory))
            {
                cout << "Primary key index could not be built from blocks in: " << BlockDirectory << "\n";
                return found;
            }
        }
        else if (!ReadFromFile(BlockDirectory))
        {
            cout << "Primary key index not found for file: " << BlockDirectory << "\n";
            return found;
        }
    }

    for (const string& Zip : zipCodes)
    {
        auto it = Index.lower_bound(Zip);
        if (it == Index.end())
        {
            cout << "Zip Code " << Zip << " not found.\n";
            continue;
        }

        int rbn = it->second;
        string blockPath = GetBlockFilePath(rbn);
        ifstream InFile(blockPath);
        if (!InFile.is_open())
        {
            cout << "Could not open block file: " << blockPath << "\n";
            continue;
        }

        string headerLine;
        if (!getline(InFile, headerLine))
        {
            cout << "Failed to read block header for zip " << Zip << "\n";
            continue;
        }

        bool foundInBlock = false;
        string recordLine;
        while (getline(InFile, recordLine))
        {
            string zipStr = ExtractZipKey(recordLine);
            if (zipStr != Zip)
            {
                continue;
            }

            stringstream ss(recordLine);
            string lengthStr, city, state, county, latStr, longStr;
            getline(ss, lengthStr, ',');
            getline(ss, zipStr, ',');
            getline(ss, state, ',');
            getline(ss, latStr, ',');
            getline(ss, longStr, ',');

            double latitude = 0.0;
            double longitude = 0.0;
            try { latitude = stod(latStr); } catch (...) {}
            try { longitude = stod(longStr); } catch (...) {}

            string zipCopy = zipStr;
            string stateCopy = state;
            ZipCodeRecord record(zipCopy, stateCopy, latitude, longitude);

            cout << record.getZip() << ", " << record.getState() << ", "
                 << record.getLatitude() << ", " << record.getLongitude() << "\n";

            if (!foundAny)
            {
                found = record;
                foundAny = true;
            }
            foundInBlock = true;
            break;
        }

        if (!foundInBlock)
        {
            cout << "Zip Code " << Zip << " not found.\n";
        }
    }

    return found;
}

/**
 * @brief Produces a readable dump of simple index entries.
 * @return Newline-separated CSV lines in format <highestZipKey,RBN>.
 */
string PrimaryKeyIndex::Dump() const
{
    ostringstream out;
    for (const auto& pair : Index)
    {
        out << pair.first << "," << pair.second << "\n";
    }
    return out.str();
}

#endif