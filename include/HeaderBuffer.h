/**
 * @file HeaderBuffer.h
 * @author Robert Shores
 * @date 2026-02-10
 * @brief Declares the HeaderBuffer class for reading and writing header records.
 * @details HeaderBuffer serializes and deserializes the metadata record stored
 *          at the beginning of a length-indicated data file.
 */

#ifndef  HEADBUFFER_H
#define  HEADBUFFER_H

#include <istream>
#include <ostream>
#include <string>
#include "HeaderRecord.h"

class HeaderBuffer {
    public:

        /**
         * @brief Writes the header record to the output stream.
         * @param out Output stream for the target length-indicated data file.
         * @param header Header metadata model to serialize and write.
         * @return true if the header was serialized and written successfully; otherwise false.
         * @details This method is expected to write the header as the first record
         *          in the data file using the project's canonical wire format.
         */
        bool writeHeader(std::ostream& out, const HeaderRecord& header) const;



        /**
         * @brief Reads and parses the header record from the input stream.
         * @param in Input stream for the source length-indicated data file.
         * @param headerOut Output parameter populated with parsed header metadata.
         * @return true if the header was read and parsed successfully; otherwise false.
         * @details This method should be called before reading data records so
         *          record parsing logic can rely on header metadata.
         */
        bool readHeader(std::istream& in, HeaderRecord& headerOut) const;

    private:

        /**
         * @brief Serializes a HeaderRecord object into wire-format payload text.
         * @param header Header metadata object to serialize.
         * @return Serialized payload string for header storage.
         */
        std::string packHeader(const HeaderRecord& header) const;

        /**
         * @brief Deserializes wire-format payload text into a HeaderRecord object.
         * @param payload Serialized header payload text.
         * @param headerOut Output parameter populated from the payload.
         * @return true if parsing succeeded; otherwise false.
         */
        bool unpackHeader(const std::string& payload, HeaderRecord& headerOut) const;



};

#endif 