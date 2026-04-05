/**
 * @file BlockBuffer.h
 * @author Robert Shores
 * @date 2026-03-31
 * @brief declaration file for the BlockBuffer class which unpacks a record from
 * a block into a record buffer. Reads/writes whole fixed-size blocks (default 512 bytes).
 *
 * Knows block metadatea layout: record count, prev RBN, next RBN
 */



 #ifndef BLOCKBUFFER_H
 #define BLOCKBUFFER_H

 #include <iostream>
 #include <fstream>
 #include <string>
 #include <memory>
#include <cstring>
 #include "Block.h"
 


 /**
  * @brief unpacks a record form a black into a record buffer
  * @details
  */

  class BlockBuffer {
    private:
        std::ifstream file; /** Input file stream */
        std::string csvPath;
        bool BufferRead;
        
    public:

        /**
         * @brief Initilizes the BlockBuffer 
         * @param Block The block to be unpacked into a record buffer
         */


        BlockBuffer (int maxBytes = 512);

        
        void Clear (); // clear fields from buffer
        int Read (istream& stream);
        int Write (ostream& stream) const;
        int Pack (const char*, short size = -1);
            // set the value of the next field of the buffer
        int Unpack (char *);
            // extract the value of the next field of the buffer
        void Print (ostream&) const;
        int Init (int maxBytes = 512);

    private:

        char * Buffer; // character array to hold field values
        int BufferSize; // size of packed fields
        int MaxBytes; // maximum number of characters in the buffer
        int NextByte; // packing/unpacking position in the buffer



  };




#endif