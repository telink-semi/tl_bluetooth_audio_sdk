#!/bin/bash

d25fBinFile=$1
n22BinFile=$3
dspBinFile=$5
d25fBinVersion=$2
n22BinVersion=$4
dspBinVersion=$6

d25fBinType=$7
n22BinType=$8
dspBinType=$9
binCount=0
resv=0

output_byte() {
    local value=$1
    printf "\x$(printf "%02x" "$value")"
}
tempFile=$(mktemp)

if [ -f "$d25fBinFile" ]; then
    binCount=$((binCount + 1))
fi

if [ -f "$n22BinFile" ]; then
    binCount=$((binCount + 1))
fi

if [ -f "$dspBinFile" ]; then
    binCount=$((binCount + 1))
fi

{
    # binCount (4 bytes)
    output_byte $((binCount & 0xFF))
    output_byte $(((binCount >> 8) & 0xFF))
    output_byte $(((binCount >> 16) & 0xFF))
    output_byte $(((binCount >> 24) & 0xFF))
} > "$tempFile"

if [ -f "$d25fBinFile" ]; then
    {
        # type (4 bytes)
        output_byte $((d25fBinType & 0xFF))
        output_byte $(((d25fBinType >> 8) & 0xFF))
        output_byte $(((d25fBinType >> 16) & 0xFF))
        output_byte $(((d25fBinType >> 24) & 0xFF))
        # version (4 bytes)
        output_byte $((d25fBinVersion & 0xFF))
        output_byte $(((d25fBinVersion >> 8) & 0xFF))
        output_byte $(((d25fBinVersion >> 16) & 0xFF))
        output_byte $(((d25fBinVersion >> 24) & 0xFF))
    } >> "$tempFile"
fi

if [ -f "$n22BinFile" ]; then
    {
        # type (4 bytes)
        output_byte $((n22BinType & 0xFF))
        output_byte $(((n22BinType >> 8) & 0xFF))
        output_byte $(((n22BinType >> 16) & 0xFF))
        output_byte $(((n22BinType >> 24) & 0xFF))
        # version (4 bytes)
        output_byte $((n22BinVersion & 0xFF))
        output_byte $(((n22BinVersion >> 8) & 0xFF))
        output_byte $(((n22BinVersion >> 16) & 0xFF))
        output_byte $(((n22BinVersion >> 24) & 0xFF))
    } >> "$tempFile"
fi

if [ -f "$dspBinFile" ]; then
    {
        # type (4 bytes)
        output_byte $((dspBinType & 0xFF))
        output_byte $(((dspBinType >> 8) & 0xFF))
        output_byte $(((dspBinType >> 16) & 0xFF))
        output_byte $(((dspBinType >> 24) & 0xFF))
        # version (4 bytes)
        output_byte $((dspBinVersion & 0xFF))
        output_byte $(((dspBinVersion >> 8) & 0xFF))
        output_byte $(((dspBinVersion >> 16) & 0xFF))
        output_byte $(((dspBinVersion >> 24) & 0xFF))
    } >> "$tempFile"
fi

{
    # resv (4 bytes)
    output_byte $((resv & 0xFF))
    output_byte $(((resv >> 8) & 0xFF))
    output_byte $(((resv >> 16) & 0xFF))
    output_byte $(((resv >> 24) & 0xFF))
} >> "$tempFile"

cat "$tempFile"
rm -f "$tempFile"