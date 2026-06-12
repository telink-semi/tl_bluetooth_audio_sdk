# BLE OTA Example

目前支持的芯片

| MCU Series | Board Model                                                    |
|------------|----------------------------------------------------------------|
| TL721x     | C1TXA104_V1.1(CODEC1-V2)    <br>  C1T315A20  <br> C1T315A20_V2 |
| TLSR952x   | C1T266A20_V1.3   <br>                                          | 
| TL751x     | C1T368A20_V1_1   <br>                                          | 

SDK支持的OTA升级功能，该示例演示了如何通过BLE GATT Server实现OTA升级，并将OTA的功能集成到其他的BLE应用中。

通常GATT Server都是运行在GAP ACL Peripheral角色上，该示例也是基于此角色展示的，实际也可以基于GAP ACL Central角色实现OTA升级。

## OTA协议和流程

关于SDK的OTA协议和流程，参考官方Handbook文档。

## OTA 手机端Demo

OTA手机端的APP获取方式，联系FAE获取或者参考官方Handbook文档。

## BLE OTA Example 代码结构

用户需要使能SDK OTA功能，通过宏定义TLK_MW_OTA_ENABLE。
```c
#define TLK_MW_OTA_ENABLE       1
```

注册BLE Telink OTA v2 Service，并添加OTA characteristic(API:blc_svc_addOtaV2Group)。

```c
int INIT(APP_BLE_OTA_BLE)(void)
{
    blc_svc_addOtaV2Group();
    INIT(APP_BLE_ACL_PERIPHERAL)();

    tlk_printf("[APP] BLE OTA service initialized");
    return 0;
}
```

## BLE OTA Example 编译及烧写

支持OTA的固件编译方法与其他BLE应用一样，烧录的固件需要通过脚本生成。

脚本基于Python 3.7以上版本开发，开发者需要首先按照Python环境配置好，具体的步骤可以咨询AI。

[OTA GUI工具](../../../shell/ota/ota_gui.py) 需要支持Tkinter模块，Windows原始支持。

[OTA 脚本](../../../shell/ota/tlk_ota.py)

### OTA 脚本使用方法

根据实际情况修改脚本中的参数，执行脚本即可完成OTA升级。

文件不存在时，选择None即可。

```
python3 tlk_ota.py
// or
python tlk_ota.py
```

```python
    d25f_bin = tlk_bin_file_info("ota/bt_interphone.bin", 0x13040, type=0x01)
    n22_bin = tlk_bin_file_info("ota/bt_interphone_controller.bin", 0x50020000, type=2)
    dsp_bin = tlk_bin_file_info("ota/dsp_audio_sdk_v0.1.0.2_for_ram_boot.bin", 0x200040, type=3)
```

### OTA GUI使用方法

通过python的核心工具，可以打包成可秩序文件，具体方式咨询AI。

```
pyinstaller -F -w ota_gui.py
```

也可以每次运行脚本工作
```
python3 ota_gui.py
// or
python ota_gui.py
```

OTA的GUI界面如下，用户可以根据芯片不同，选择D25F/N22/DSP三种固件进行升级。

Address对应的数值不需要用户修改，Version是固件的版本，用来维护系统版本，默认是1。Boot文件可选，如果有Boot文件会生成Boot+OTA固件，可以直接从0地址烧录。

OTA生成的文件默认'tlk_ot_file.bin'，可以自行修改，Timestamp勾选后，生成的OTA文件名称会带时间戳，方便用户多版本Debug时使用。

点击'Generate OTA'按钮，生成OTA文件。

![OTA GUI](../img/ota_tool.png "OTA GUI")

比如下面图片，选择的D25F和N22固件，以及选择了Boot文件，默认会生成OTA文件和OTA+Boot文件。

文件名分别是'tlk_ota_file.bin'和'tlk_ota_file_with_boot.bin'。

![OTA GUI Selected Files](../img//ota_tool_select.png "OTA GUI Selected Files")

如果勾选了Timestamp，生成的文件名会带时间戳。

文件名分别是'tlk_ota_file_2025-12-05 14-34-55.bin'和'tlk_ota_file_with_boot_2025-12-05 14-34-55.bin'。

![OTA GUI Timestamp](../img//ota_tool_select_timestamp.png "OTA GUI Timestamp")
