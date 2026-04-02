/**
 * @file BlockBuffer.cpp
 * @author Robert Shores
 * @date 2026-03-31
 * @brief Implementation file for the BlockBuffer class which unpacks a record from
 * a block into a record buffer. Reads/writes whole fixed-size blocks (default 512 bytes).
 *
 * Knows block meta data layout: record count, prev RBN, next RBN
 */



 #include "block.h"
 #include "BlockBuffer.h"
 


int BlockBuffer :: Unpack (char * str)
// extract the value of the next field of the buffer
{
	short len; // length of packed string
	if (NextByte >= BufferSize) return false; // no more fields
	int start = NextByte; // first character to be unpacked
	memcpy (&len, &Buffer[start], sizeof(short));
	NextByte += len + sizeof(short);
	if (NextByte > BufferSize) return false;
	strncpy (str, &Buffer[start+sizeof(short)], len);
	str [len] = 0; // zero termination for string 
	return true;
}














