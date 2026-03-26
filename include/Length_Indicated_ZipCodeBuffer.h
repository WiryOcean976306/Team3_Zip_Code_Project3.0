/**
*@file Length_Indicated_ZipCodeBuffer.h
*@author Ken Tabata & Gabriel K Coffie Boafo & Robert Shores
*@date 2026-02-10
*@brief Declares the LengthIndicatedZipCodeBuffer class, which reads and writes
 *        length indicated Zip Code records.
 */

 #ifndef  LENGTH_INDICATED_ZIPCODEBUFFER_H
 #define  LENGTH_INDICATED_ZIPCODEBUFFER_H
 
 #include <iostream>
 #include <fstream>
 #include <string>
 #include <memory>
 #include "ZipCodeRecord.h"

    /**
            * @brief Provides seqential access to zip code records from a data source.
            @details 
            *Opens a CSV file 
            *Skips the header line 
            *Reads each line, parses it into a ZipCodeRecord, and stores it in a buffer for later retrieval.
            * Returns one record at a time, allowing for efficient memory usage when processing large datasets.
    */

 class Length_Indicated_ZipCodeBuffer{
    private:
          std::ifstream file; /** Input file stream for reading zip code data */

          
    public:     
        /**
         * @brief Constructor that initializes the ZipCodeBuffer with the specified file path.
         * @param filePath The path to the CSV file containing zip code data.
         */
        Length_Indicated_ZipCodeBuffer(const std::string& filePath);


        /**
         * @brief  Reads the next CSV row
         * @param record Output parameter that will hold the next ZipCodeRecord read from the file.
         * @return true if a record was successfully read, false if end of file is reached.
         */
        bool readNext(ZipCodeRecord& recordOut);



        /**
         * @brief Pack Record into length indicated file
         * @param record 
         * @return 
         */
        std::string packRecord(ZipCodeRecord& record) const;



        /**
         * @brief Destructor that closes the file stream.
         */
        ~Length_Indicated_ZipCodeBuffer();
 };
// #include "ZipCodeBuffer.cpp" // This is not needed since we will compile the .cpp file separately, and including it here can lead to multiple definition errors.
#endif