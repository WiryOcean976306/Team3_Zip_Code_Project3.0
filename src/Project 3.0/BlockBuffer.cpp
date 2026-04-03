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





/**
 * @brief Constructs a BlockBuffer with the specified maximum byte capacity.
 * @param maxBytes Maximum number of bytes the buffer can hold.
 * @details Allocates the internal byte array and initializes the buffer state.
 */
BlockBuffer :: BlockBuffer (int maxBytes)
// construct with  maximum of maxFields
{
    Init (maxBytes);
}


/**
 * @brief Clears the buffer state so it can be reused.
 * @details Resets the current cursor position and stored size to zero.
 */
void BlockBuffer :: clear ()
// clear fields from buffer
{
    NextByte = 0;
    BufferSize = 0;
}

/**
 * @brief Reads a packed buffer from an input stream.
 * @param stream Input stream containing length-prefixed packed data.
 * @return True if the buffer was read successfully; otherwise false.
 * @details Reads the size prefix first, validates the size, and then reads the
 *          packed bytes into the internal buffer.
 */
int BlockBuffer :: Read (istream & stream)
{
    Clear();
    stream . read(reinterpret_cast<char*>(&BufferSize), sizeof(BufferSize));
    if (stream.fail()) return false;
    if (BufferSize > MaxBytes) return false; // buffer overflow
    stream . read (Buffer, BufferSize);
    return stream . good (); 
}


/**
 * @brief Writes the current packed buffer to an output stream.
 * @param stream Output stream that receives the buffer contents.
 * @return True if the buffer was written successfully; otherwise false.
 * @details Writes the stored size first, followed by the raw packed bytes.
 */
int BlockBuffer :: Write (ostream & stream) const
{
    stream . write (reinterpret_cast<const char*>(&BufferSize), sizeof(BufferSize));
    stream . write (Buffer, BufferSize)
}


/**
 * @brief Packs a string into the buffer using a length prefix.
 * @param str String to store in the buffer.
 * @param size Number of bytes to pack, or -1 to use strlen(str).
 * @return True if the string was packed successfully; otherwise false.
 * @details Stores the string length first, then copies the character data into
 *          the internal buffer and advances the cursor.
 */
int BlockBuffer :: Pack (const char * str, short size)
// set the value of the next field of the buffer;
// if size = -1 (default) use strlen(str) as length of field
{
    short len; //length of string to be packed
    if (size >= 0) len = size;
    else len = strlen (str);
    if (len > strlen(str)) // str is too short!
        return false;
    int start = NextByte; // first character to be packed
    NextByte += (len + sizeof(len));
    if (NextByte > MaxBytes) return false;
    memcpy (&Buffer[start], &len, sizeof(len));
    strncpy (&Buffer[start+sizeof(len)], str, len);
    BufferSize = NextByte;
    return true;
}

/**
 * @brief Unpacks the next length-prefixed string from the buffer.
 * @param str Destination buffer that receives the unpacked string.
 * @return True if a string was unpacked successfully; otherwise false.
 * @details Reads the stored length, copies the matching bytes to the output,
 *          and appends a null terminator.
 */
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

/**
 * @brief Prints a summary of the buffer state.
 * @param stream Output stream used for display.
 */
void BlockBuffer :: Print (ostream & stream) const
{
    stream << "Buffer has characters "<< MaxBytes<<" and Buffer Size "
        <<BufferSize<<endl;
}


/**
 * @brief Initializes the buffer storage.
 * @param maxBytes Maximum number of bytes the buffer can hold.
 * @return True if initialization succeeded.
 * @details Allocates the internal byte array and resets the buffer state.
 */
int BlockBuffer:: Init (int maxBytes)
// construct with a maximum of maxFields

{
    if (maxBytes < 0) maxBytes = 0;
    MaxBytes = maxBytes;
    Buffer = new char[MaxBytes];
    Clear ();
    return 1;
    

}









