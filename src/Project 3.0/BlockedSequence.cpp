/**
 * @file BlockedSequence.cpp
 * @author Zach Houck
 * @date 2026-03-26
 * @brief Implements BlockedSequence operations for a doubly-linked block chain.
 *
 * This module manages in-memory block allocation, append behavior, and
 * head-to-tail traversal for read/write operations.
 */

#ifndef BLOCKEDSEQUENCE_CPP
#define BLOCKEDSEQUENCE_CPP

#include <map>
#include <string>
#include <sstream>
#include <vector>

#include "BlockedSequence.h"
#include "Block.h"

using namespace std;

namespace {

string ExtractZipKey(const string& packedRecord)
{
    size_t firstComma = packedRecord.find(',');
    if (firstComma == string::npos)
    {
        return "?";
    }

    size_t secondComma = packedRecord.find(',', firstComma + 1);
    if (secondComma == string::npos)
    {
        return packedRecord.substr(firstComma + 1);
    }

    return packedRecord.substr(firstComma + 1, secondComma - firstComma - 1);
}

string FormatBlockLine(Block& blk)
{
    ostringstream line;
    line << blk.GetRBN() << " [prev=" << blk.GetPrevRBN()
         << ", next=" << blk.GetNextRBN() << "] ";

    vector<string>& records = blk.GetRecords();
    if (records.empty())
    {
        line << "*available*";
        return line.str();
    }

    for (size_t i = 0; i < records.size(); ++i)
    {
        if (i > 0)
        {
            line << " ";
        }
        line << ExtractZipKey(records[i]);
    }

    return line.str();
}

string BuildDumpHeader(int headRBN)
{
    ostringstream out;
    out << "List Head: " << headRBN << "\n";
    out << "Avail Head: 0\n";
    return out.str();
}

} // namespace

/**
 * @brief Constructs an empty blocked sequence.
 * @param startRBN Initial RBN value assigned to the first new block.
 */
BlockedSequence::BlockedSequence(int startRBN)
{
    HeadRBN = 0;
    TailRBN = 0;
    NextRBN = startRBN;
}

/**
 * @brief Checks whether the sequence currently has no blocks.
 * @return True when head RBN is 0; otherwise false.
 */
bool BlockedSequence::IsEmpty()
{
    return HeadRBN == 0;
}

/**
 * @brief Returns the head block RBN.
 * @return RBN of the first block, or 0 if empty.
 */
int BlockedSequence::GetHeadRBN()
{
    return HeadRBN;
}

/**
 * @brief Returns the tail block RBN.
 * @return RBN of the last block, or 0 if empty.
 */
int BlockedSequence::GetTailRBN()
{
    return TailRBN;
}

/**
 * @brief Returns the next available RBN for block creation.
 * @return Next RBN counter value.
 */
int BlockedSequence::GetNextRBN()
{
    return NextRBN;
}

/**
 * @brief Returns the number of blocks tracked by this sequence.
 * @return Count of entries in the internal block map.
 */
int BlockedSequence::GetCount()
{
    return static_cast<int>(blocks.size());
}

/**
 * @brief Finds a block by its relative block number.
 * @param rbn Block identifier to look up.
 * @return Pointer to the located block, or nullptr if the RBN is not present.
 */
Block* BlockedSequence::GetBlock(int rbn)
{
    auto it = blocks.find(rbn);
    if (it == blocks.end())
        return nullptr;
    return &(it->second);
}

/**
 * @brief Appends one packed record to the tail of the blocked sequence.
 * @param recordCsv Length-indicated record payload to append.
 * @return True if appended successfully; otherwise false.
 * @details Creates the first block on demand. If the tail block is full,
 *          allocates and links a new tail block before insertion.
 */
