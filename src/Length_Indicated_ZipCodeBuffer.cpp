/**
*@file Length_Indicated_ZipCodeBuffer.cpp
*@author Ken Tabata & Gabriel K Coffie Boafo & Robert Shoes
*@date 2026-02-11
*@brief Implementation file for the Length_Indicated_ZipCodeBuffer class, which provides methods for reading and writing length-indicated Zip Code records.
 */

#include "../include/Length_Indicated_ZipCodeBuffer.h"
#include <stdexcept>  // for std::runtime_error
#include <sstream>    // for std::stringstream
#include <iomanip>    // for stream formatting
#include <cstring>    // for string operations
#include <cstdint> 

/**
 * @brief Constructor for Length_Indicated_ZipCodeBuffer that opens the specified length-indicated file.
 * @param filePath The path to the length-indicated file containing zip code records.
 * @throws std::runtime_error if the file cannot be opened.
 * @details Opens the file for reading length-indicated records. Each record is preceded by a size indicator that tells how many bytes the record contains.
 */
Length_Indicated_ZipCodeBuffer::Length_Indicated_ZipCodeBuffer(const std::string& filePath) {
    file.open(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }
}

/**
 * @brief Reads the next length-indicated record from the file and unpacks it into a ZipCodeRecord object.
 * @param recordOut Output parameter that will hold the unpacked ZipCodeRecord.
 * @return true if a record was successfully read and unpacked, false if end of file is reached.
 * @details Reads a size indicator (integer) followed by the record data. The record is parsed into zip code, state, latitude, and longitude fields, then constructed into a ZipCodeRecord object.
 */
bool Length_Indicated_ZipCodeBuffer::readNext(ZipCodeRecord& recordOut) {
    // Project file format is text length-indicated rows:
    // <len>,<zip>,<city>,<state>,<county>,<lat>,<long>
    // plus a metadata header row that should be skipped.
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        std::string lenText;
        std::string zip;
        std::string city;
        std::string state;
        std::string county;
        std::string latText;
        std::string longText;

        if (!std::getline(ss, lenText, ',')) continue;
        if (!std::getline(ss, zip, ',')) continue;
        if (!std::getline(ss, city, ',')) continue;
        if (!std::getline(ss, state, ',')) continue;
        if (!std::getline(ss, county, ',')) continue;
        if (!std::getline(ss, latText, ',')) continue;
        if (!std::getline(ss, longText, ',')) continue;

        try {
            double latitude = std::stod(latText);
            double longitude = std::stod(longText);
            recordOut = ZipCodeRecord(zip, state, latitude, longitude);
            return true;
        }
        catch (const std::invalid_argument&) {
            // Skip metadata or malformed rows and keep scanning.
            continue;
        }
        catch (const std::out_of_range&) {
            // Skip malformed rows and keep scanning.
            continue;
        }
    }

    return false;
}

/**
 * @brief Packs a ZipCodeRecord into a length-indicated format string.
 * @param record The ZipCodeRecord to pack.
 * @return A string containing the packed record in "size,data" format where size is the byte count.
 * @details Converts the record fields into a comma-separated string, prefixes it with a 4-byte size indicator, and returns the combined data suitable for writing to a file.
 */
std::string Length_Indicated_ZipCodeBuffer::packRecord(ZipCodeRecord& record) const {
    // Build the comma-separated data string
    std::stringstream dataStream;
    dataStream << record.getZip() << ","
               << record.getState() << ","
               << std::fixed << std::setprecision(6) << record.getLatitude() << ","
               << std::fixed << std::setprecision(6) << record.getLongitude();
    
    std::string dataStr = dataStream.str();
    
    // Create output with size prefix (4-byte unsigned integer)
    std::stringstream output;
    uint32_t size = static_cast<uint32_t>(dataStr.length());
    
    // Write size as binary
    output.write(reinterpret_cast<const char*>(&size), sizeof(uint32_t));
    // Write data
    output.write(dataStr.c_str(), dataStr.length());
    
    return output.str();
}

/**
 * @brief Destructor for Length_Indicated_ZipCodeBuffer that closes the file stream if it is open.
 * @details Ensures proper cleanup of the file stream resource when the object is destroyed.
 */
Length_Indicated_ZipCodeBuffer::~Length_Indicated_ZipCodeBuffer() {
    if (file.is_open()) {
        file.close();
    }
}