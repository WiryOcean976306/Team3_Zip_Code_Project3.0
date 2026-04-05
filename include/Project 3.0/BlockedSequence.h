/**
 * @file BlockedSequence.h
 * @author Zach Houck
 * @date 2026-03-26
 * @brief Declares the BlockedSequence container used to manage linked blocks by RBN.
 *
 * The sequence stores blocks in memory and maintains logical ordering through
 * each block's previous and next relative block number (RBN).
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
        /** Maps each block's RBN to the corresponding in-memory Block object. */
        map<int, Block> blocks;
        /** RBN of the first block in the logical sequence (0 when empty). */
        int HeadRBN = 0;
        /** RBN of the last block in the logical sequence (0 when empty). */
        int TailRBN = 0;
        /** Next RBN value to assign when a new block is created. */
        int NextRBN = 1;

    public:
        
        /**
         * @brief Constructs an empty blocked sequence.
         * @param startRBN Initial RBN value to assign to the first created block.
         * @details Initializes head and tail as empty (0) and stores the next
         *          RBN counter used for newly allocated blocks.
         */
        BlockedSequence(int startRBN = 1);

        /**
         * @brief Indicates whether the sequence has any blocks.
         * @return True when no blocks are currently linked; otherwise false.
         */
        bool IsEmpty();

        /**
         * @brief Returns the RBN of the head block.
         * @return Head block RBN, or 0 when the sequence is empty.
         */
        int GetHeadRBN();

        /**
         * @brief Returns the RBN of the tail block.
         * @return Tail block RBN, or 0 when the sequence is empty.
         */
        int GetTailRBN();

        /**
         * @brief Returns the next RBN that will be assigned to a new block.
         * @return Next available RBN value.
         */
        int GetNextRBN();

        /**
         * @brief Returns the number of blocks currently tracked in memory.
         * @return Count of blocks in the internal map.
         */
        int GetCount();

        /**
         * @brief Appends one packed record to the logical tail of the sequence.
         * @param recordCsv Length-indicated record payload to append.
         * @return True if the record is inserted into an existing or new block;
         *         otherwise false.
         * @details If the current tail block has space, the record is inserted there.
         *          If not, a new block is created, linked after the tail, and the
         *          record is inserted into the new block.
         */
        bool AppendRecord(const string& recordCsv);

        /**
         * @brief Writes every linked block from head to tail.
         * @return True if all blocks are written successfully; otherwise false.
         * @details Traverses the active chain using each block's next-RBN pointer
         *          and calls Block::WriteBlock on each visited block.
         */
        bool WriteAll();

        /**
         * @brief Reads every linked block from head to tail.
         * @return True if all blocks are read successfully; otherwise false.
         * @details Traverses the active chain using each block's next-RBN pointer
         *          and calls Block::ReadBlock on each visited block.
         */
        bool LoadAllFromHead();

        /**
         * @brief Looks up a block by relative block number.
         * @param rbn RBN key of the block to locate.
         * @return Pointer to the matching block, or nullptr if not found.
         */
        Block* GetBlock(int rbn);
};

#endif