/**
 * @file BlockBuffer.cpp
 * @author Robert Shores
 * @date 2026-03-31
 * @brief Implementation file for the BlockBuffer class which unpacks a record from
 * a block into a record buffer. Reads/writes whole fixed-size blocks (default 512 bytes).
 *
 * Knows block meta data layout: record count, prev RBN, next RBN
 */




 #include "BlockBuffer.h"
 #include <string.h>





BlockBuffer :: BlockBuffer (int maxBytes)
// construct with  maximum of maxFields
{
    Init (maxBytes);
}


void BlockBuffer :: clear ()
// clear fields from buffer
{
    NextByte = 0;
    BufferSize = 0;
}

int BlockBuffer :: Read (istream & stream)
{
    Clear();
    stream . read(reinterpret_cast<char*>(&BufferSize), sizeof(BufferSize));
    if (stream.fail()) return false;
    if (BufferSize > MaxBytes) return false; // buffer overflow
    stream . read (Buffer, BufferSize);
    return stream . good (); 
}


int BlockBuffer :: Write (ostream & stream) const
{
    stream . write (reinterpret_cast<const char*>(&BufferSize), sizeof(BufferSize));
    stream . write (Buffer, BufferSize)
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









