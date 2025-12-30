#!/bin/bash

binType=$1
binVersion=$2
binAddr=$3
binSize=$4

output_byte() {
    local value=$1
    printf "\x$(printf "%02x" "$value")"
}

{
    # binType (4 bytes)
    output_byte $((binType & 0xFF))
    output_byte $(((binType >> 8) & 0xFF))
    output_byte $(((binType >> 16) & 0xFF))
    output_byte $(((binType >> 24) & 0xFF))
    
    # binVersion (4 bytes)
    output_byte $((binVersion & 0xFF))
    output_byte $(((binVersion >> 8) & 0xFF))
    output_byte $(((binVersion >> 16) & 0xFF))
    output_byte $(((binVersion >> 24) & 0xFF))
    
    # binAddr (4 bytes)
    output_byte $((binAddr & 0xFF))
    output_byte $(((binAddr >> 8) & 0xFF))
    output_byte $(((binAddr >> 16) & 0xFF))
    output_byte $(((binAddr >> 24) & 0xFF))
    
    # binSize (4 bytes)
    output_byte $((binSize & 0xFF))
    output_byte $(((binSize >> 8) & 0xFF))
    output_byte $(((binSize >> 16) & 0xFF))
    output_byte $(((binSize >> 24) & 0xFF))

    for _ in {1..16}; do printf "\0"; done # resv 16
}
