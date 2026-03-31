/**
 * @file Block.cpp
 * @author Zach Houck
 * @date 2026-03-26
 * @brief Implements Block, View Block.h for descriptions.
 */

#ifndef BLOCK_CPP
#define BLOCK_CPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Block.h"

using namespace std;

int Block::GetRecordCount()
{ 
    return RecordCount; 
}

int Block::GetRBN()
{ 
    return RBN; 
}

int Block::GetPrevRBN()
{ 
    return PrevRBN; 
}

int Block::GetNextRBN()
{ 
    return NextRBN; 
}

int Block::GetByteSize()
{ 
    return ByteSize; 
}

vector<string>& Block::GetRecords()
{ 
    return Records; 
}

void Block::SetNextRBN(int nextRbn)
{ 
    NextRBN = nextRbn; 
    UpdateHeader(); 
}

void Block::UpdateHeader()
{
    BlockHeader = to_string(HeaderSize) + "," + to_string(RecordCount) + "," + to_string(RBN) + "," + to_string(PrevRBN) + "," + to_string(NextRBN);
    HeaderSize = static_cast<int>(BlockHeader.size());
    BlockHeader = to_string(HeaderSize) + "," + to_string(RecordCount) + "," + to_string(RBN) + "," + to_string(PrevRBN) + "," + to_string(NextRBN);
}

Block::Block(int prevrbn, int rbn)
{
    RecordCount = 0;
    RBN = rbn;
    PrevRBN = prevrbn;
    NextRBN = 0;
    ByteSize = 0;
    HeaderSize = 0;
    BlockHeader = "";
    Records.clear();

    UpdateHeader();
}

bool Block::AddRecord(const string& Record)
{
    string csv = Record;
    size_t pos = csv.find(',');
    if (pos == string::npos)
    {
        return false;
    }

    int recordSize;
    recordSize = stoi(csv.substr(0, pos));

    if (recordSize < 0 || ByteSize + recordSize > ByteMaxSize)
    {
        return false;
    }

    Records.push_back(Record);
    RecordCount++;
    ByteSize += recordSize;
    UpdateHeader();
    return true;
}

#endif