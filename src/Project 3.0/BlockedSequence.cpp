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

#include "BlockedSequence.h"
#include "Block.h"

using namespace std;

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
        int newRBN = NextRBN++;
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

    int newRBN = NextRBN++;
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

#endif