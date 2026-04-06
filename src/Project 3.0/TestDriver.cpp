#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <stdexcept>

#include "Length_Indicated_ZipCodeBuffer.h"
#include "Project 3.0/BlockBuffer.h"
#include "Project 3.0/HeaderBuffer.h"
#include "HeaderRecord.h"
#include "PrimaryKeyIndex.h"
#include "BlockedSequence.h"

using namespace std;

namespace {

struct Options {
    string inputFile = "data/Length_Indicated_us_postal_codes.csv";
    string blockedSetFile = "data/blocked_sequence_set.txt";
    int blockSize = 512;
    double minCapacity = 0.50;
    int startRBN = 1;
    string dumpFile;
    string physicalDumpFile = "data/physical_block_dump.txt";
    string logicalDumpFile = "data/logical_block_dump.txt";
    string indexFile = "data/simple_index.txt";
    string indexDumpFile = "data/simple_index_dump.txt";
    bool printRecords = false;
    int maxPrint = 50;
    string addFile;                           /**< File containing records to add. */
    string deleteFile;                        /**< File containing keys to delete. */
    string addLogFile = "data/addition_log.txt";     /**< Output log for add operations. */
    string deleteLogFile = "data/deletion_log.txt";  /**< Output log for delete operations. */
};

void PrintUsage(const char* program)
{
    cout << "Usage:\n"
         << "  " << program
         << " -in <input_length_indicated_csv>"
         << " -out <blocked_sequence_set_file>"
         << " [-blockSize <bytes>]"
         << " [-minCapacity <0.0-1.0>]"
         << " [-startRBN <int>]"
         << " [-dump <output_dump_file>]"
         << " [-physicalDump <physical_dump_file>]"
         << " [-logicalDump <logical_dump_file>]"
         << " [-index <index_file>]"
         << " [-indexDump <index_dump_file>]"
         << " [-printRecords <0|1>]"
         << " [-maxPrint <count>]\n\n"
         << "Example:\n"
         << "  " << program
         << " -in data/Length_Indicated_us_postal_codes.csv"
         << " -out data/zip_blocked_set.txt"
         << " -blockSize 512 -minCapacity 0.50 -startRBN 1"
         << " -dump data/dumped_records.txt"
         << " -physicalDump data/physical_block_dump.txt"
         << " -logicalDump data/logical_block_dump.txt"
         << " -index data/simple_index.txt"
         << " -indexDump data/simple_index_dump.txt"
         << " -printRecords 1 -maxPrint 25\n";
}

bool ParseArgs(int argc, char* argv[], Options& opts)
{
    if (argc == 1)
    {
        return true;
    }

    for (int i = 1; i < argc; ++i)
    {
        string arg = argv[i];

        if (arg == "-h" || arg == "--help")
        {
            PrintUsage(argv[0]);
            return false;
        }

        if (i + 1 >= argc)
        {
            cerr << "Missing value for option: " << arg << "\n";
            return false;
        }

        string value = argv[++i];
        try {
            if (arg == "-in")
            {
                opts.inputFile = value;
            }
            else if (arg == "-out")
            {
                opts.blockedSetFile = value;
            }
            else if (arg == "-blockSize")
            {
                opts.blockSize = stoi(value);
            }
            else if (arg == "-minCapacity")
            {
                opts.minCapacity = stod(value);
            }
            else if (arg == "-startRBN")
            {
                opts.startRBN = stoi(value);
            }
            else if (arg == "-dump")
            {
                opts.dumpFile = value;
            }
            else if (arg == "-physicalDump")
            {
                opts.physicalDumpFile = value;
            }
            else if (arg == "-logicalDump")
            {
                opts.logicalDumpFile = value;
            }
            else if (arg == "-index")
            {
                opts.indexFile = value;
            }
            else if (arg == "-indexDump")
            {
                opts.indexDumpFile = value;
            }
            else if (arg == "-printRecords")
            {
                opts.printRecords = (stoi(value) != 0);
            }
            else if (arg == "-maxPrint")
            {
                opts.maxPrint = stoi(value);
            }
            else if (arg == "-a")
            {
                opts.addFile = value;
            }
            else if (arg == "-d")
            {
                opts.deleteFile = value;
            }
            else if (arg == "-addLog")
            {
                opts.addLogFile = value;
            }
            else if (arg == "-deleteLog")
            {
                opts.deleteLogFile = value;
            }
            else
            {
                cerr << "Unknown option: " << arg << "\n";
                return false;
            }
        }
        catch (const exception&)
        {
            cerr << "Invalid value for " << arg << ": " << value << "\n";
            return false;
        }
    }

    if (opts.blockSize <= 0)
    {
        cerr << "-blockSize must be > 0\n";
        return false;
    }
    if (opts.minCapacity <= 0.0 || opts.minCapacity > 1.0)
    {
        cerr << "-minCapacity must be in range (0.0, 1.0]\n";
        return false;
    }
    if (opts.startRBN <= 0)
    {
        cerr << "-startRBN must be > 0\n";
        return false;
    }
    if (opts.maxPrint <= 0)
    {
        cerr << "-maxPrint must be > 0\n";
        return false;
    }

    return true;
}

bool WriteGeneratorHeader(const Options& opts, int recordCount, BlockedSequence& sequence)
{
    filesystem::path outPath(opts.blockedSetFile);
    if (outPath.has_parent_path())
    {
        filesystem::create_directories(outPath.parent_path());
    }

    ofstream out(opts.blockedSetFile, ios::trunc);
    if (!out.is_open())
    {
        return false;
    }

    HeaderRecord header;
    header.ConfigureBlockedSequenceHeader(
        opts.indexFile,
        recordCount,
        sequence.GetCount(),
        opts.blockSize,
        static_cast<int>(opts.minCapacity * 100.0),
        0,
        sequence.GetHeadRBN(),
        false);

    HeaderBuffer headerBuffer;
    if (!headerBuffer.writeHeader(out, header))
    {
        return false;
    }

    return out.good();
}

bool WriteDumpFile(const string& dumpFile, const string& content)
{
    if (dumpFile.empty())
    {
        return true;
    }

    filesystem::path outPath(dumpFile);
    if (outPath.has_parent_path())
    {
        filesystem::create_directories(outPath.parent_path());
    }

    ofstream out(dumpFile, ios::trunc);
    if (!out.is_open())
    {
        return false;
    }
    out << content;
    return out.good();
}

bool WriteIndexArtifacts(const Options& opts)
{
    PrimaryKeyIndex index("data/blocks");
    if (!index.BuildFromBlocks("data/blocks"))
    {
        cerr << "Failed to build primary key index from data/blocks\n";
        return false;
    }

    if (!index.WriteToFile(opts.indexFile))
    {
        cerr << "Failed writing index file: " << opts.indexFile << "\n";
        return false;
    }

    if (!WriteDumpFile(opts.indexDumpFile, index.Dump()))
    {
        cerr << "Failed writing index dump file: " << opts.indexDumpFile << "\n";
        return false;
    }

    return true;
}

int GenerateBlockedSet(const Options& opts, BlockedSequence& sequence)
{
    Length_Indicated_ZipCodeBuffer input(opts.inputFile);
    ZipCodeRecord record;

    int recordCount = 0;
    while (input.readNext(record))
    {
        if (!sequence.AppendRecord(input.packRecord(record)))
        {
            cerr << "Failed appending record index " << recordCount << "\n";
            return 1;
        }
        ++recordCount;
    }

    if (!sequence.WriteAll())
    {
        cerr << "Failed writing blocks to data/blocks\n";
        return 1;
    }

    if (!WriteGeneratorHeader(opts, recordCount, sequence))
    {
        cerr << "Failed writing blocked sequence set file: " << opts.blockedSetFile << "\n";
        return 1;
    }

    if (!WriteDumpFile(opts.dumpFile, sequence.DumpLogicalOrder()))
    {
        cerr << "Failed writing dump file: " << opts.dumpFile << "\n";
        return 1;
    }

    if (!WriteDumpFile(opts.physicalDumpFile, sequence.DumpPhysicalBlockOrder()))
    {
        cerr << "Failed writing physical block dump file: " << opts.physicalDumpFile << "\n";
        return 1;
    }

    if (!WriteDumpFile(opts.logicalDumpFile, sequence.DumpLogicalBlockOrder()))
    {
        cerr << "Failed writing logical block dump file: " << opts.logicalDumpFile << "\n";
        return 1;
    }

    if (!WriteIndexArtifacts(opts))
    {
        return 1;
    }

    cout << "Blocked sequence set generated successfully.\n";
    cout << "Records processed: " << recordCount << "\n";
    cout << "Blocks written: " << sequence.GetCount() << "\n";
    cout << "Blocked sequence set file: " << opts.blockedSetFile << "\n";
    cout << "Physical block dump: " << opts.physicalDumpFile << "\n";
    cout << "Logical block dump: " << opts.logicalDumpFile << "\n";
    cout << "Index file: " << opts.indexFile << "\n";
    cout << "Index dump: " << opts.indexDumpFile << "\n";
    if (!opts.dumpFile.empty())
    {
        cout << "Dump file: " << opts.dumpFile << "\n";
    }

    if (opts.printRecords)
    {
        string allRecords = sequence.DumpLogicalOrder();
        istringstream input(allRecords);
        string line;
        int printed = 0;
        cout << "\nSample records (logical order):\n";
        while (printed < opts.maxPrint && getline(input, line))
        {
            cout << line << "\n";
            ++printed;
        }
        if (printed == 0)
        {
            cout << "(no records)\n";
        }
        else if (getline(input, line))
        {
            cout << "... output truncated after " << printed << " records (use -maxPrint to change).\n";
        }
    }

    return 0;
}

/**
 * @brief Processes record additions from an input file.
 * @param addFile Path to file containing packed records to add.
 * @param sequence Reference to blocked sequence to modify.
 * @param logFile Output log file path.
 * @param verbose If true, print to console.
 * @return Number of records successfully added.
 */
int ProcessAdditions(const string& addFile, BlockedSequence& sequence, 
                     const string& logFile, bool verbose = true)
{
    if (addFile.empty())
        return 0;

    ifstream inFile(addFile);
    if (!inFile.is_open())
    {
        cerr << "Error: Cannot open add file: " << addFile << "\n";
        return 0;
    }

    ofstream log(logFile, ios::app);
    if (!log.is_open())
    {
        cerr << "Error: Cannot open log file: " << logFile << "\n";
        return 0;
    }

    log << "\n=== Record Addition Session ===\n";
    if (verbose)
        cout << "\n=== Processing Additions ===\n";

    int added = 0;
    string line;
    while (getline(inFile, line))
    {
        if (line.empty())
            continue;

        string eventLog;
        if (sequence.Insert(line, eventLog))
        {
            added++;
            log << eventLog;
            if (verbose)
                cout << eventLog;
        }
        else
        {
            log << "[FAIL] Could not insert record.\n";
            if (verbose)
                cout << "[FAIL] Could not insert record.\n";
        }
    }

    log << "[SUMMARY] Added " << added << " records.\n";
    cout << "Records added: " << added << "\n";
    cout << "Log: " << logFile << "\n";

    inFile.close();
    log.close();
    return added;
}

/**
 * @brief Processes record deletions from an input file.
 * @param deleteFile Path to file containing ZIP keys to delete.
 * @param sequence Reference to blocked sequence to modify.
 * @param logFile Output log file path.
 * @param verbose If true, print to console.
 * @return Number of records successfully deleted.
 */
int ProcessDeletions(const string& deleteFile, BlockedSequence& sequence,
                     const string& logFile, bool verbose = true)
{
    if (deleteFile.empty())
        return 0;

    ifstream inFile(deleteFile);
    if (!inFile.is_open())
    {
        cerr << "Error: Cannot open delete file: " << deleteFile << "\n";
        return 0;
    }

    ofstream log(logFile, ios::app);
    if (!log.is_open())
    {
        cerr << "Error: Cannot open log file: " << logFile << "\n";
        return 0;
    }

    log << "\n=== Record Deletion Session ===\n";
    if (verbose)
        cout << "\n=== Processing Deletions ===\n";

    int deleted = 0;
    string line;
    while (getline(inFile, line))
    {
        if (line.empty())
            continue;

        string eventLog;
        if (sequence.Delete(line, eventLog))
        {
            deleted++;
            log << eventLog;
            if (verbose)
                cout << eventLog;
        }
        else
        {
            log << "[FAIL] Could not delete ZIP: " << line << "\n";
            if (verbose)
                cout << "[FAIL] Could not delete ZIP: " << line << "\n";
        }
    }

    log << "[SUMMARY] Deleted " << deleted << " records.\n";
    cout << "Records deleted: " << deleted << "\n";
    cout << "Log: " << logFile << "\n";

    inFile.close();
    log.close();
    return deleted;
}

} // namespace

