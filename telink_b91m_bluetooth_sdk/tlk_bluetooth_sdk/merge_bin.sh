input_str="$1"

# file1="../d25f_test/d25f_test.bin"
controller_config_file="../../../vendor/controller/controller_config.h"
#controller_mode=$(awk '/#define CONTROLLER_MODE/ {print $3}' $controller_config_file)
controller_mode=$(grep '#define CONTROLLER_MODE' "$controller_config_file" | tr -s ' ' | tr -d '\r'| cut -d' ' -f3)
echo "$controller_mode"

file1="d25f.bin"
file2="../controller/controller.bin"
file3="n22.bin"

output_file=""
output_file_copy=""

prj_name=$input_str
if [ "$input_str" != "controller" ]; then
    if [ "$prj_name" = "bttpsll_headset" ]; then
        controller_mode="BTBLE_TPSLL_HEADSET"
    elif [ "$prj_name" = "bletpsll_tws" ]; then
        controller_mode="BLETPSLL_TWS"
    elif [ "$prj_name" = "bttpsll_tws" ]; then
        controller_mode="BTTPSLL_TWS"
    elif [ "$prj_name" = "btble_headset" ]; then
        controller_mode="BTBLE_HEADSET"
    elif [ "$prj_name" = "btble_a2dp_to_bis" ]; then
        controller_mode="BTBLE_A2DP_TO_BIS"
    elif [ "$prj_name" = "btble_audio_source" ]; then
        controller_mode="BTBLE_AUDIO_SOURCE"
    elif [ "$prj_name" = "bt_interphone" ]; then
        controller_mode="BT_INTERPHONE"
    elif [ "$prj_name" = "recording_card" ]; then
        controller_mode="RECORDING_CARD"
    elif [ "$prj_name" = "hearing_aid" ]; then
        controller_mode="HEARING_AID"
    elif [ "$prj_name" = "mesh_audio_dongle" ]; then
        controller_mode="MESH_AUDIO_DONGLE"
    elif [ "$prj_name" = "ble_example" ]; then
        controller_mode="BLE_CONTROLLER"
    elif [ "$prj_name" = "ruimo_btble_dongle" ]; then
        controller_mode="RUIMO_BTBLE_DONGLE"
    elif [ "$prj_name" = "GameSir_Xiaoji" ]; then
        controller_mode="GAMESIR_XIAOJI"
    fi
    file3="../controller/${controller_mode}/${prj_name}_controller.bin"
    output_file="./${prj_name}&n22_controller_120.bin"
    output_file_copy="./${prj_name}&n22_controller_121.bin"
else
    if [ "$controller_mode" = "BTBLE_TPSLL_HEADSET" ]; then
        prj_name="bttpsll_headset"
    elif [ "$controller_mode" = "BLETPSLL_TWS" ]; then
        prj_name="bletpsll_tws"
    elif [ "$controller_mode" = "BTTPSLL_TWS" ]; then
        prj_name="bttpsll_tws"
    elif [ "$controller_mode" = "BTBLE_HEADSET" ]; then
        prj_name="btble_headset"
    elif [ "$controller_mode" = "BTBLE_A2DP_TO_BIS" ]; then
        prj_name="btble_a2dp_to_bis"
    elif [ "$controller_mode" = "BTBLE_AUDIO_SOURCE" ]; then
        prj_name="btble_audio_source"
    elif [ "$controller_mode" = "BT_INTERPHONE" ]; then
        prj_name="bt_interphone"
    elif [ "$controller_mode" = "RECORDING_CARD" ]; then
        prj_name="recording_card"
    elif [ "$controller_mode" = "HEARING_AID" ]; then
        prj_name="hearing_aid"
    elif [ "$controller_mode" = "MESH_AUDIO_DONGLE" ]; then
        prj_name="mesh_audio_dongle"
    elif [ "$controller_mode" = "BLE_CONTROLLER" ]; then
        prj_name="ble_example"
    elif [ "$controller_mode" = "RUIMO_BTBLE_DONGLE" ]; then
        prj_name="ruimo_btble_dongle"
    elif [ "$controller_mode" = "GAMESIR_XIAOJI" ]; then
        prj_name="GameSir_Xiaoji"
    fi
    file3="../controller/${controller_mode}/${prj_name}_controller.bin"
    mkdir "../controller/${controller_mode}"
    if [ ! -f "$file2" ]; then
      echo "File $file2 not found!"
      exit 1
    fi
    cp "$file2" "$file3"

    output_file="./${controller_mode}/${prj_name}&n22_controller_120.bin"
    output_file_copy="./${controller_mode}/${prj_name}&n22_controller_121.bin"
fi

file1="../${prj_name}/${prj_name}.bin"




echo "note: Please check this file is end with LF(not CRLF) on windows "
echo "note: Please grant permission(chmod+x) to this script on Linux"
echo "merage n22_controller and d25f bins into one bin"

if [ ! -f "$file1" ]; then
    echo "File $file1 not found!"
    exit 1
fi


if [ ! -f "$file3" ]; then
    echo "File $file3 not found!"
    exit 1
fi

file1_size=$(stat -c%s "$file1")

offset=$((0x100000))

padding_size=$((offset - file1_size))

if [ "$file1_size" -ge "$offset" ]; then
    echo "Error: File1 size ($file1_size) exceeds the offset (0x100000)."
    exit 1
fi

dd if=/dev/zero bs=1 count="$padding_size" | tr '\0' '\377' > padding.bin

cat "$file1" padding.bin "$file3" > "$output_file"

cp "$output_file" "$output_file_copy"

rm padding.bin

echo "Merged file1 saved as $output_file"
echo "Merged file2 saved as $output_file_copy"