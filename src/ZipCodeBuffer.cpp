/**
*@file ZipCodeBuffer.cpp
*@author Ken Tabata & Gabriel K Coffie Boafo
*@date 2026-02-11
*@brief Implementation file for the ZipCodeBuffer class, which provides methods for reading and managing zip code data.
 */

#include "../include/ZipCodeBuffer.h"
#include <stdexcept> // added for std::runtime_error
#include <sstream> // added for std::stringstream


/**
 * @brief Constructor for ZipCodeBuffer that opens the specified CSV file and reads the header to determine column indices.
 * @param filePath The path to the CSV file containing zip code data.
 * @throws std::runtime_error if the file cannot be opened.
 */
ZipCodeBuffer::ZipCodeBuffer(const std::string& filePath) {
    csvPath = filePath;
    file.open(csvPath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + csvPath);
    }
    std::string header;
    std::getline(file, header); // reads the first row of the CSV and stores it in header
    // Parse the header line to determine the indices of the relevant columns (zip code, state, latitude, longitude)
    std::stringstream ss(header);
    std::string column;
    int index = 0;
    while (std::getline(ss, column, ',')) {
        if (column == "ZipCode") {
            zipIndex = index;
        } else if (column == "State") {
            stateIndex = index;
        } else if (column == "Lat") {
            latIndex = index;
        } else if (column == "Long") {
            lonIndex = index;
        }
        index++;
    }
    headerRead = true;
}

/**
 * @brief Reads the next line from the CSV file and parses it into a ZipCodeRecord object.
 * @param recordOut Output parameter that will hold the next ZipCodeRecord read from the file.
 * @return true if a record was successfully read, false if end of file is reached.
 * @details The method reads the next line from the CSV file, splits it into columns based on the previously determined indices, and constructs a ZipCodeRecord object with the parsed data. If the end of the file is reached, it returns false to indicate that there are no more records to read.
 */
bool ZipCodeBuffer::readNext(ZipCodeRecord& recordOut) {
    std::string line;
    if (std::getline(file, line)) {
        
        std::stringstream ss(line);
        std::string zip, state;
        double latitude, longitude;
        std::string column;
        int index = 0;

        // Parse the line based on the previously determined column indices
        while (std::getline(ss, column, ',')) {
            if (index == zipIndex) {
                zip = column;
            } else if (index == stateIndex) {
                state = column;
            } else if (index == latIndex) {
                latitude = std::stod(column);
            } else if (index == lonIndex) {
                longitude = std::stod(column);
            }
            index++;
        }
        recordOut = ZipCodeRecord(zip, state, latitude, longitude);
        return true;
    }
    return false; // No more records to read
}

/**
 * @brief Destructor for ZipCodeBuffer that closes the file stream if it is open.
 * @details The destructor checks if the file stream is open and closes it to release any resources associated with the file. This ensures that the file is properly closed when the ZipCodeBuffer object goes out of scope or is destroyed.
 */
ZipCodeBuffer::~ZipCodeBuffer() {
    if (file.is_open()) {
        file.close();
    }
}