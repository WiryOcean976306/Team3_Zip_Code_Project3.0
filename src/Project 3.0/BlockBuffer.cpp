/**
 * @file BlockBuffer.cpp
 * @author Robert Shores
 * @date 2026-03-31
 * @brief Implementation file for the BlockBuffer class which unpacks a record from
 * a block into a record buffer. Reads/writes whole fixed-size blocks (default 512 bytes).
 *
 * Knows block metadatea layout: record count, prev RBN, next RBN
 */



 #include "block.h"
 


int BlockBuffer::Unpack ( void* field, int maxBytes) {
    if (NextRecord == RecordCount || Packing)
    // buffer is full or not in unpacking mode
    return -1;
    int start = NextByte; // first byte to be unpacked
    int packSize = FieldSize(NextField); // bytes to be unpacked
    memcpy (field, &buffer[start], packSize); // move the bytes
    NextByte += packSize; // advance NextByte to the following char
    NextField ++; // advance NextField
    if (NextRecord == RecordCount) clear(); // all fields unpacked
        return packSize;
}














