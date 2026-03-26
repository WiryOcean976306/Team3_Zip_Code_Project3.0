/**
*@file analyzer.h
*@author Robert Shores & Zach Houck
*@date 2026-02-10
*@brief Header file for the Analyzer class, which provides methods for analyzing data sets.
 */

 #ifndef ANALYZER_H
 #define ANALYZER_H


 
 #include <map>
 #include <string>
 #include "ZipCodeRecord.h"
 #include "StateExtremes.h"

 class Analyzer 
    {

        private:
            /**
            * @brief Map from state ID to that state's extremes
             */
            std::map<std::string, StateExtremes> results;
        
             


        public:
            /**
            * @brief consumes one ZipCodeRecord and updates per state extremes.
            * @param record Parsed CSV row
            * @details The consume function takes a ZipCodeRecord object as input and updates the per state.
            * @return void
            */

         void consume(const ZipCodeRecord& record);


          /** 
            * @brief Returns the results of ComputeExtremes
            * @param State The state for which to return the extremes
            * @details The map is returned as a const reference to avoid unnecessary copying of the data,
            * and const reference to ensure that the caller cannot modify the internal state of the Analyzer class,
            * while still allowing them to access the results of the analysis.
            * @return A const reference to the map of state extremes, which is a map from state ID to that state's extremes.
            */

         const std::map<std::string, StateExtremes>& getResults() const;
           
        

    };

#endif // ANALYZER_H