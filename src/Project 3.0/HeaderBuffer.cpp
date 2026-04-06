/**
 * @file HeaderBuffer.cpp
 * @author Robert Shores
 * @date 2026-02-10
 * @brief Implements HeaderBuffer methods for header-record serialization.
 */

#ifndef  HEADBUFFER_CPP
#define  HEADBUFFER_CPP

#include "Project 3.0/HeaderBuffer.h"
#include <sstream>
#include <vector>

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
 * @details Parses both core schema fields and Project 3.0 blocked-sequence
 *          metadata fields (block size, capacities, list heads, stale flag).
 */
bool HeaderBuffer::unpackHeader(const std::string& payload, HeaderRecord& headerOut) const {
	std::vector<std::string> fields;
	std::stringstream ss(payload);
	std::string token;
	while (std::getline(ss, token, ','))
	{
		fields.push_back(token);
	}

	if (fields.size() < 21)
	{
		return false;
	}

	try
	{
		std::size_t index = 0;
		headerOut.HeaderSizeBytes = std::stoi(fields[index++]);
		headerOut.FileStructureType = fields[index++];
		headerOut.Version = fields[index++];
		headerOut.SizeFormatType = (fields[index++] == "Binary");
		headerOut.SizeInclusion = (fields[index++] == "TRUE");
		headerOut.PrimaryKeyIndex = fields[index++];
		headerOut.RecordCount = std::stoi(fields[index++]);
		headerOut.FieldCount = std::stoi(fields[index++]);

		headerOut.Fields.clear();
		headerOut.FieldTypes.clear();
		for (int i = 0; i < headerOut.FieldCount; ++i)
		{
			headerOut.Fields.push_back(fields[index++]);
			headerOut.FieldTypes.push_back(fields[index++]);
		}

		headerOut.PrimaryKey = std::stoi(fields[index++]);

		if (index < fields.size()) headerOut.BlockSize = std::stoi(fields[index++]);
		if (index < fields.size()) headerOut.MinBlockCapacity = std::stoi(fields[index++]);
		if (index < fields.size()) headerOut.BlockCount = std::stoi(fields[index++]);
		if (index < fields.size()) headerOut.availListHeadRBN = std::stoi(fields[index++]);
		if (index < fields.size()) headerOut.ActiveListHeadRBN = std::stoi(fields[index++]);
		if (index < fields.size()) headerOut.StaleFlag = (fields[index++] == "TRUE");

		headerOut.RebuildHeader();
		return true;
	}
	catch (...)
	{
		return false;
	}
}

#endif 