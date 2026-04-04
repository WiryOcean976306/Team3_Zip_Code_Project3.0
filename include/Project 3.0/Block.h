/**
 * @file Block.h
 * @author Zach Houck
 * @date 2026-03-26
 * @brief Declares Block, a chunk of complete records that is a certain size.
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

        int RecordCount = 0;
        int RBN = 0;
        int PrevRBN = 0;
        int NextRBN = 0;
        int ByteSize = 0;
        int ByteMaxSize = 512; //512 is default 
        int HeaderSize = 0;

        string BlockHeader; //Formatted as: HeaderSize,RecordCount,RBN,PrevRBN,NextRBN 
        vector<string> Records;

        void UpdateHeader();

    public:

        /**
         * @brief Constructs a Block with the given previous RBN and current RBN, 
         * initializing all other members to default values.
         * @param prevrbn The RBN of the previous block.
         * @param rbn The RBN of this block.
        */
        Block(int prevrbn, int rbn);

        /**
         * @brief Attempts to add a record to the block, checking if it fits within the byte size limit.
         * @param Record The record string to add.
         * @return True if the record was added successfully, false if it would exceed the block.
         */
        bool AddRecord(const string& Record);
    
        bool ReadBlock();

        bool WriteBlock();

        /**
         * @brief Getters and setters for block properties.
         * @details Getters return the current value of the property. The setter for NextRBN also updates the block header to reflect the new value.
         */
        int GetRecordCount();
        int GetRBN();
        int GetPrevRBN();
        int GetNextRBN();
        int GetByteSize();
        vector<string>& GetRecords();
        void SetNextRBN(int nextRbn);

};

#endif