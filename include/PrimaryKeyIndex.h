/**
 * @file PrimaryKeyIndex.h
 * @author Zach Houck
 * @date 2026-03-14
 * @brief Declares PrimaryKeyIndex, a primary key index that can be used to display the Zip Code data for all Zip Codes listed on the command line.
 */

#ifndef  PRIMARYKEYINDEX_H
#define  PRIMARYKEYINDEX_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include "ZipCodeRecord.h"

using namespace std;

class PrimaryKeyIndex
{
    private:

        /**
        * @brief A map that stores the PrimaryKeyIndex of a file.
        * @param string The ZipCode
        * @param int The byte position of the record
        */
        map<string, int> Index;

        string File = "";
        string header = "";

    public:

        /**
        * @brief Creates a Primary Key Index for a provided ZipCodeRecord.
        * @param file The file a Primary Key index will be made for.
        */
        PrimaryKeyIndex(const string& FilePath);

        /**
         * @brief Finds the ZipCodeRecord for a provided Zip Codes.
         * @param Zips A vector of Zip Codes to search for.
         * @return The ZipCodeRecord for the first matching Zip Code, or an invalid record if none found.
         */
        ZipCodeRecord find(vector<string> Zips);

        /**
         * @brief Writes the Primary Key Index to a file in the PKI directory.
         * @param path The path to the file to write the index to.
         * @return true if the index was successfully written, false otherwise.
         */
        bool WriteToFile(const string& path);

        /**
         * @brief Reads the Primary Key Index from a file in the PKI directory.
         * @param path The path to the file to read the index from.
         * @return true if the index was successfully read, false otherwise.
         */
        bool ReadFromFile(const string& path);
};

#endif