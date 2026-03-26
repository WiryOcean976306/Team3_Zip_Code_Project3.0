/**
*@file printerCSV.cpp
*@author Robert Shores, Dade Kutzera, Zach Houck
*@date 2026-02-10
*@brief Implementation file for the PrinterCSV class, which provides methods for printing the results of the analysis.
 */

#ifndef PRINTERCSV_CPP
#define PRINTERCSV_CPP


#include "../include/PrinterCSV.h"
#include "../include/analyzer.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
    
/**
 * @brief Prints the table of state extremes, which includes the easternmost, westernmost, northernmost, and southernmost zip codes for each state.
 * @param results A map of state extremes, where the key is the state ID and the value is a StateExtremes object containing the extreme zip code records for that state.
 */
void PrinterCSV::printCSV(std::ostream& outFile, std::map<std::string, StateExtremes> results) {

    // Print header with column labels
    /**
    * @brief Output the column headers for the table of extremes.
    *
    * Columns are fixed-width to provide a clean, aligned display. The
    * values correspond to the state abbreviation and each directional
    * extreme.
    */
    outFile << "State,Easternmost,Westernmost,Northernmost,Southernmost" << std::endl;

    // Iterate over the map 
    /**
     * @brief Lamba to convert a ZipCodeRecord into a printable string  (May need to be tweaked depending on what ZipCodeRecord Contains)
     */
    auto formatRecord = [](const ZipCodeRecord& r) {
        std::ostringstream oss;
        oss << r.getZip();
        return oss.str();
    };


    auto it = results.begin();
    if (it != results.end()) {
        do {
            const std::string& state = it->first;
            const StateExtremes& e = it->second;

            if (!e.initialized) {
                ++it;
                continue;
            }

            outFile << state << ","
                    << formatRecord(e.easternmost) << ","
                    << formatRecord(e.westernmost) << ","
                    << formatRecord(e.northernmost) << ","
                    << formatRecord(e.southernmost)
                    << std::endl;

            ++it;
        } while (it != results.end());
    }

}

#endif
