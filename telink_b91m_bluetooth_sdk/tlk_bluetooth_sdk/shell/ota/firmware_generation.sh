#!/bin/bash

d25fBinVersion="1"
n22BinVersion="1"
dspBinVersion="1"

d25fBinType="1"
n22BinType="2"
dspBinType="3"

need_boot=0
need_dsp=0
dual_core_mode=0

# Select CHIP TYPE
CHIP_NAMES=()
CHIP_DUAL_CORE=("TL322X" "TL751X" "TL752X" "TL753X")

TARGET_DIR="../../build"
while IFS= read -r dir; do
    dir_name=$(basename "$dir")
    CHIP_NAMES+=("$dir_name")
done < <(find "$TARGET_DIR" -mindepth 1 -maxdepth 1 -type d)

echo -e "\n===== Chip Type Select ====="
for index in "${!CHIP_NAMES[@]}"; do
    choice_num=$((index + 1))
    chip_name=${CHIP_NAMES[$index]}
    chip_options+="  $choice_num: $chip_name"
done
echo "$chip_options"
echo "============================"

MIN_CHOICE=1
MAX_CHOICE=${#CHIP_NAMES[@]}


while true; do
    read -p "Select Chip Type($MIN_CHOICE-$MAX_CHOICE):" chip_choice

    if [ -z "$chip_choice" ] || ! [[ "$chip_choice" =~ ^[0-9]+$ ]] || [ "$chip_choice" -lt "$MIN_CHOICE" ] || [ "$chip_choice" -gt "$MAX_CHOICE" ]; then
        echo -e "${RED}Error: Invalid input! Please enter($MIN_CHOICE-$MAX_CHOICE).${NC}"
        continue
    fi

    chip_index=$((chip_choice - 1))
    CHIP_TYPE=${CHIP_NAMES[$chip_index]}
    CHIP_FIRMWARE_PATH="../../build/$CHIP_TYPE"

    if printf '%s\n' "${CHIP_DUAL_CORE[@]}" | grep -qw "$CHIP_TYPE"; then
        dual_core_mode=1
    fi

    break
done



# Select Whether need bootloader
# None means no bootloader.
# If need, select bootloader file.
TARGET_DIR="../../../boot_loader/$CHIP_TYPE"
BOOT_BIN_FILES=("None")

if [ ! -d "$TARGET_DIR" ]; then
    echo "Error: Dir '$TARGET_DIR' not found!"
    exit 1
fi

while IFS= read -r file_path; do
    file_name=$(basename "$file_path")
    
    BOOT_BIN_FILES+=("$file_name")
done < <(find "$TARGET_DIR" -mindepth 1 -maxdepth 1 -type f -name "*.bin")

echo -e "\n===== Boot File Select ====="
for index in "${!BOOT_BIN_FILES[@]}"; do
    choice_num=$((index + 1))
    chip_name=${BOOT_BIN_FILES[$index]}
    boot_options+="  $choice_num: $chip_name"
done
echo "$boot_options"
echo "============================"

MIN_CHOICE=1
MAX_CHOICE=${#BOOT_BIN_FILES[@]}


while true; do
    read -p "Select Bootloader file (None means no bootloader.)($MIN_CHOICE-$MAX_CHOICE):" boot_choice

    if [ -z "$boot_choice" ] || ! [[ "$boot_choice" =~ ^[0-9]+$ ]] || [ "$boot_choice" -lt "$MIN_CHOICE" ] || [ "$boot_choice" -gt "$MAX_CHOICE" ]; then
        echo -e "${RED}Error: Invalid input! Please enter($MIN_CHOICE-$MAX_CHOICE).${NC}"
        continue
    fi

    boot_index=$((boot_choice - 1))
    BOOT_TYPE=${BOOT_BIN_FILES[$boot_index]}

    if [ "$BOOT_TYPE" = "None" ]; then
        need_boot=0
    else
        need_boot=1
        BOOT_FIRMWARE_PATH="../../../boot_loader/$CHIP_TYPE/$BOOT_TYPE"
    fi
    break
done


# Select project demo
PROJECT_NAMES=()

TARGET_DIR="../../vendor"
while IFS= read -r dir; do
    dir_name=$(basename "$dir")
    PROJECT_NAMES+=("$dir_name")
done < <(find "$TARGET_DIR" -mindepth 1 -maxdepth 1 -type d)

echo -e "\n===== Project Select ====="
for index in "${!PROJECT_NAMES[@]}"; do
    project_num=$((index + 1))
    project_name=${PROJECT_NAMES[$index]}
    project_options+="  $project_num: $project_name"
done
echo "$project_options"
echo "============================"

MIN_CHOICE=1
MAX_CHOICE=${#PROJECT_NAMES[@]}


while true; do
    read -p "Select Project($MIN_CHOICE-$MAX_CHOICE):" project_choice

    if [ -z "$project_choice" ] || ! [[ "$project_choice" =~ ^[0-9]+$ ]] || [ "$project_choice" -lt "$MIN_CHOICE" ] || [ "$project_choice" -gt "$MAX_CHOICE" ]; then
        echo -e "${RED}Error: Invalid input! Please enter($MIN_CHOICE-$MAX_CHOICE).${NC}"
        continue
    fi

    project_index=$((project_choice - 1))
    PROJECT_TYPE=${PROJECT_NAMES[$project_index]}
    MCU_FIRMWARE_PATH="../../build/$CHIP_TYPE/$PROJECT_TYPE/${PROJECT_TYPE}.bin"

    break
done

if [ ! -f "$MCU_FIRMWARE_PATH" ]; then
    echo "File $MCU_FIRMWARE_PATH not found!"
    exit 1
fi

if [ "$dual_core_mode" -eq 1 ]; then 
    N22_FIRMWARE_PATH="../../build/$CHIP_TYPE/controller/${PROJECT_TYPE^^}/${PROJECT_TYPE}_controller.bin"

    if [ ! -f "$N22_FIRMWARE_PATH" ]; then
        echo "File $N22_FIRMWARE_PATH not found!"
        exit 1
    fi
fi

if [ "$dual_core_mode" -eq 1 ]; then
    # Select Whether need DSP
    # None means no DSP.
    # If need, select DSP file.
    TARGET_DIR="../../../dsp/bin"
    DSP_BIN_FILES=("None")

    if [ ! -d "$TARGET_DIR" ]; then
        echo "Error: Dir '$TARGET_DIR' not found!"
        exit 1
    fi

    while IFS= read -r file_path; do
        file_name=$(basename "$file_path")
        
        DSP_BIN_FILES+=("$file_name")
    done < <(find "$TARGET_DIR" -mindepth 1 -maxdepth 1 -type f -name "*.bin")

    echo -e "\n===== DSP File Select ====="
    for index in "${!DSP_BIN_FILES[@]}"; do
        dsp_num=$((index + 1))
        dsp_name=${DSP_BIN_FILES[$index]}
        dsp_options+="  $dsp_num: $dsp_name"
    done
    echo "$dsp_options"
    echo "============================"

    MIN_CHOICE=1
    MAX_CHOICE=${#DSP_BIN_FILES[@]}


    while true; do
        read -p "Select DSP file (None means no DSP.)($MIN_CHOICE-$MAX_CHOICE):" dsp_choice

        if [ -z "$dsp_choice" ] || ! [[ "$dsp_choice" =~ ^[0-9]+$ ]] || [ "$dsp_choice" -lt "$MIN_CHOICE" ] || [ "$dsp_choice" -gt "$MAX_CHOICE" ]; then
            echo -e "${RED}Error: Invalid input! Please enter($MIN_CHOICE-$MAX_CHOICE).${NC}"
            continue
        fi

        dsp_index=$((dsp_choice - 1))
        DSP_TYPE=${DSP_BIN_FILES[$dsp_index]}

        if [ "$DSP_TYPE" = "None" ]; then
            need_dsp=0
        else
            need_dsp=1
            DSP_FIRMWARE_PATH="../../../dsp/bin/$DSP_TYPE"
        fi
        break
    done
fi



#cfg here
if [ "$need_boot" -eq 1 ]; then 
    otaFile="./${CHIP_TYPE}_firmware_0x00000.bin"
else
    otaFile="./${CHIP_TYPE}_firmware_0x12000.bin"
fi


totalFile="./all.bin"

echo "Please wait for a moment"

tempFile=$(mktemp)

if [ "$need_boot" -eq 1 ]; then 
    ./boot.sh "$BOOT_FIRMWARE_PATH" > "$tempFile"
    ./ota_info.sh >> "$tempFile"
fi

#create bin_info
#arg 0:bin_file 1:start addr 2:featureMap
if [ "$dual_core_mode" -eq 1 ] && [ "$need_dsp" -eq 1 ]; then 
    ./img.sh "$MCU_FIRMWARE_PATH" "$d25fBinVersion" "$N22_FIRMWARE_PATH" "$n22BinVersion" "$DSP_FIRMWARE_PATH" "$dspBinVersion" "$d25fBinType" "$n22BinType" "$dspBinType">> "$tempFile" #create img block
    ./bin_info.sh "$MCU_FIRMWARE_PATH" "0x13040" "0" >> "$tempFile" #bin
    ./bin_info.sh "$N22_FIRMWARE_PATH" "0x50020000" "0" >> "$tempFile" #bin
    ./bin_info.sh "$DSP_FIRMWARE_PATH" "0x200040" "0" >> "$tempFile" #bin
elif [ "$dual_core_mode" -eq 1 ]; then
    ./img.sh "$MCU_FIRMWARE_PATH" "$d25fBinVersion" "$N22_FIRMWARE_PATH" "$n22BinVersion" "0" "0" "$d25fBinType" "$n22BinType">> "$tempFile" #create img block
    ./bin_info.sh "$MCU_FIRMWARE_PATH" "0x13040" "0" >> "$tempFile" #bin
    ./bin_info.sh "$N22_FIRMWARE_PATH" "0x50020000" "0" >> "$tempFile" #bin
else
    ./img.sh "$MCU_FIRMWARE_PATH" "$d25fBinVersion"  "0" "0"   "0" "0"  "$d25fBinType" >> "$tempFile" #create img block
    ./bin_info.sh "$MCU_FIRMWARE_PATH" "0x13040" "0" >> "$tempFile" #bin
fi

cat "$tempFile" > "$otaFile"

rm -f "$tempFile"

echo "Firmware ${otaFile} create now"