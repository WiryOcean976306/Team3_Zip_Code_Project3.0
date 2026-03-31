/**
 * @file BlockedSequence.h
 * @author Zach Houck
 * @date 2026-03-26
 * @brief Declares BlockedSequence as a DLL of Block by RBN values.
 */

#ifndef BLOCKEDSEQUENCE_H
#define BLOCKEDSEQUENCE_H

#include <map>
#include <string>

#include "Block.h"

using namespace std;

class BlockedSequence
{
    private:
        map<int, Block> blocks;
        int HeadRBN = 0;
        int TailRBN = 0;
        int NextRBN = 1;

    public:
        
        /**
         * @brief Constructs an empty BlockedSequence with the given starting RBN for new blocks.
         * @param startRBN The RBN to assign to the first block added to the
         */
        BlockedSequence(int startRBN = 1);

        /**
         * @brief Checks if the BlockedSequence is empty by verifying if the HeadRBN is 0.
         * @return True if the sequence is empty, false otherwise.
         */
        bool IsEmpty();

        /**
         * @brief Getters for the head RBN, tail RBN, next RBN, and count of blocks in the sequence.
         */
        int GetHeadRBN();
        int GetTailRBN();
        int GetNextRBN();
        int GetCount();

        /**
         * @brief Appends a record to the end of the BlockedSequence, creating new blocks as necessary when the current tail block is full.
         * @param recordCsv The record string to append, formatted as a CSV.
         * @return True if the record was appended successfully, false if there was an error (
         */
        bool AppendRecord(const string& recordCsv);

        /**
         * @brief Writes all blocks in the sequence to disk, starting from the head block and following the next RBN links until the end of the sequence.
         * @return True if all blocks were written successfully, false if there was an error writing
         */
        bool WriteAll();

        /**
         * @brief Loads all blocks in the sequence from disk, starting from the head block and following the next RBN links until the end of the sequence.
         * @return True if all blocks were loaded successfully, false if there was an error loading
         */
        bool LoadAllFromHead();

        /**
         * @brief Retrieves a pointer to the Block with the specified RBN from the sequence.
         * @param rbn The RBN of the block to retrieve.
         * @return A pointer to the Block if found, or nullptr if no block with the specified RBN exists in the sequence.
         */
        Block* GetBlock(int rbn);
};

#endif