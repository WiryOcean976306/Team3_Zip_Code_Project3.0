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
         * @brief In-memory simple index mapping highest-key values to block RBNs.
         */
        map<string, int> Index;

        /** Directory containing block_<rbn>.blk files used for index build/read. */
        string BlockDirectory = "data/blocks";
        /** Optional path to a serialized simple index file. */
        string IndexFilePath = "";

        /** Extracts the ZIP key from a packed record string. */
        static string ExtractZipKey(const string& packedRecord);

        /** Returns the block file path for a given RBN. */
        string GetBlockFilePath(int rbn) const;

    public:

        /**
         * @brief Creates a PrimaryKeyIndex object rooted at a block directory or index path.
         * @param FilePath Path to a block directory (preferred) or index file.
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
         * @brief Finds ZIP records for command-line style query arguments.
         * @param Zips ZIP query tokens (supports -Z<zip> form).
         * @return The first matching ZipCodeRecord, or an empty/default record when none are found.
         */
        ZipCodeRecord find(vector<string> Zips);

        /**
         * @brief Writes the in-memory simple index to a file.
         * @param path Output path.
         * @return true if the index was successfully written; otherwise false.
         */
        bool WriteToFile(const string& path);

        /**
         * @brief Reads a serialized simple index file into memory.
         * @param path Input path.
         * @return true if the index was successfully read; otherwise false.
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