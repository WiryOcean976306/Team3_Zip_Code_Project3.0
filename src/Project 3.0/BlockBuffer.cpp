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
 #include <string.h>





BlockBuffer :: BlockBuffer (int maxBytes)
// construct witha  maximum of maxFields
{
    Init (maxBytes);
}






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




int BlockBuffer:: Init (int maxBytes)
// construct with a maximum of maxFields

{
    if (maxBytes < 0) maxBytes = 0;
    MaxBytes = maxBytes;
    Buffer = new char[MaxBytes];
    Clear ();
    return 1;
    

}









