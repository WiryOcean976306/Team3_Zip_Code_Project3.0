/**
*@file SearchIndex.cpp
*@author Zach
*@date 2026-02-10
*@brief Search program for blocked-sequence data using a simple primary-key index.
 */

#include <iostream>
#include <string>
#include <vector>
#include "PrimaryKeyIndex.h"

using namespace std;

namespace {

void PrintUsage(const char* program)
{
    cout << "Usage:\n"
         << "  " << program
         << " -data <blocked_sequence_set_file>"
         << " -index <simple_index_file>"
         << " [-blocks <block_directory>]"
         << " -Z<zip> [-Z<zip> ...]\n\n"
         << "Example:\n"
         << "  " << program
         << " -data data/blocked_sequence_set.txt"
         << " -index data/simple_index.txt"
         << " -blocks data/blocks"
         << " -Z00501 -Z56301 -Z99999\n";
}

} // namespace


int main(int argc, char* argv[]) {

    if (argc <= 1)
    {
        PrintUsage(argv[0]);
        return 1;
    }

    string dataFile;
    string indexFile;
    string blockDir = "data/blocks";
    vector<string> args;

    for (int i = 1; i < argc; ++i)
    {
        string arg = argv[i];
        if (arg == "-h" || arg == "--help")
        {
            PrintUsage(argv[0]);
            return 0;
        }

        if (arg == "-data")
        {
            if (i + 1 >= argc)
            {
                cerr << "Missing value for -data\n";
                return 1;
            }
            dataFile = argv[++i];
            continue;
        }

        if (arg == "-index")
        {
            if (i + 1 >= argc)
            {
                cerr << "Missing value for -index\n";
                return 1;
            }
            indexFile = argv[++i];
            continue;
        }

        if (arg == "-blocks")
        {
            if (i + 1 >= argc)
            {
                cerr << "Missing value for -blocks\n";
                return 1;
            }
            blockDir = argv[++i];
            continue;
        }

        args.emplace_back(arg);
    }

    if (dataFile.empty())
    {
        cerr << "Missing required option: -data <blocked_sequence_set_file>\n";
        return 1;
    }

    if (indexFile.empty())
    {
        cerr << "Missing required option: -index <simple_index_file>\n";
        return 1;
    }

    if (args.empty())
    {
        cerr << "Provide one or more zip queries using -Z<zip>\n";
        return 1;
    }

    ifstream dataIn(dataFile);
    if (!dataIn.is_open())
    {
        cerr << "Could not open blocked sequence set data file: " << dataFile << "\n";
        return 1;
    }
    dataIn.close();

    PrimaryKeyIndex pki(indexFile);
    pki.SetBlockDirectory(blockDir);
    if (!pki.LoadFromIndexFile(indexFile))
    {
        cerr << "Could not load simple index file: " << indexFile << "\n";
        return 1;
    }

    pki.find(args);

    return 0;
}
