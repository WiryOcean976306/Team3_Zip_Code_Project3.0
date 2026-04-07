/**
 * @file Block.h
 * @author Zach Houck
 * @date 2026-03-26
 * @brief Declares the Block container used to store packed records within a
 * fixed byte-capacity unit.
 *
 * Each block tracks metadata (record count, RBN links, byte usage) and stores
 * length-indicated record payloads.
 */

#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Block
{
    private:

        /** Number of records currently stored in this block. */
        int RecordCount = 0;
        /** Relative block number (RBN) identifying this block. */
        int RBN = 0;
        /** RBN of the previous block in the active chain. */
        int PrevRBN = 0;
        /** RBN of the next block in the active chain. */
        int NextRBN = 0;
        /** Total bytes currently used by record payloads. */
        int ByteSize = 0;
        /** Maximum payload capacity in bytes for this block. */
        int ByteMaxSize = 512; //512 is default
        /** Minimum payload capacity in bytes for this block. */
        int ByteMinSize = 256; //256 is default
        /** Byte length of the serialized header string. */
        int HeaderSize = 0;

        /** Serialized metadata header: HeaderSize,RecordCount,RBN,PrevRBN,NextRBN. */
        string BlockHeader;
        /** Record payloads stored in insertion order. */
        vector<string> Records;

        /** Rebuilds the serialized header after metadata changes. */
        void UpdateHeader();

    public:

        /**
         * @brief Constructs an empty block and initializes link metadata.
         * @param prevrbn RBN of the previous block in the sequence.
         * @param rbn RBN assigned to this block.
        */
        Block(int prevrbn, int rbn);

        /**
         * @brief Attempts to append one packed record into this block.
         * @param Record Length-indicated record text to add.
         * @return True if the record is valid and fits in the remaining capacity;
         *         otherwise false.
         */
        bool AddRecord(const string& Record);

        /**
         * @brief Reads this block's serialized representation from storage.
         * @return True on successful read and parse; otherwise false.
         */
        bool ReadBlock();

        /**
         * @brief Writes this block's serialized representation to storage.
         * @return True on successful write; otherwise false.
         */
        bool WriteBlock();

        /**
         * @brief Returns the number of records stored in this block.
         * @return Current record count.
         */
        int GetRecordCount();

        /**
         * @brief Returns this block's RBN.
         * @return Current block RBN.
         */
        int GetRBN();

        /**
         * @brief Returns the previous block's RBN.
         * @return Previous block RBN.
         */
        int GetPrevRBN();

        /**
         * @brief Returns the next block's RBN.
         * @return Next block RBN.
         */
        int GetNextRBN();

        /**
         * @brief Returns total payload bytes currently consumed in the block.
         * @return Number of used payload bytes.
         */
        int GetByteSize();

        /**
         * @brief Returns the maximum payload capacity in bytes for this block.
         * @return Maximum byte capacity.
         */
        int GetByteMaxSize();

        /**
         * @brief Returns the minimum payload capacity in bytes for this block.
         * @return Minimum byte capacity.
         */
        int GetByteMinSize();

        /**
         * @brief Provides mutable access to stored record payloads.
         * @return Reference to the internal record vector.
         */
        vector<string>& GetRecords();

        /**
         * @brief Updates the next-block link and refreshes serialized header data.
         * @param nextRbn RBN value to assign as the next block link.
         */
        void SetNextRBN(int nextRbn);

        /**
         * @brief Updates the previous-block link and refreshes serialized header data.
         * @param prevRbn RBN value to assign as the previous block link.
         */
        void SetPrevRBN(int prevRbn);

};

#endif