/**
 * @file CsvToLength.h
 * @author Zach Houck
 * @date 2026-02-10
 * @brief Declares CsvToLength, a utility class that converts a CSV file into a length-indicated output file.
 */

#ifndef  CSVTOLENGTH_H
#define  CSVTOLENGTH_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <memory>

using namespace std;

class CsvToLength
{
    private:

    public:
    /**
     * @brief Converts a source CSV file into a length-indicated output file.
     * @param filePath Path to the source CSV file.
     * @details The constructor performs the conversion immediately by writing a generated
     *          header record and then writing each source row with a length prefix.
     */
    CsvToLength(const string& filePath);



};



#endif