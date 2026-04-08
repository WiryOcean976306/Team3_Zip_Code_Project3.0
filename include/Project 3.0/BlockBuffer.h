/**
 * @file BlockBuffer.h
 * @author Robert Shores
 * @date 2026-03-31
 * @brief Declares BlockBuffer utilities for packing/unpacking block and record data.
 */



#ifndef BLOCKBUFFER_H
#define BLOCKBUFFER_H

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <cstring>
#include "Block.h"
#include "ZipCodeRecord.h"

/**
 * @brief Buffer helper for reading/writing packed fields and unpacking records from blocks.
 */
class BlockBuffer {
    private:
        std::ifstream file; /**< Reserved input stream handle for buffer workflows. */
        std::string csvPath; /**< Optional source path used by callers. */
        bool BufferRead; /**< Tracks whether the internal buffer currently has content. */

    public:

        /**
         * @brief Constructs a block buffer with a maximum byte capacity.
         * @param maxBytes Maximum number of bytes that can be stored in the internal buffer.
         */
        BlockBuffer(int maxBytes = 512);

        /**
         * @brief Clears packed content and resets cursor positions.
         */
        void Clear();

        /**
         * @brief Reads packed bytes from a stream into the internal buffer.
         * @param stream Source stream.
         * @return Non-zero when read succeeds; otherwise zero.
         */
        int Read(istream& stream);

        /**
         * @brief Writes packed bytes from the internal buffer to a stream.
         * @param stream Destination stream.
         * @return Non-zero when write succeeds; otherwise zero.
         */
        int Write(ostream& stream) const;

        /**
         * @brief Packs one field string into the internal buffer.
         * @param str Input C-string field value.
         * @param size Optional explicit size, or -1 to use string length.
         * @return Non-zero when pack succeeds; otherwise zero.
         */
        int Pack(const char*, short size = -1);

        /**
         * @brief Unpacks one field string from the internal buffer.
         * @param str Destination C-string buffer.
         * @return Non-zero when unpack succeeds; otherwise zero.
         */
        int Unpack(char*);

        /**
         * @brief Prints a short diagnostic summary of the internal buffer.
         * @param stream Output stream.
         */
        void Print(ostream&) const;

        /**
         * @brief Initializes or reinitializes internal storage.
         * @param maxBytes Maximum allowed bytes for this buffer.
         * @return Non-zero when initialization succeeds.
         */
        int Init(int maxBytes = 512);

        /**
         * @brief Extracts one packed record string from a block.
         * @param block Source block containing packed records.
         * @param recordIndex Zero-based index of the record in the block.
         * @param recordBufferOut Output packed record string.
         * @return true if extraction succeeds; otherwise false.
         */
        bool UnpackRecordFromBlock(Block& block, int recordIndex, std::string& recordBufferOut) const;

        /**
         * @brief Parses one packed record buffer into a ZipCodeRecord object.
         * @param recordBuffer Packed record in format <len>,<zip>,<state>,<lat>,<long>.
         * @param recordOut Output record object.
         * @return true if parse succeeds; otherwise false.
         */
        bool UnpackFieldsToRecord(const std::string& recordBuffer, ZipCodeRecord& recordOut) const;

        /**
         * @brief Convenience method: unpack a block record directly into an object.
         * @param block Source block.
         * @param recordIndex Zero-based index into block records.
         * @param recordOut Output record object.
         * @return true if both unpack stages succeed; otherwise false.
         */
        bool UnpackRecordFromBlock(Block& block, int recordIndex, ZipCodeRecord& recordOut) const;

    private:

        char* Buffer; /**< Character array holding packed field bytes. */
        int BufferSize; /**< Number of valid bytes currently in Buffer. */
        int MaxBytes; /**< Maximum allowed number of bytes in Buffer. */
        int NextByte; /**< Cursor used by Pack/Unpack operations. */
};

#endif