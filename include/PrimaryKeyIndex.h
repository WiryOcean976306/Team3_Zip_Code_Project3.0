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

        string BlockDirectory = "data/blocks";
        string IndexFilePath = "";

        /** Extracts the ZIP key from a packed record string. */
        static string ExtractZipKey(const string& packedRecord);

        /** Returns the block file path for a given RBN. */
        string GetBlockFilePath(int rbn) const;

    public:

        /**
        * @brief Creates a Primary Key Index builder rooted at a block directory.
        * @param FilePath Path to the block directory (default: data/blocks).
        */
        PrimaryKeyIndex(const string& FilePath);

        /**
         * @brief Sets the block directory used to resolve RBN -> block file path.
         * @param blockDirectory Directory containing block_*.blk files.
         */
        void SetBlockDirectory(const string& blockDirectory);

        /**
         * @brief Builds the in-memory index from the current block directory.
         * @return true if the index was built successfully; otherwise false.
         */
        bool BuildFromBlocks(const string& blockDirectory);

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

        /**
         * @brief Loads a simple index file into RAM and remembers its file path.
         * @param indexPath Path to simple index file.
         * @return true when index file was loaded successfully; otherwise false.
         */
        bool LoadFromIndexFile(const string& indexPath);

        /**
         * @brief Returns a readable dump of the current index contents.
         * @return Text listing highest-key / RBN pairs.
         */
        string Dump() const;
};

#endif