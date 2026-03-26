/**
*@file Printer.h
*@author Robert Shores
*@date 2026-02-10
*@brief Header file for the Printer class, which provides methods for printing the results of the analysis.
 */

#ifndef PRINTER_H
#define PRINTER_H

#include "StateExtremes.h"
#include <map>


/**
 * @brief The Printer class is responsible for printing the results of the analysis in a formatted table.
 * It provides a method to print the table of state extremes, which includes the easternmost,
 */
class Printer
   {
    public:

    /**
     * @brief Prints the table of state extremes, which includes the easternmost, westernmost, northernmost, and southernmost zip codes for each state.
     * @param results A map of state extremes, where the key is the state ID and the value is a StateExtremes object containing the extreme zip code records for that state.
     */
    void printTable(std::map<std::string, StateExtremes> results);

   };

#endif