bool BlockedSequence::AppendRecord(const string& recordCsv)
{
    if (IsEmpty())
    {
        int newRBN;
        if (!availList.empty())
        {
            newRBN = availList.front();
            availList.pop();
        } 
        else 
        {
            newRBN = NextRBN++;
        }
        Block newBlock(0, newRBN);
        if (!newBlock.AddRecord(recordCsv))
            return false;

        HeadRBN = TailRBN = newRBN;
        blocks.emplace(newRBN, std::move(newBlock));
        return true;
    }

    Block* tailBlock = GetBlock(TailRBN);
    if (!tailBlock)
        return false;

    if (tailBlock->AddRecord(recordCsv))
    {
        return true;
    }

    int newRBN;
    if (!availList.empty())
    {
        newRBN = availList.front();
        availList.pop();
    } 
    else 
    {
        newRBN = NextRBN++;
    }
    Block newBlock(TailRBN, newRBN);
    if (!newBlock.AddRecord(recordCsv))
        return false;

    tailBlock->SetNextRBN(newRBN);
    blocks.emplace(newRBN, std::move(newBlock));
    TailRBN = newRBN;

    return true;

    
}

/**
 * @brief Writes all blocks from head to tail using next-RBN links.
 * @return True if every block write succeeds; otherwise false.
 */
bool BlockedSequence::WriteAll()
{
    if (IsEmpty())
        return true;

    int currentRBN = HeadRBN;
    while (currentRBN != 0)
    {
        Block* blk = GetBlock(currentRBN);
        if (!blk)
            return false;

        if (!blk->WriteBlock())
            return false;

        currentRBN = blk->GetNextRBN();
    }

    return true;
}

/**
 * @brief Reads all blocks from head to tail using next-RBN links.
 * @return True if every block read succeeds; otherwise false.
 */
bool BlockedSequence::LoadAllFromHead()
{
    if (IsEmpty())
        return false;

    int currentRBN = HeadRBN;
    while (currentRBN != 0)
    {
        Block* blk = GetBlock(currentRBN);
        if (!blk)
            return false;

        if (!blk->ReadBlock())
            return false;

        currentRBN = blk->GetNextRBN();
    }

    return true;
}


string BlockedSequence::DumpLogicalOrder()
{
    string result;
    if  (IsEmpty())
        return result;

    int currentRBN = HeadRBN;
    while (currentRBN != 0)
    {
        Block* blk = GetBlock(currentRBN);
        if (!blk)
            break;

        for (const string& record : blk->GetRecords())
        {
            result += record + "\n";
        }

        currentRBN = blk->GetNextRBN();
    }
    return result;

}

string BlockedSequence::DumpPhysicalBlockOrder()
{
    string result = BuildDumpHeader(HeadRBN);
    for (auto& kv : blocks)
    {
        result += FormatBlockLine(kv.second) + "\n";
    }
    return result;
}

string BlockedSequence::DumpLogicalBlockOrder()
{
    string result = BuildDumpHeader(HeadRBN);

    int currentRBN = HeadRBN;
    while (currentRBN != 0)
    {
        Block* blk = GetBlock(currentRBN);
        if (!blk)
        {
            break;
        }

        result += FormatBlockLine(*blk) + "\n";
        currentRBN = blk->GetNextRBN();
    }

    return result;
}

/**
 * @brief Inserts a record into the appropriate block, handling block splits if needed.
 * @param recordCsv Packed record to insert.
 * @param logOutput Reference to string where split/insert events are logged.
 * @return True if insert succeeds; otherwise false.
 */
bool BlockedSequence::Insert(const string& recordCsv, string& logOutput)
{
    ostringstream log;

    if (IsEmpty())
    {
        log << "[INSERT] Sequence empty. Creating first block.\n";
        int newRBN = NextRBN++;
        Block newBlock(0, newRBN);
        if (!newBlock.AddRecord(recordCsv))
        {
            log << "[ERROR] Failed to add record to new block.\n";
            logOutput = log.str();
            return false;
        }
        HeadRBN = TailRBN = newRBN;
        blocks.emplace(newRBN, std::move(newBlock));
        log << "[SUCCESS] Record inserted into block " << newRBN << ".\n";
        logOutput = log.str();
        return true;
    }

    // Try to insert into tail block first
    Block* tailBlock = GetBlock(TailRBN);
    if (!tailBlock)
    {
        log << "[ERROR] Tail block not found.\n";
        logOutput = log.str();
        return false;
    }

    if (tailBlock->AddRecord(recordCsv))
    {
        log << "[INSERT] Record inserted into existing block " << TailRBN << ".\n";
        logOutput = log.str();
        return true;
    }

    // Tail block is full; need to split or create new block
    log << "[SPLIT] Block " << TailRBN << " is full. Creating new block.\n";
    int newRBN = NextRBN++;
    Block newBlock(TailRBN, newRBN);
    
    if (!newBlock.AddRecord(recordCsv))
    {
        log << "[ERROR] Failed to add record to new block.\n";
        logOutput = log.str();
        return false;
    }

    tailBlock->SetNextRBN(newRBN);
    blocks.emplace(newRBN, std::move(newBlock));
    TailRBN = newRBN;

    log << "[SPLIT_COMPLETE] New block " << newRBN << " created and linked.\n";
    logOutput = log.str();
    return true;
}