void testBlockBuffer ()
{

    
    Length_Indicated_ZipCodeBuffer LIrecBuff1("data/Length_Indicated_us_postal_codes.csv");

}



void testBlockedSequence () 
{

    std::string filename;
    
    Length_Indicated_ZipCodeBuffer LIBuff2("data/Length_Indicated_us_postal_codes.csv");
    BlockedSequence sequence;
    ZipCodeRecord record;


    
    while(LIBuff2.readNext(record))
    {    
        sequence.AppendRecord(LIBuff2.packRecord(record));  
    }

    bool writeSuccess= sequence.WriteAll();
    if (writeSuccess) {
        cout << "\n Succesfully wrote " << sequence.GetCount() << " blocks to disk." << endl;
    } else {
        cout << "\n Failed to write blocks to disk." << endl; 
    }

   std::string dumpedRecords = sequence.DumpLogicalOrder();
   std::ofstream dumpOut("data/dumped_records.txt");
    if (!dumpOut.is_open()) {
        cout << "\n Failed to open dump output file." << endl;
    } else {
        dumpOut << dumpedRecords;
        if (dumpOut.good()) {
            cout << "\n Successfully dumped records to data/dumped_records.txt" << endl;
        } else {
            cout << "\n Failed to write dumped records to file." << endl;
        }
    }
       
}

int main(int argc, char* argv[]) {
    Options options;
    if (!ParseArgs(argc, argv, options))
    {
        PrintUsage(argv[0]);
        return 1;
    }

    try {
        BlockedSequence sequence(options.startRBN);

        // Generate initial blocked sequence set
        int result = GenerateBlockedSet(options, sequence);
        if (result != 0)
            return result;

        // If add or delete operations requested, perform them
        if (!options.addFile.empty() || !options.deleteFile.empty())
        {
            // Process additions if requested
            if (!options.addFile.empty())
            {
                ProcessAdditions(options.addFile, sequence, options.addLogFile, true);
            }

            // Process deletions if requested
            if (!options.deleteFile.empty())
            {
                ProcessDeletions(options.deleteFile, sequence, options.deleteLogFile, true);
            }

            // Write modified sequence back to disk
            if (!sequence.WriteAll())
            {
                cerr << "Error: Failed to write modified blocks back to disk.\n";
                return 1;
            }

            cout << "\nModified blocks written to disk.\n";
        }

        return 0;
    }
    catch (const exception& ex)
    {
        cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
}
