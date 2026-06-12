#!/bin/bash
#cfg here
bootFile="../../../boot_loader/TL751X/boot_loader_rtc.bin"
d25fFile="../../build/TL751X/recording_card/recording_card.bin"
dspFile="../../../dsp/bin/dsp_rc_1.0.3.bin"
n22File="../../build/TL751X/controller/RECORDING_CARD/recording_card_controller.bin"
d25fBinVersion="1"
n22BinVersion="1"
dspBinVersion="1"

d25fBinType="1"
n22BinType="2"
dspBinType="3"
#cfg here

otaFile="./download_firmware.bin"
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

./boot.sh "$bootFile" > "$tempFile"

./ota_info.sh >> "$tempFile"

./img.sh "$d25fFile" "$d25fBinVersion" "$n22File" "$n22BinVersion" "$dspFile" "$dspBinVersion" "$d25fBinType" "$n22BinType" "$dspBinType" >> "$tempFile" #create img block

#create bin_info
#arg 0:bin_file 1:start addr 2:featureMap
./bin_info.sh "$d25fFile" "0x13040" "0" >> "$tempFile" #bin

./bin_info.sh "$n22File" "0x50020000" "0" >> "$tempFile" #bin

./bin_info.sh "$dspFile" "0x200040" "0" >> "$tempFile" #bin

cat "$tempFile" > "$otaFile"

rm -f "$tempFile"

echo "ota bin create now"
