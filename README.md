
# Team 3 Zip Code Project 3.0

## Overview

This project implements a blocked sequence set workflow for ZIP code records, including:

1. CSV input preparation from XLSX source data.
2. Conversion to length-indicated records with a header record.
3. Generation of a blocked sequence set data store.
4. Physical and logical block dumps.
5. Simple primary key index generation, persistence, loading, and search.
6. Record insertion/deletion testing with operation logs.
7. Doxygen-based source documentation outputs and user-guide style usage notes.

The implementation is in C++17 and is built with `make`.

## Data Pipeline

### 1) XLSX -> CSV

Export the spreadsheet source to a comma-separated CSV file (for example, `data/us_postal_codes.csv`) using Excel or another spreadsheet tool.

### 2) CSV -> Length-Indicated File (with Header)

Use the CSV conversion driver to produce a length-indicated data file format. The project already includes `data/Length_Indicated_us_postal_codes.csv` as the primary input for blocked-sequence generation.

### 3) Length-Indicated -> Blocked Sequence Set

`TestDriver` reads the length-indicated file, writes blocks under `data/blocks/`, and writes a blocked-sequence header record file plus dumps and index artifacts.

## Blocked Sequence Set Architecture

### Header Record

The blocked-sequence header record is written at the beginning of the blocked-sequence-set data file and includes metadata such as:

1. Index filename
2. Record count
3. Block count
4. Block size
5. Minimum block capacity
6. Avail-list head RBN
7. Active-list head RBN
8. Stale flag

Header behavior is implemented using `HeaderRecord` and `HeaderBuffer`.

### Block Architecture

All blocks are fixed-size (default: 512 bytes payload max). Each block stores:

1. Metadata line: `RecordCount,RBN,PrevRBN,NextRBN,ByteSize,ByteMaxSize`
2. Complete packed records (one per line)

Active blocks are linked by predecessor/successor RBN pointers.

### Avail List

Unused/deleted block RBNs are tracked in an in-memory avail list for reuse. Dump headers include list-head metadata to make structural changes visible.

## Build and Run

### Build the test driver

```bash
make bin/TestDriver
```

### Run the test driver

```bash
make run-test
```

## Command-Line Options (Blocked Sequence Generation/Test Driver)

`bin/TestDriver` supports:

1. `-in <input_length_indicated_csv>`
2. `-out <blocked_sequence_set_file>`
3. `-blockSize <bytes>`
4. `-minCapacity <0.0-1.0>`
5. `-startRBN <int>`
6. `-dump <output_dump_file>`
7. `-physicalDump <physical_dump_file>`
8. `-logicalDump <logical_dump_file>`
9. `-index <index_file>`
10. `-indexDump <index_dump_file>`
11. `-printRecords <0|1>`
12. `-maxPrint <count>`
13. `-a <records_to_add_file>`
14. `-d <keys_to_delete_file>`
15. `-addLog <addition_log_file>`
16. `-deleteLog <deletion_log_file>`

### Example

```bash
./bin/TestDriver \
	-in data/Length_Indicated_us_postal_codes.csv \
	-out data/blocked_sequence_set.txt \
	-blockSize 512 -minCapacity 0.50 -startRBN 1 \
	-dump data/dumped_records.txt \
	-physicalDump data/physical_block_dump.txt \
	-logicalDump data/logical_block_dump.txt \
	-index data/simple_index.txt \
	-indexDump data/simple_index_dump.txt \
	-printRecords 1 -maxPrint 25
```

## Sequential Processing with Buffer Classes

The project uses both block-level and record-level buffering:

1. Block-level handling is performed by blocked-sequence/block classes to read/write block units.
2. Record-level handling is performed by length-indicated record buffer logic to pack/unpack record fields.
3. Records are unpacked into `ZipCodeRecord` objects for search/display operations.

## Block Dumps (Physical and Logical)

Two blocked-sequence dump modes are generated:

1. Physical block order (ascending physical block/RBN map order)
2. Logical block order (follows linked-list successor RBN pointers)

At initial creation, these may match. After non-appending avail-block reuse and structural updates, they may differ.

The dump output is intentionally diff-friendly, with head metadata and per-block key listings.

## Simple Index (Highest-Key -> RBN)

A simple primary key index is generated and stored as ordered pairs:

`<highest key in block>,<RBN>`

Artifacts:

1. Index file: `data/simple_index.txt`
2. Readable index dump: `data/simple_index_dump.txt`

The index is generated in memory, written to disk, and can be read back into RAM for search.

## Search Program Expectations

The search flow is designed to:

1. Load simple primary key index entries in RAM.
2. Use index lookup to identify candidate block(s).
3. Read and search only the indexed block(s) from blocked storage.
4. Print matching ZIP record data or a not-found message.

CLI search arguments support `-Z<zip>` style keys (multiple allowed).

## Addition and Deletion Test Runs

Use `TestDriver` with add/delete inputs:

1. Additions: `-a data/recordsToAdd.txt -addLog data/addition_log.txt`
2. Deletions: `-d data/keysToDelete.txt -deleteLog data/deletion_log.txt`

Expected logging behavior:

1. Block split events during insertions are logged.
2. Index-affecting operations should be logged.
3. Deletions log failures and successful removals.
4. Merge/redistribution events are part of the required behavior and should be logged when implemented.

## Typical Output Files

1. `data/blocked_sequence_set.txt`
2. `data/physical_block_dump.txt`
3. `data/logical_block_dump.txt`
4. `data/dumped_records.txt`
5. `data/simple_index.txt`
6. `data/simple_index_dump.txt`
7. `data/addition_log.txt`
8. `data/deletion_log.txt`
9. `data/blocks/block_*.blk`

## Demonstration Checklist

Use this list for a complete test-run demonstration:

1. Build and run blocked-sequence generation.
2. Verify header + blocks + physical/logical dumps are produced.
3. Verify simple index and index dump are produced.
4. Execute search with several valid ZIPs and at least one invalid ZIP.
5. Execute additions from an input file and capture split/index logs.
6. Execute deletions from an input file and capture delete/merge logs.
7. Re-run dumps and compare outputs (diff-friendly format).

## Documentation Requirements (Doxygen + User Guide)

The source includes Doxygen-oriented comments in headers and implementation files.

To complete deliverables:

1. Generate HTML and PDF documentation with Doxygen + LaTeX.
2. Include command-line usage examples for generation/search/add/delete.
3. Include expected output formats (dumps, index, logs).

The repository already contains generated documentation folders under `docs/html` and `docs/latex`.

## Notes on Configurability

Project variables that can vary should be set from:

1. Command-line options (with defaults), or
2. Metadata in header/index files

This aligns with the project requirement that runtime configuration is externalized rather than hardcoded.
