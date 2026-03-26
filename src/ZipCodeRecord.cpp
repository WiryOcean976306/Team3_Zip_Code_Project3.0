/**
*@file ZipCodeRecord.cpp
*@author Ken Tabata & Gabriel K.Coffie Boafo
*@date 2026-02-12
*@brief Implementation file for the ZipCodeRecord class, which provides a data structure for storing zip code information.
 */
#include "../include/ZipCodeRecord.h"
/**
 * @brief Default constructor for ZipCodeRecord that initializes all member variables to default values.
 * The zip code and state are initialized to empty strings, while latitude and longitude are initialized to 0.0.
 * This constructor allows for the creation of a ZipCodeRecord object without providing any initial data, which can be useful in situations where the record will be populated later or when creating placeholder records.
 */
ZipCodeRecord::ZipCodeRecord() : zip(""), state(""), latitude(0.0), longitude(0.0) {}

/**
 * @brief Constructor for ZipCodeRecord that initializes the member variables with the provided parameters.
 * @param zip The zip code to be stored in the record.
 * @param state The state ID to be stored in the record.
 * @param latitude The latitude of the city to be stored in the record.
 * @param longitude The longitude of the city to be stored in the record.
 * This constructor allows for the creation of a ZipCodeRecord object with specific data, which can be useful when the record needs to be initialized with known values.
 */
ZipCodeRecord::ZipCodeRecord(std::string& zip, std::string& state, double& latitude, double& longitude) 
    : zip(zip), state(state), latitude(latitude), longitude(longitude) {}

/**
 * @brief Getter for the state ID of the zip code record.
 * @return A const reference to the state ID string stored in the record.
 * This method provides read-only access to the state ID of the zip code record, allowing other parts of the program to retrieve this information without modifying it.
 */
const std::string& ZipCodeRecord::getState() const {
    return state;
};

/**
 * @brief Getter for the zip code of the zip code record.
 * @return A const reference to the zip code string stored in the record.
 * This method provides read-only access to the zip code of the record, allowing other parts of
 * the program to retrieve this information without modifying it.
 */
const std::string& ZipCodeRecord::getZip() const {
    return zip;
}

/**
 * @brief Getter for the latitude of the zip code record.
 * @return A const reference to the latitude value stored in the record.
 * This method provides read-only access to the latitude of the record, allowing other parts of the
 *  program to retrieve this information without modifying it.
 */
const double& ZipCodeRecord::getLatitude() const {
    return latitude;
}

/**
 * @brief Getter for the longitude of the zip code record.
 * @return A const reference to the longitude value stored in the record.
 * This method provides read-only access to the longitude of the record, allowing other parts of the
 * program to retrieve this information without modifying it.
 */
const double& ZipCodeRecord::getLongitude() const {
    return longitude;
}
