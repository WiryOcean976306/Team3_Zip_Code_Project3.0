/**
*@file ZipCodeRecord.h
*@author Ken Tabata & Gabriel K.Coffie Boafo
*@date 2026-02-12
*@brief Header file for the ZipCodeRecord class, which provides a data structure for storing zip code information.
 */

 #include <string>

 #ifndef ZIPCODERECORD_H
 #define ZIPCODERECORD_H

 class ZipCodeRecord 
    {
        public:
            /**
            * @brief Default constructor for ZipCodeRecord
            */
            ZipCodeRecord();

            /**
            * @brief Constructor for ZipCodeRecord with parameters
            * @param zip The zip code
            * @param state The state ID
            * @param latitude The latitude of the city
            * @param longitude The longitude of the city
            */
            ZipCodeRecord(std::string& zip, std::string& state, double& latitude, double& longitude);

            /**
             * @brief Getter for the state ID of the zip code record
             * @return A const reference to the state ID string
             */
            const std::string& getState() const;

            /**
             * @brief Getter for the zip code of the zip code record
             * @return A const reference to the zip code string
             */
            const std::string& getZip() const;

            /**
             * @brief Getter for the latitude of the zip code record
             * @return A const reference to the latitude value
             */
            const double& getLatitude() const;

            /**
             * @brief Getter for the longitude of the zip code record
             * @return A const reference to the longitude value
             */
            const double& getLongitude() const;

        private:
            
            std::string zip; /** The zip code */
            std::string state; /** The state ID */
            double latitude; /** The latitude of the city */
            double longitude; /** The longitude of the city */
    };
#endif