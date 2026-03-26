/**
*@file ZipCodeBuffer.h
*@author Ken Tabata & Gabriel K Coffie Boafo
*@date 2026-02-10
*@brief This declares the ZipCodeBuffer class, which is responsible for buffering and managing zip code data for efficient access and processing.
 */

 #ifndef  ZIPCODEBUFFER_H
 #define  ZIPCODEBUFFER_H
 
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

 class ZipCodeBuffer{
    private:
          std::ifstream file; /** Input file stream for reading zip code data */
          std::string csvPath; /** Input file stream for reading zip code data */
          bool headerRead; /** Flag to indicate if the header line has been read from the CSV file */
          int zipIndex; /** The index of the zip code column in the CSV file */
          int stateIndex; /** The index of the state column in the CSV file */
          int latIndex; /** The index of the latitude column in the CSV file */
          int lonIndex; /** The index of the longitude column in the CSV file */
          
    public:     
        /**
         * @brief Constructor that initializes the ZipCodeBuffer with the specified file path.
         * @param filePath The path to the CSV file containing zip code data.
         */
        ZipCodeBuffer(const std::string& filePath);
        /**
         * @brief  Reads the next CSV row
         * @param record Output parameter that will hold the next ZipCodeRecord read from the file.
         * @return true if a record was successfully read, false if end of file is reached.
         */
        bool readNext(ZipCodeRecord& recordOut);
        
        /**
         * @brief Destructor that closes the file stream.
         */
        ~ZipCodeBuffer();
 };
// #include "ZipCodeBuffer.cpp" // This is not needed since we will compile the .cpp file separately, and including it here can lead to multiple definition errors.
#endif