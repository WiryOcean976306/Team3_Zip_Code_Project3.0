/**
 * @file BlockBuffer.cpp
 * @author Robert Shores
 * @date 2026-03-31
 * @brief Implementation file for the BlockBuffer class which unpacks a record from
 * a block into a record buffer. Reads/writes whole fixed-size blocks (default 512 bytes).
 *
 * Knows block metadatea layout: record count, prev RBN, next RBN
 */