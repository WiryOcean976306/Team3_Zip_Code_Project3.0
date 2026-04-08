/**
 * @file TestDriver.cpp
 * @brief Minimal scenario runner for Project 3.0.
 * @author Robert Shores
 * This executable is intentionally lightweight and delegates core behavior
 * to the canonical production CLI in bin/main.
 */

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace {

/**
 * @brief Runtime switches for the scenario runner.
 */
struct ScenarioOptions {
    bool withMutations = false;
    bool printRecords = false;
    bool dryRun = false;
    bool showHelp = false;
};

/**
 * @brief Prints scenario-runner usage information.
 * @param program Executable name from argv[0].
 */
void PrintUsage(const char* program)
{
    cout << "Usage:\n"
         << "  " << program << " [--mutations] [--print] [--dry-run]\n\n"
         << "Description:\n"
         << "  Runs a predefined Project 3.0 scenario by invoking bin/main.\n\n"
         << "Options:\n"
         << "  --mutations   Include add/delete files in the scenario run.\n"
         << "  --print       Ask main to print sample records.\n"
         << "  --dry-run     Print the command without running it.\n"
         << "  -h, --help    Show this help message.\n\n"
         << "Examples:\n"
         << "  " << program << "\n"
         << "  " << program << " --mutations\n"
         << "  " << program << " --mutations --print\n";
}

/**
 * @brief Parses scenario-runner flags.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @param opts Output options structure.
 * @return True when parsing succeeds; otherwise false.
 */
bool ParseArgs(int argc, char* argv[], ScenarioOptions& opts)
{
    for (int i = 1; i < argc; ++i)
    {
        string arg = argv[i];
        if (arg == "-h" || arg == "--help")
        {
            opts.showHelp = true;
            return true;
        }
        if (arg == "--mutations")
        {
            opts.withMutations = true;
            continue;
        }
        if (arg == "--print")
        {
            opts.printRecords = true;
            continue;
        }
        if (arg == "--dry-run")
        {
            opts.dryRun = true;
            continue;
        }

        cerr << "Unknown option: " << arg << "\n";
        return false;
    }

    return true;
}

/**
 * @brief Resolves the canonical main binary path for the current platform.
 * @return Relative path to main executable, or empty string when unavailable.
 */
string ResolveMainBinary()
{
    if (filesystem::exists("bin/main.exe"))
    {
        return "bin/main.exe";
    }
    if (filesystem::exists("bin/main"))
    {
        return "bin/main";
    }
    return "";
}

/**
 * @brief Builds the delegated main-command string for the selected scenario.
 * @param mainBinary Path to main executable.
 * @param opts Scenario flags.
 * @return Full shell command text.
 */
string BuildScenarioCommand(const string& mainBinary, const ScenarioOptions& opts)
{
    ostringstream cmd;
    cmd << '"' << mainBinary << '"'
        << " -in data/Length_Indicated_us_postal_codes.csv"
        << " -out data/blocked_sequence_set.txt"
        << " -blockSize 512"
        << " -minCapacity 0.50"
        << " -startRBN 1"
        << " -dump data/dumped_records.txt"
        << " -physicalDump data/physical_block_dump.txt"
        << " -logicalDump data/logical_block_dump.txt"
        << " -index data/simple_index.txt"
        << " -indexDump data/simple_index_dump.txt"
        << " -printRecords " << (opts.printRecords ? 1 : 0)
        << " -maxPrint 25";

    if (opts.withMutations)
    {
        cmd << " -a data/recordsToAdd.txt"
            << " -d data/keysToDelete.txt"
            << " -addLog data/addition_log.txt"
            << " -deleteLog data/deletion_log.txt";
    }

    return cmd.str();
}

} // namespace

/**
 * @brief Main entry point for the lightweight scenario runner.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Process exit code.
 */
int main(int argc, char* argv[])
{
    ScenarioOptions options;
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

    const string mainBinary = ResolveMainBinary();
    if (mainBinary.empty())
    {
        cerr << "Could not find bin/main. Build it first with: make bin/main\n";
        return 1;
    }

    const string command = BuildScenarioCommand(mainBinary, options);
    cout << "Running scenario command:\n" << command << "\n\n";

    if (options.dryRun)
    {
        return 0;
    }

    const int rc = system(command.c_str());
    if (rc != 0)
    {
        cerr << "Scenario run failed with exit code: " << rc << "\n";
        return rc;
    }

    cout << "Scenario completed successfully.\n";
    return 0;
}
