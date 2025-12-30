#!/bin/bash

output_byte() {
    local value=$1
    printf "\x$(printf "%02x" "$value")"
}

d25fBinFile=$1
d25fBinFileVersion=$2
n22BinFile=$3
n22BinFileVersion=$4
dspBinFile=$5
dspBinFileVersion=$6
d25fBinFileType=$7
n22BinFileType=$8
dspfBinFileType=$9

binAddr=4096
binExtraSize=96
tempFile=$(mktemp)

./img_head.sh "$d25fBinFile" "$n22BinFile" "$dspBinFile" > "$tempFile" #head
./img_group.sh "$1" "$2" "$3" "$4" "$5" "$6" "$7" "$8" "$9">> "$tempFile" #group

# FW Descriptors List
if [ -f "$d25fBinFile" ]; then
    oriBinSize=$(stat -c %s "$d25fBinFile")
    binSize=$((oriBinSize + binExtraSize))
    alignedBinSize=$(( (binSize + 4095) & ~4095 ))
    #arg 0:type 1:version 2:addr 3:size
    ./img_item.sh "$d25fBinFileType" "$d25fBinFileVersion" "$binAddr" "$binSize" >> "$tempFile"
    binAddr=$((binAddr + alignedBinSize))
fi

if [ -f "$n22BinFile" ]; then
    oriBinSize=$(stat -c %s "$n22BinFile")
    binSize=$((oriBinSize + binExtraSize))
    alignedBinSize=$(( (binSize + 4095) & ~4095 ))
    #arg 0:type 1:version 2:addr 3:size
    ./img_item.sh "$n22BinFileType" "$n22BinFileVersion" "$binAddr" "$binSize" >> "$tempFile"
    binAddr=$((binAddr + alignedBinSize))
fi

if [ -f "$dspBinFile" ]; then
    oriBinSize=$(stat -c %s "$dspBinFile")
    binSize=$((oriBinSize + binExtraSize))
    alignedBinSize=$(( (binSize + 4095) & ~4095 ))
    #arg 0:type 1:version 2:addr 3:size
    ./img_item.sh "$dspfBinFileType" "$dspBinFileVersion" "$binAddr" "$binSize" >> "$tempFile"
    binAddr=$((binAddr + alignedBinSize))
fi

# Reserved (16 bytes)
{
    for _ in {1..16}; do printf "\0"; done 
} >> "$tempFile"


#crc block (32 bytes)
crc32=$(cat "$tempFile" | ./crc32.sh)
{
    # CRC32 (4 bytes)
    output_byte $((0x${crc32:6:2} & 0xFF))
    output_byte $((0x${crc32:4:2} & 0xFF))
    output_byte $((0x${crc32:2:2} & 0xFF))
    output_byte $((0x${crc32:0:2} & 0xFF))
    
    # CRC reserved (28bytes)
    for _ in {1..28}; do printf "\0"; done
} >> "$tempFile"

#fill img block to 4k with 0xFF
nowBinSize=$(stat -c %s "$tempFile")
paddingSize=$((4096 - nowBinSize))
{
    for ((i=0; i<paddingSize; i++)); do
        output_byte 0xFF
    done
} >> "$tempFile"

cat "$tempFile"
rm -f "$tempFile"
   

