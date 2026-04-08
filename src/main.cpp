/**
 * @file main.cpp
 * @brief Production CLI entry point for Project 3.0 blocked-sequence generation.
 */

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Length_Indicated_ZipCodeBuffer.h"
#include "Project 3.0/BlockedSequence.h"
#include "Project 3.0/HeaderBuffer.h"
#include "HeaderRecord.h"
#include "PrimaryKeyIndex.h"

using namespace std;

namespace {

/**
 * @brief Runtime configuration parsed from command-line flags.
 */
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
    string addFile;
    string deleteFile;
    string addLogFile = "data/addition_log.txt";
    string deleteLogFile = "data/deletion_log.txt";
    bool showHelp = false;
};

/**
 * @brief Prints command-line help text for the main application.
 * @param program Executable name from argv[0].
 */
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
         << " [-maxPrint <count>]"
         << " [-a <records_to_add_file>]"
         << " [-d <keys_to_delete_file>]"
         << " [-addLog <addition_log_file>]"
         << " [-deleteLog <deletion_log_file>]\n\n"
         << "Example:\n"
         << "  " << program
         << " -in data/Length_Indicated_us_postal_codes.csv"
         << " -out data/blocked_sequence_set.txt"
         << " -blockSize 512 -minCapacity 0.50 -startRBN 1"
         << " -dump data/dumped_records.txt"
         << " -physicalDump data/physical_block_dump.txt"
         << " -logicalDump data/logical_block_dump.txt"
         << " -index data/simple_index.txt"
         << " -indexDump data/simple_index_dump.txt"
         << " -printRecords 0\n";
}

/**
 * @brief Parses and validates command-line options.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @param opts Output options structure.
 * @return True when parsing and validation succeed; otherwise false.
 */
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
            opts.showHelp = true;
            return true;
        }

        if (i + 1 >= argc)
        {
            cerr << "Missing value for option: " << arg << "\n";
            return false;
        }

        string value = argv[++i];
        try
        {
            if (arg == "-in") opts.inputFile = value;
            else if (arg == "-out") opts.blockedSetFile = value;
            else if (arg == "-blockSize") opts.blockSize = stoi(value);
            else if (arg == "-minCapacity") opts.minCapacity = stod(value);
            else if (arg == "-startRBN") opts.startRBN = stoi(value);
            else if (arg == "-dump") opts.dumpFile = value;
            else if (arg == "-physicalDump") opts.physicalDumpFile = value;
            else if (arg == "-logicalDump") opts.logicalDumpFile = value;
            else if (arg == "-index") opts.indexFile = value;
            else if (arg == "-indexDump") opts.indexDumpFile = value;
            else if (arg == "-printRecords") opts.printRecords = (stoi(value) != 0);
            else if (arg == "-maxPrint") opts.maxPrint = stoi(value);
            else if (arg == "-a") opts.addFile = value;
            else if (arg == "-d") opts.deleteFile = value;
            else if (arg == "-addLog") opts.addLogFile = value;
            else if (arg == "-deleteLog") opts.deleteLogFile = value;
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

/**
 * @brief Writes one text artifact file, creating parent directories as needed.
 * @param dumpFile Output path.
 * @param content Text content to write.
 * @return True when write succeeds; otherwise false.
 */
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

/**
 * @brief Counts active records by traversing the logical block chain.
 * @param sequence Sequence whose active records are counted.
 * @return Total number of active records.
 */
int CountActiveRecords(BlockedSequence& sequence)
{
    int total = 0;
    int currentRBN = sequence.GetHeadRBN();
    while (currentRBN != 0)
    {
        Block* blk = sequence.GetBlock(currentRBN);
        if (!blk)
        {
            break;
        }

        total += blk->GetRecordCount();
        currentRBN = blk->GetNextRBN();
    }

    return total;
}

/**
 * @brief Writes blocked-sequence header metadata to the configured data file.
 * @param opts Runtime options.
 * @param recordCount Number of active records.
 * @param sequence Sequence containing current block state.
 * @return True when header write succeeds; otherwise false.
 */
bool WriteHeader(const Options& opts, int recordCount, BlockedSequence& sequence)
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
    return headerBuffer.writeHeader(out, header) && out.good();
}

/**
 * @brief Rebuilds and writes simple index artifacts from current block files.
 * @param opts Runtime options containing index output paths.
 * @return True when index file and index dump are written successfully.
 */
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

/**
 * @brief Refreshes all generated outputs after sequence changes.
 * @param opts Runtime options for artifact paths.
 * @param sequence Updated sequence state.
 * @return True when header, dumps, and index artifacts are all refreshed.
 */
bool RefreshArtifacts(const Options& opts, BlockedSequence& sequence)
{
    const int recordCount = CountActiveRecords(sequence);

    if (!WriteHeader(opts, recordCount, sequence)) return false;
    if (!WriteDumpFile(opts.dumpFile, sequence.DumpLogicalOrder())) return false;
    if (!WriteDumpFile(opts.physicalDumpFile, sequence.DumpPhysicalBlockOrder())) return false;
    if (!WriteDumpFile(opts.logicalDumpFile, sequence.DumpLogicalBlockOrder())) return false;
    if (!WriteIndexArtifacts(opts)) return false;

    return true;
}

