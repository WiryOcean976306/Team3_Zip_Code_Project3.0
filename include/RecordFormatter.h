/**
 * @file RecordFormatter.h
 * @author Gabriel Kofi Coffie Boafo & Ken Tabata
 * @date 2026-02-14
 * @brief Declares the RecordFormatter class, which provides methods for packing and unpacking ZipCodeRecord objects into a length-indicated format.
 */

    #ifndef RECORDFORMATTER_H
    #define RECORDFORMATTER_H

   #include "ZipCodeRecord.h"
   #include <string>
   #include <sstream>

    /**
     * @brief The RecordFormatter class provides methods for packing and unpacking ZipCodeRecord objects into a length-indicated format.
     * It allows for efficient serialization and deserialization of ZipCodeRecord data, making it easier to store and transmit records in a compact format.
     */
    class RecordFormatter {
    public:

      //   Packs a ZipCodeRecord into a length-indicated string format.
        
        static std::string pack(const ZipCodeRecord& record);

         // Unpacks a length-indicated string into a ZipCodeRecord object.
         
        static ZipCodeRecord unpack(const std::string& packedRecord);

        /**
         * @brief Formats a ZipCodeRecord into a labeled string for easy reading.
         *
         * Example output:
         *   zipcode: 00501 | State: NY | Lat: 40.8154 | long: -73.0451
         *
         * @param record The record to format.
         * @return A single-line string with labeled fields.
         */
        static std::string formatLabeled(const ZipCodeRecord& record);
    };

    #endif // RECORDFORMATTER_H
