/**
 * @file RecordFormatter.cpp
 * @author Gabriel Kofi Coffie Boafo & Ken Tabata
 * @date 2026-02-14
 * @brief Implements the RecordFormatter class, which provides methods for packing and unpacking ZipCode
 */


#include "RecordFormatter.h"
#include <iomanip> // for std::setprecision

std::string RecordFormatter::pack(const ZipCodeRecord& record) {
    // Serialize each field as "<length>:<value>" so the output is a self-describing, length-indicated string.
    // This makes it easy to parse records even if fields contain delimiters.
    std::ostringstream oss;

    const std::string& zip = record.getZip();
    const std::string& state = record.getState();

    // We store latitude and longitude as text with fixed precision.
    std::ostringstream latOss;
    latOss << std::fixed << std::setprecision(4) << record.getLatitude();
    std::string latStr = latOss.str();
 
    // Longitude is also stored as text with fixed precision.
    std::ostringstream longOss;
    longOss << std::fixed << std::setprecision(4) << record.getLongitude();
    std::string longStr = longOss.str();

    // Helper lambda to append a field with its length prefix.
    auto append = [&](const std::string& field) {
        oss << field.size() << ":" << field;
    };

    append(zip);
    append(state);
    append(latStr);
    append(longStr);

    return oss.str();
}

ZipCodeRecord RecordFormatter::unpack(const std::string& packedRecord) {
    // Parse a length-indicated record in the format produced by pack().
    size_t pos = 0;

    auto readField = [&](void) {
        if (pos >= packedRecord.size()) {
            return std::string();
        }

        // Read length prefix
        size_t colon = packedRecord.find(':', pos);
        if (colon == std::string::npos) {
            return std::string();
        }

        std::string lenStr = packedRecord.substr(pos, colon - pos);
        size_t fieldLen = static_cast<size_t>(std::stoul(lenStr));
        pos = colon + 1;

        std::string value = packedRecord.substr(pos, fieldLen);
        pos += fieldLen;
        return value;
    };
 
    // Read fields in the order they were packed: zip, state, latitude, longitude.
    std::string zip = readField();
    std::string state = readField();
    std::string latStr = readField();
    std::string longStr = readField();

    double latitude = 0.0;
    double longitude = 0.0;
    try {
        latitude = std::stod(latStr);
        longitude = std::stod(longStr);
    } catch (...) {
        // If conversion fails, leave zeros.
    }

    return ZipCodeRecord(zip, state, latitude, longitude);
}

// This method formats a ZipCodeRecord into user friendly labeled format for easy reading.

std::string RecordFormatter::formatLabeled(const ZipCodeRecord& record) {
    std::ostringstream oss;
    oss << "zipcode: " << record.getZip()
        << " | State: " << record.getState()
        << " | Lat: " << std::fixed << std::setprecision(4) << record.getLatitude()
        << " | long: " << std::fixed << std::setprecision(4) << record.getLongitude();
    return oss.str();
}