/**
 * @brief Builds a blocked sequence set from the configured length-indicated input.
 * @param opts Runtime options.
 * @param sequence Sequence object to populate.
 * @return Process exit code semantics: 0 on success, non-zero on failure.
 */
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

    if (!RefreshArtifacts(opts, sequence))
    {
        cerr << "Failed writing blocked-sequence artifacts\n";
        return 1;
    }

    cout << "Blocked sequence set generated successfully.\n";
    cout << "Records processed: " << recordCount << "\n";
    cout << "Blocks written: " << sequence.GetCount() << "\n";

    if (opts.printRecords)
    {
        string allRecords = sequence.DumpLogicalOrder();
        istringstream in(allRecords);
        string line;
        int printed = 0;
        cout << "\nSample records (logical order):\n";
        while (printed < opts.maxPrint && getline(in, line))
        {
            cout << line << "\n";
            ++printed;
        }
        if (printed == 0)
        {
            cout << "(no records)\n";
        }
    }

    return 0;
}

/**
 * @brief Applies additions from a file and logs each event.
 * @param addFile Path to packed records to insert.
 * @param sequence Sequence to mutate.
 * @param logFile Path for append-mode operation log output.
 * @return Number of successful insertions.
 */
int ProcessAdditions(const string& addFile, BlockedSequence& sequence, const string& logFile)
{
    if (addFile.empty()) return 0;

    ifstream inFile(addFile);
    ofstream log(logFile, ios::app);
    if (!inFile.is_open() || !log.is_open())
    {
        cerr << "Error opening add/log file(s).\n";
        return 0;
    }

    log << "\n=== Record Addition Session ===\n";
    int added = 0;
    string line;
    while (getline(inFile, line))
    {
        if (line.empty()) continue;
        string eventLog;
        if (sequence.Insert(line, eventLog))
        {
            ++added;
            log << eventLog;
            cout << eventLog;
        }
        else
        {
            log << "[FAIL] Could not insert record.\n";
            cout << "[FAIL] Could not insert record.\n";
        }
    }
    log << "[SUMMARY] Added " << added << " records.\n";
    return added;
}

/**
 * @brief Applies deletions from a file and logs each event.
 * @param deleteFile Path to ZIP keys to delete.
 * @param sequence Sequence to mutate.
 * @param logFile Path for append-mode operation log output.
 * @return Number of successful deletions.
 */
int ProcessDeletions(const string& deleteFile, BlockedSequence& sequence, const string& logFile)
{
    if (deleteFile.empty()) return 0;

    ifstream inFile(deleteFile);
    ofstream log(logFile, ios::app);
    if (!inFile.is_open() || !log.is_open())
    {
        cerr << "Error opening delete/log file(s).\n";
        return 0;
    }

    log << "\n=== Record Deletion Session ===\n";
    int deleted = 0;
    string line;
    while (getline(inFile, line))
    {
        if (line.empty()) continue;
        string eventLog;
        if (sequence.Delete(line, eventLog))
        {
            ++deleted;
            log << eventLog;
            cout << eventLog;
        }
        else
        {
            log << "[FAIL] Could not delete ZIP: " << line << "\n";
            cout << "[FAIL] Could not delete ZIP: " << line << "\n";
        }
    }
    log << "[SUMMARY] Deleted " << deleted << " records.\n";
    return deleted;
}

} // namespace

/**
 * @brief Main entry point for blocked-sequence generation and optional mutations.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Process exit code.
 */
int main(int argc, char* argv[])
{
    Options options;
    if (!ParseArgs(argc, argv, options))
    {
        PrintUsage(argv[0]);
        return 1;
    }

    if (options.showHelp)
    {
        PrintUsage(argv[0]);
        return 0;
    }

    try
    {
        BlockedSequence sequence(options.startRBN);
        int result = GenerateBlockedSet(options, sequence);
        if (result != 0)
        {
            return result;
        }

        if (!options.addFile.empty() || !options.deleteFile.empty())
        {
            if (!options.addFile.empty())
            {
                ProcessAdditions(options.addFile, sequence, options.addLogFile);
            }
            if (!options.deleteFile.empty())
            {
                ProcessDeletions(options.deleteFile, sequence, options.deleteLogFile);
            }

            if (!sequence.WriteAll())
            {
                cerr << "Error: Failed to write modified blocks back to disk.\n";
                return 1;
            }

            if (!RefreshArtifacts(options, sequence))
            {
                cerr << "Error: Failed to refresh output artifacts.\n";
                return 1;
            }

            cout << "Modified blocks written and artifacts refreshed.\n";
        }

        return 0;
    }
    catch (const exception& ex)
    {
        cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
}

        