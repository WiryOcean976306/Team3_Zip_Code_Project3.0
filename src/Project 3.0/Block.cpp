/**
 * @file Block.cpp
 * @author Zach Houck
 * @date 2026-03-26
 * @brief Implements Block behavior for record storage, metadata updates, and
 * capacity checks.
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

/**
 * @brief Returns the number of records stored in this block.
 * @return Current record count.
 */
int Block::GetRecordCount()
{ 
    return RecordCount; 
}

/**
 * @brief Returns this block's relative block number.
 * @return Block RBN.
 */
int Block::GetRBN()
{ 
    return RBN; 
}

/**
 * @brief Returns the previous block link.
 * @return Previous block RBN.
 */
int Block::GetPrevRBN()
{ 
    return PrevRBN; 
}

/**
 * @brief Returns the next block link.
 * @return Next block RBN.
 */
int Block::GetNextRBN()
{ 
    return NextRBN; 
}

/**
 * @brief Returns the payload bytes currently used by records.
 * @return Current byte usage.
 */
int Block::GetByteSize()
{ 
    return ByteSize; 
}

/**
 * @brief Returns mutable access to the block's stored records.
 * @return Reference to the record vector.
 */
vector<string>& Block::GetRecords()
{ 
    return Records; 
}

/**
 * @brief Updates the next block link and refreshes header metadata.
 * @param nextRbn New next block RBN value.
 */
void Block::SetNextRBN(int nextRbn)
{ 
    NextRBN = nextRbn; 
    UpdateHeader(); 
}

/**
 * @brief Recomputes the serialized header string.
 * @details Header is stored as comma-separated metadata fields in the form:
 *          HeaderSize,RecordCount,RBN,PrevRBN,NextRBN.
 */
void Block::UpdateHeader()
{
    BlockHeader = to_string(HeaderSize) + "," + to_string(RecordCount) + "," + to_string(RBN) + "," + to_string(PrevRBN) + "," + to_string(NextRBN);
    HeaderSize = static_cast<int>(BlockHeader.size());
    BlockHeader = to_string(HeaderSize) + "," + to_string(RecordCount) + "," + to_string(RBN) + "," + to_string(PrevRBN) + "," + to_string(NextRBN);
}

/**
 * @brief Constructs an empty block with initialized links and metadata.
 * @param prevrbn RBN of the previous block in the chain.
 * @param rbn RBN assigned to this block.
 */
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

/**
 * @brief Attempts to append one packed record to this block.
 * @param Record Length-indicated record string.
 * @return True if record format is valid and capacity permits insertion;
 *         otherwise false.
 * @details Expects a leading numeric size prefix before the first comma.
 *          Uses that size to enforce block byte-capacity limits.
 */
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