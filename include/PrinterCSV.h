/**
*@file PrinterCSV.h
*@author Robert Shores & Zach Houck
*@date 2026-02-10
*@brief Header file for the PrinterCSV class, which provides methods for printing the results of the analysis into a CSV file.
 */

#ifndef PRINTERCSV_H
#define PRINTERCSV_H

#include "StateExtremes.h"
#include <iostream>
#include <fstream>
#include <map>


/**
 * @brief The Printer class is responsible for printing the results of the analysis in a CSV file.
 * It provides a method to print the table of state extremes, which includes the easternmost,
 */
class PrinterCSV
   {
    public:

    /**
     * @brief Prints the table of state extremes, which includes the easternmost, westernmost, northernmost, and southernmost zip codes for each state.
     * @param results A map of state extremes, where the key is the state ID and the value is a StateExtremes object containing the extreme zip code records for that state.
     */
    void printCSV(std::ostream& outFile, std::map<std::string, StateExtremes> results);

   };

#endif