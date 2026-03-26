/**
*@file Printer.cpp
*@author Robert Shores & Dade Kutzera
*@date 2026-02-10
*@brief Implementation file for the Printer class, which provides methods for printing the results of the analysis.
 */

#ifndef PRINTER_CPP
#define PRINTER_CPP


#include "../include/Printer.h"
#include "../include/analyzer.h"
#include <iostream>
#include <iomanip>
#include <sstream>
    
/**
 * @brief Prints the table of state extremes, which includes the easternmost, westernmost, northernmost, and southernmost zip codes for each state.
 * @param results A map of state extremes, where the key is the state ID and the value is a StateExtremes object containing the extreme zip code records for that state.
 */
void Printer::printTable(std::map<std::string, StateExtremes> results) {

    // Print header with column labels
    /**
    * @brief Output the column headers for the table of extremes.
    *
    * Columns are fixed-width to provide a clean, aligned display. The
    * values correspond to the state abbreviation and each directional
    * extreme.
    */
    std::cout << std::left << std::setw(7) << "State"
              << std::setw(30) << "Easternmost"
              << std::setw(30) << "Westernmost"
              << std::setw(30) << "Northernmost"
              << std::setw(30) << "Southernmost" << "\n";
    std::cout << std::string((7 + 30 * 4), '-') << "\n";


    // Iterate over the map 
    /**
     * @brief Lamba to convert a ZipCodeRecord into a printable string  (May need to be tweaked depending on what ZipCodeRecord Contains)
     */
      auto formatRecord = [](const ZipCodeRecord& r) {
        std::ostringstream oss;
        oss << r.getZip() << " (" << r.getLongitude() << ", " << r.getLatitude() << ")";
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

        std::cout << std::left << std::setw(7) << state
                  << std::setw(30) << formatRecord(e.easternmost)
                  << std::setw(30) << formatRecord(e.westernmost)
                  << std::setw(30) << formatRecord(e.northernmost)
                  << std::setw(30) << formatRecord(e.southernmost)
                  << "\n";

        ++it;
    } while (it != results.end());
}


}

#endif
