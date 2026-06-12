#!/bin/bash

#cfg here
bootFile="../../../boot_loader/TL721X/boot_loader_rtc_xtal.bin"
single_firmware="../../build/TL721X/recording_card/recording_card.bin"
d25fBinVersion="1"
singleBinType="1"
#cfg here

otaFile="./download_single_ota.bin"
totalFile="./all.bin"



if [ ! -f "$single_firmware" ]; then
    echo "File $single_firmware not found!"
    exit 1
fi


echo "Please wait for a moment"

tempFile=$(mktemp)

./boot.sh "$bootFile" > "$tempFile"

./ota_info.sh >> "$tempFile"

./img.sh "$single_firmware" "$d25fBinVersion"  "0" "0"   "0" "0"  "$singleBinType" >> "$tempFile" #create img block

#create bin_info
#arg 0:bin_file 1:start addr 2:featureMap
./bin_info.sh "$single_firmware" "0x13040" "0" >> "$tempFile" #bin


cat "$tempFile" > "$otaFile"

rm -f "$tempFile"

echo "ota bin create now"