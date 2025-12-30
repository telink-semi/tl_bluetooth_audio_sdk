#!/bin/bash

imgVer=0x00000001
imgSize=68
binCount=0
totalSize=4096
binInfoHeadSize=96
groupCount=1
d25fBinFile=$1
n22BinFile=$2
dspBinFile=$3

if [ -f "$d25fBinFile" ]; then
    binCount=$((binCount + 1))
    oriBinSize=$(stat -c %s "$d25fBinFile")
    alignedBinSize=$((( (oriBinSize + binInfoHeadSize + 4095) & ~4095 )))
    totalSize=$((totalSize + alignedBinSize))
fi

if [ -f "$n22BinFile" ]; then
    binCount=$((binCount + 1))
    oriBinSize=$(stat -c %s "$n22BinFile")
    alignedBinSize=$((( (oriBinSize + binInfoHeadSize + 4095) & ~4095 )))
    totalSize=$((totalSize + alignedBinSize))
fi

if [ -f "$dspBinFile" ]; then
    binCount=$((binCount + 1))
    oriBinSize=$(stat -c %s "$dspBinFile")
    alignedBinSize=$((( (oriBinSize + binInfoHeadSize + 4095) & ~4095 )))
    totalSize=$((totalSize + alignedBinSize))
fi

groupsize=$((binCount * 8 + 8))   #Temp support only one group
imgSize=$((imgSize + 32 * binCount + groupsize))

output_byte() {
    local value=$1
    printf "\x$(printf "%02x" "$value")"
}

{
    # imgVer (4 bytes)
    output_byte $((imgVer & 0xFF))
    output_byte $(((imgVer >> 8) & 0xFF))
    output_byte $(((imgVer >> 16) & 0xFF))
    output_byte $(((imgVer >> 24) & 0xFF))
    
    # imgSize (4 bytes)
    output_byte $((imgSize & 0xFF))
    output_byte $(((imgSize >> 8) & 0xFF))
    output_byte $(((imgSize >> 16) & 0xFF))
    output_byte $(((imgSize >> 24) & 0xFF))
    
    # binCount (4 bytes)
    output_byte $((binCount & 0xFF))
    output_byte $(((binCount >> 8) & 0xFF))
    output_byte $(((binCount >> 16) & 0xFF))
    output_byte $(((binCount >> 24) & 0xFF))
    
    # totalSize (4 bytes)
    output_byte $((totalSize & 0xFF))
    output_byte $(((totalSize >> 8) & 0xFF))
    output_byte $(((totalSize >> 16) & 0xFF))
    output_byte $(((totalSize >> 24) & 0xFF))

    # groupCount (4 bytes)
    output_byte $((groupCount & 0xFF))
    output_byte $(((groupCount >> 8) & 0xFF))
    output_byte $(((groupCount >> 16) & 0xFF))
    output_byte $(((groupCount >> 24) & 0xFF))
}