#!/bin/bash

binPID=0x00000001
binVID=0x00000001

binFile="$1"
binStartAddr="$2"
binFeatureMap="$3"

if [ ! -f "$binFile" ]; then
    echo "Error: File not found." >&2
    exit 1
fi

output_byte() {
    local value=$1
    printf "\x$(printf "%02x" "$value")"
}

tempFull=$(mktemp)

# 1. header 32 bytes
{
    # PID (4 bytes)
    output_byte $((binPID & 0xFF))
    output_byte $(((binPID >> 8) & 0xFF))
    output_byte $(((binPID >> 16) & 0xFF))
    output_byte $(((binPID >> 24) & 0xFF))
    
    # VID (4 bytes)
    output_byte $((binVID & 0xFF))
    output_byte $(((binVID >> 8) & 0xFF))
    output_byte $(((binVID >> 16) & 0xFF))
    output_byte $(((binVID >> 24) & 0xFF))
    
    # StartAddr (4 bytes)
    output_byte $((binStartAddr & 0xFF))
    output_byte $(((binStartAddr >> 8) & 0xFF))
    output_byte $(((binStartAddr >> 16) & 0xFF))
    output_byte $(((binStartAddr >> 24) & 0xFF))
    
    # FeatureMap (8 bytes)
    output_byte $((binFeatureMap & 0xFF))
    output_byte $(((binFeatureMap >> 8) & 0xFF))
    output_byte $(((binFeatureMap >> 16) & 0xFF))
    output_byte $(((binFeatureMap >> 24) & 0xFF))
    output_byte $(((binFeatureMap >> 32) & 0xFF))
    output_byte $(((binFeatureMap >> 40) & 0xFF))
    output_byte $(((binFeatureMap >> 48) & 0xFF))
    output_byte $(((binFeatureMap >> 56) & 0xFF))
    
    # Reserved (12 bytes)
    for _ in {1..12}; do printf "\0"; done
    
} > "$tempFull"

crcblock=$(cat "$tempFull" | ./crc32.sh)
# 2.crc 4 byte ,28 resv
{
    # crcblock (4 bytes)
    output_byte $((0x${crcblock:6:2} & 0xFF))
    output_byte $((0x${crcblock:4:2} & 0xFF))
    output_byte $((0x${crcblock:2:2} & 0xFF))
    output_byte $((0x${crcblock:0:2} & 0xFF))
       
    # Reserved (28 bytes)
    for _ in {1..28}; do printf "\0"; done
    
} >> "$tempFull"

# 3. append original bin
cat "$binFile" >> "$tempFull"

# 4. compute CRC32 over header(32) + bin
crc32=$(cat "$tempFull" | ./crc32.sh)

{
    # CRC32 (4 bytes)
    output_byte $((0x${crc32:6:2} & 0xFF))
    output_byte $((0x${crc32:4:2} & 0xFF))
    output_byte $((0x${crc32:2:2} & 0xFF))
    output_byte $((0x${crc32:0:2} & 0xFF))
    
    # CRC reserved (16 + 12 = 28 bytes)
    for _ in {1..28}; do printf "\0"; done
} >> "$tempFull"

# 5. 4096-align with 0xFF
totalSize=$(stat -c%s "$tempFull")
padding=$(((4096 - (totalSize % 4096)) % 4096))
if [ $padding -gt 0 ]; then
    dd if=/dev/zero bs=1 count=$padding 2>/dev/null | tr "\000" "\377" >> "$tempFull"
fi

cat "$tempFull"
rm -f "$tempFull"