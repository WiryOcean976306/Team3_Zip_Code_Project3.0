Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

Set-Location $PSScriptRoot

Write-Host '=== Build ==='
make clean
make bin/main
make bin/SearchIndex
make bin/TestDriver

Write-Host "`n=== Scenario 1: Add requiring no split (default block size) ==="
.\bin\main `
  -in data/Length_Indicated_small_default.txt `
  -out data/small_default_blocked_set.txt `
  -blockSize 512 -minCapacity 0.50 -startRBN 1 `
  -dump data/small_default_dump.txt `
  -physicalDump data/small_default_physical_dump.txt `
  -logicalDump data/small_default_logical_dump.txt `
  -index data/small_default_index.txt `
  -indexDump data/small_default_index_dump.txt `
  -a data/small_records_add_no_split.txt `
  -addLog data/small_default_addition_log.txt `
  -printRecords 0

Write-Host "`n=== Scenario 2: Add requiring split + avail-list reuse update ==="
Write-Host 'Deletes run before adds when both files are supplied.'
.\bin\main `
  -in data/Length_Indicated_small_merge_case.txt `
  -out data/small_split_avail_blocked_set.txt `
  -blockSize 180 -minCapacity 0.50 -startRBN 1 `
  -dump data/small_split_avail_dump.txt `
  -physicalDump data/small_split_avail_physical_dump.txt `
  -logicalDump data/small_split_avail_logical_dump.txt `
  -index data/small_split_avail_index.txt `
  -indexDump data/small_split_avail_index_dump.txt `
  -d data/small_keys_delete_merge_rightmost.txt `
  -a data/small_records_add_split.txt `
  -deleteLog data/small_split_avail_deletion_log.txt `
  -addLog data/small_split_avail_addition_log.txt `
  -printRecords 0

Write-Host "`n=== Scenario 3: Delete requiring no redistribution or merge ==="
.\bin\main `
  -in data/Length_Indicated_small_default.txt `
  -out data/small_delete_simple_blocked_set.txt `
  -blockSize 512 -minCapacity 0.50 -startRBN 1 `
  -dump data/small_delete_simple_dump.txt `
  -physicalDump data/small_delete_simple_physical_dump.txt `
  -logicalDump data/small_delete_simple_logical_dump.txt `
  -index data/small_delete_simple_index.txt `
  -indexDump data/small_delete_simple_index_dump.txt `
  -d data/small_keys_delete_no_redistribution.txt `
  -deleteLog data/small_delete_simple_deletion_log.txt `
  -printRecords 0

Write-Host "`n=== Scenario 4: Delete requiring redistribution (no merge) ==="
.\bin\main `
  -in data/Length_Indicated_small_six_capacity.txt `
  -out data/small_redistribute_blocked_set.txt `
  -blockSize 180 -minCapacity 0.50 -startRBN 1 `
  -dump data/small_redistribute_dump.txt `
  -physicalDump data/small_redistribute_physical_dump.txt `
  -logicalDump data/small_redistribute_logical_dump.txt `
  -index data/small_redistribute_index.txt `
  -indexDump data/small_redistribute_index_dump.txt `
  -d data/small_keys_delete_redistribution.txt `
  -deleteLog data/small_redistribute_deletion_log.txt `
  -printRecords 0

Write-Host "`n=== Scenario 5: Delete requiring merge with rightmost block cleared ==="
.\bin\main `
  -in data/Length_Indicated_small_merge_case.txt `
  -out data/small_merge_blocked_set.txt `
  -blockSize 180 -minCapacity 0.50 -startRBN 1 `
  -dump data/small_merge_dump.txt `
  -physicalDump data/small_merge_physical_dump.txt `
  -logicalDump data/small_merge_logical_dump.txt `
  -index data/small_merge_index.txt `
  -indexDump data/small_merge_index_dump.txt `
  -d data/small_keys_delete_merge_rightmost.txt `
  -deleteLog data/small_merge_deletion_log.txt `
  -printRecords 0

Write-Host "`n=== Search sanity check ==="
.\bin\SearchIndex `
  -data data/small_merge_blocked_set.txt `
  -index data/small_merge_index.txt `
  -blocks data/blocks `
  -Z10001 -Z10007 -Z99999

Write-Host "`nDemo script completed."
