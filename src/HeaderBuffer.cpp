/**
 * @file HeaderBuffer.cpp
 * @author Robert Shores
 * @date 2026-02-10
 * @brief Implements HeaderBuffer methods for header-record serialization.
 */

#ifndef  HEADBUFFER_CPP
#define  HEADBUFFER_CPP

#include "../include/HeaderBuffer.h"

/**
 * @brief Writes the serialized header to an output stream.
 * @param out Output stream for the destination file.
 * @param header Header metadata object to serialize and write.
 * @return true if the write operation succeeded; otherwise false.
 */
bool HeaderBuffer::writeHeader(std::ostream& out, const HeaderRecord& header) const {
	const std::string payload = packHeader(header);
	out << payload << "\n";
	return static_cast<bool>(out);
}

/**
 * @brief Reads and parses the header from an input stream.
 * @param in Input stream for the source file.
 * @param headerOut Output metadata object populated from parsed header text.
 * @return true if the header was read and parsed; otherwise false.
 */
bool HeaderBuffer::readHeader(std::istream& in, HeaderRecord& headerOut) const {
	std::string payload;
	if (!std::getline(in, payload)) {
		return false;
	}

	return unpackHeader(payload, headerOut);
}

/**
 * @brief Converts a HeaderRecord into serialized payload text.
 * @param header Header metadata to serialize.
 * @return Serialized payload string.
 */
std::string HeaderBuffer::packHeader(const HeaderRecord& header) const {
	return header.GetHeader();
}

/**
 * @brief Converts serialized payload text into a HeaderRecord object.
 * @param payload Serialized header payload.
 * @param headerOut Output object to populate.
 * @return true if parsing succeeded; otherwise false.
 * @details Parsing logic is a placeholder and should be replaced with
 *          full metadata field extraction/validation.
 */
bool HeaderBuffer::unpackHeader(const std::string& payload, HeaderRecord& headerOut) const {
	(void)payload;
	(void)headerOut;
	return true;
}

#endif 