/**
 * @brief Deletes a record by ZIP key, handling block merges/redistributions if needed.
 * @param zipKey ZIP code to delete.
 * @param logOutput Reference to string where delete/merge events are logged.
 * @return True if deletion succeeds; otherwise false.
 */
bool BlockedSequence::Delete(const string& zipKey, string& logOutput)
{
    ostringstream log;

    if (IsEmpty())
    {
        log << "[DELETE_FAIL] Sequence is empty.\n";
        logOutput = log.str();
        return false;
    }

    // Search all blocks for the record
    int currentRBN = HeadRBN;
    int foundInBlock = -1;
    size_t recordIndex = 0;

    while (currentRBN != 0)
    {
        Block* blk = GetBlock(currentRBN);
        if (!blk)
        {
            currentRBN = 0;
            break;
        }

        vector<string>& records = blk->GetRecords();
        for (size_t i = 0; i < records.size(); ++i)
        {
            if (ExtractZipKey(records[i]) == zipKey)
            {
                foundInBlock = currentRBN;
                recordIndex = i;
                break;
            }
        }

        if (foundInBlock != -1)
            break;

        currentRBN = blk->GetNextRBN();
    }

    if (foundInBlock == -1)
    {
        log << "[DELETE_FAIL] ZIP key " << zipKey << " not found.\n";
        logOutput = log.str();
        return false;
    }

    // Delete the record
    Block* targetBlock = GetBlock(foundInBlock);
    if (!targetBlock)
    {
        log << "[ERROR] Target block not accessible.\n";
        logOutput = log.str();
        return false;
    }

    if (!targetBlock->RemoveRecordAt(recordIndex))
    {
        log << "[ERROR] Failed to remove record accounting from block "
            << foundInBlock << ".\n";
        logOutput = log.str();
        return false;
    }

    log << "[DELETE_SUCCESS] Record with ZIP " << zipKey << " deleted from block " 
        << foundInBlock << ".\n";

    // Check if block is now underfull (optional merge logic)
    if (targetBlock->GetByteSize() < targetBlock->GetByteMinSize() && GetCount() > 1 && foundInBlock != HeadRBN)
    {
        
        //MERGE/REDISTRIBUTION LOGIC TO BE IMPLEMENTED
        
        //After the merge/redistribution, the empty block is added to the avail list.

        int emptyRBN = foundInBlock; //Change foundInBlock to the empty block if foundInBlock is not supposed to be the empty block.
        int prevRBN = targetBlock->GetPrevRBN();
        int nextRBN = targetBlock->GetNextRBN();

        if (prevRBN != 0)
        {
            Block* prevBlock = GetBlock(prevRBN);
            if (prevBlock)
            {
                prevBlock->SetNextRBN(nextRBN);
            }
        }
        else
        {
            HeadRBN = nextRBN;
        }

        if (nextRBN != 0)
        {
            Block* nextBlock = GetBlock(nextRBN);
            if (nextBlock)
            {
                nextBlock->SetPrevRBN(prevRBN);
            }
        }
        else
        {
            TailRBN = prevRBN;
        }

        blocks.erase(emptyRBN);
        availList.push(emptyRBN);

        log << "[MERGE_CANDIDATE] Block " << foundInBlock << " is now empty. "
            << "Merge/redistribution could be performed.\n";
    }

    logOutput = log.str();
    return true;
}

int BlockedSequence::GetAvailListSize()
{
    return static_cast<int>(availList.size());
}

#endif