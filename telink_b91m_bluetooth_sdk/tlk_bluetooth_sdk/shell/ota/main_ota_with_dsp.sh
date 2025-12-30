#!/bin/bash
#cfg here
d25fFile="../../build/TL751X/bt_interphone/bt_interphone.bin"
n22File="../../build/TL751X/controller/BT_INTERPHONE/bt_interphone_controller.bin"
dspFile="../../../dsp/bin/dsp_audio_sdk_v0.1.0.2_for_ram_boot.bin"
d25fBinVersion="1"
n22BinVersion="1"
dspBinVersion="1"

d25fBinType="1"
n22BinType="2"
dspBinType="3"
#cfg here

otaFile="./ota_firmware.bin"
totalFile="./all.bin"

if [ ! -f "$d25fFile" ]; then
    echo "File $d25fFile not found!"
    exit 1
fi

if [ ! -f "$n22File" ]; then
    echo "File $n22File not found!"
    exit 1
fi

if [ ! -f "$dspFile" ]; then
    echo "File $dspFile not found!"
    exit 1
fi

echo "Please wait for a moment"

tempFile=$(mktemp)



./img.sh "$d25fFile" "$d25fBinVersion" "$n22File" "$n22BinVersion" "$dspFile" "$dspBinVersion" "$d25fBinType" "$n22BinType" "$dspBinType"> "$tempFile" #create img block


# ./img.sh "$d25fFile" "$d25fBinVersion" "$n22File" "$n22BinVersion" "$wifiFile" "1" "$d25fBinType" > "$tempFile" #create img block

#create bin_info
#arg 0:bin_file 1:start addr 2:featureMap
./bin_info.sh "$d25fFile" "0x13040" "0" >> "$tempFile" #bin

./bin_info.sh "$n22File" "0x50020000" "0" >> "$tempFile" #bin

./bin_info.sh "$dspFile" "0x200040" "0" >> "$tempFile" #bin

cat "$tempFile" > "$otaFile"

rm -f "$tempFile"

echo "ota bin create now"