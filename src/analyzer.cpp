/**
*@file analyzer.cpp
*@author Robert Shores
*@date 2026-02-12
*@brief Implementation of the Analyzer class for data processing.
 */

 #include "../include/analyzer.h"
 #include "../include/StateExtremes.h"


/**
* @brief consumed ZipCodeRecord objects one at a time and updates per extreme values using coordinate comparisons
* @param record Parsed CSV row as a ZipCodeRecord object
* @details The consume function takes a ZipCodeRecord object as input and updates the per state. 
  @return void
 */
 void Analyzer::consume(const ZipCodeRecord& record) {

    StateExtremes& extreme = results[record.getState()];

    if (!extreme.initialized)
    {
        extreme.initialized = true;
        extreme.easternmost = record;
        extreme.westernmost = record;
        extreme.northernmost = record;
        extreme.southernmost = record;
        return;
    }
// Easternmost: least longitude
    if (record.getLongitude() < extreme.easternmost.getLongitude() || record.getLongitude() == extreme.easternmost.getLongitude() && record.getZip() < extreme.easternmost.getZip()) {
        extreme.easternmost = record;
    }
// Westernmost: greatest longitude
    if (record.getLongitude() > extreme.westernmost.getLongitude() || record.getLongitude() == extreme.westernmost.getLongitude() && record.getZip() < extreme.westernmost.getZip()) {
        extreme.westernmost = record;
    }
// Northernmost: greatest latitude
    if (record.getLatitude() > extreme.northernmost.getLatitude() || record.getLatitude() == extreme.northernmost.getLatitude() && record.getZip() < extreme.northernmost.getZip()) {
        extreme.northernmost = record;
    }
// Southernmost: least latitude
    if (record.getLatitude() < extreme.southernmost.getLatitude() || record.getLatitude() == extreme.southernmost.getLatitude() && record.getZip() < extreme.southernmost.getZip()) {
        extreme.southernmost = record;
    }
 }

 /**
  * @brief returns the map of state extremes, which is a map from state ID to that state's extremes.
  * @return The map of state extremes as a const reference to prevent modification of the internal state of the Analyzer class.
  * @details The map is returned as a const reference to avoid unnecessary copying of the data, which can be expensive in terms of performance. By returning a const reference, we ensure that the caller cannot modify the internal state of the Analyzer class, while still allowing them to access the results of the analysis.
  * 
  * 
  */
 const std::map<std::string, StateExtremes>& Analyzer::getResults() const {
    return results;
 }
