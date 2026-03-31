/**
 * @file BlockedSequence.cpp
 * @author Zach Houck
 * @date 2026-03-26
 * @brief Implements BlockedSequence for a DLL-style block chain.
 */

#ifndef BLOCKEDSEQUENCE_CPP
#define BLOCKEDSEQUENCE_CPP

#include <map>
#include <string>

#include "BlockedSequence.h"
#include "Block.h"

using namespace std;

BlockedSequence::BlockedSequence(int startRBN)
{
    HeadRBN = 0;
    TailRBN = 0;
    NextRBN = startRBN;
}

bool BlockedSequence::IsEmpty()
{
    return HeadRBN == 0;
}

int BlockedSequence::GetHeadRBN()
{
    return HeadRBN;
}

int BlockedSequence::GetTailRBN()
{
    return TailRBN;
}

int BlockedSequence::GetNextRBN()
{
    return NextRBN;
}

int BlockedSequence::GetCount()
{
    return static_cast<int>(blocks.size());
}

Block* BlockedSequence::GetBlock(int rbn)
{
    auto it = blocks.find(rbn);
    if (it == blocks.end())
        return nullptr;
    return &(it->second);
}

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