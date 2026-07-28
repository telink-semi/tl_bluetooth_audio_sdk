# tl_bluetooth_audio_sdk Get Started

<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">


## 概述

### 关于本文档

本文档旨在帮助您基于泰凌 TL322x、TL751x、TL721x 和 TLSR952x 等系列硬件，快速搭建音频应用的软件开发环境。通过一个简单的工程，您将了解如何使用 tl_bluetooth_audio_sdk（以下简称 SDK）来开发音频应用。

### 适用范围 

本文档适用的芯片型号、对应的开发板、开发平台及 SDK 版本详情，请参阅最新的 Release Notes。

!!! note "注意"

    - 打开 [Release Notes](https://gitee.com/telink-semi/tl_bluetooth_audio_sdk/blob/V6.1.0.0/doc/tl_bluetooth_audio_sdk_Release_Note.md) 页面后，通过左侧下拉列表，选择与您当前使用的 SDK 版本对应的 Release Notes 查看。

### 软硬件清单

在开始之前，请确保您已准备好以下软硬件：

|硬件                      |说明                                                            |
|---------------------------|-----------------------------------------------------------|
|开发板         |根据目标芯片系列选择对应的开发板（详见 Release Notes）                |
|泰凌烧录器（Programmer）|推荐使用 Programmer V1.0 - V3.0                                   |
|PC             |操作系统：Windows 10/11 / Linux                                            |
|USB 转串口模块  |用于 UART 日志输出与调试                                                            |
|USB数据线|用于连接烧录器与 PC |
|杜邦线   |用于连接开发板与烧录器、USB 转串口模块|
 
|软件                                |说明                               |
|------------------------------------|----------------------------------|
|IDE                                 |[Telink IoT Studio](#ide-安装与配置)                        |
|烧录工具                             |[BDT（Burning and Debugging Tool）](#安装烧录工具)      |
|上位机软件                           |[TelinkBluetoothTool](#上位机查看-log仅-windows-环境支持)（仅支持 Windows）  |
|SDK                                 |[tl_bluetooth_audio_sdk](#获取-sdk)|



## IDE 安装与配置 <a id="ide-安装与配置"></a>

Telink IoT Studio 提供了独立安装包，编译所需的工具链已集成在内，无需额外安装。同时，SDK 已默认完成工具链配置，无需手动选择版本。

请根据您的操作系统下载对应的安装包：

- Windows: [TelinkIoTStudio_V2025.2.zip](https://doc.telink-semi.cn/tools/telink_iot_studio/TelinkIoTStudio_V2025.2.zip)

- Linux: [Telink_IoT_Studio_2025.2_Installer.run](https://doc.telink-semi.cn/tools/telink_iot_studio/Telink_IoT_Studio_2025.2_Installer.run)

**安装步骤：**

Windows 环境：

（1）解压下载的 **.zip** 压缩包，运行 **TelinkIoTStudio_V2025.2.exe**，按照向导完成安装。

（2）安装完成后，必须运行 **TelinkIoTStudio Updater.exe** 以获取最新的组件更新。

Linux 环境： 

（1）针对运行环境赋予安装包执行权限。

（2）运行 **Telink_IoT_Studio_2025.2_Installer.run**，并按照终端提示完成安装。



## 获取 SDK <a id="获取-sdk"></a>

您可以在 [Gitee](https://gitee.com/telink-semi/tl_bluetooth_audio_sdk) 下载 SDK 包。

下载完成后，将 SDK 压缩包解压到自定义路径。

### SDK 目录结构速览

tl_bluetooth_audio_sdk 的目录结构及说明如下：

```c
tlk_bluetooth_sdk/  
├── boot/      # 提供工程编译及连接相关配置  
├── common/    # 提供一些通用的跨芯片系列的处理函数，如内存处理函数、字符串处理函数等  
├── core/      # 提供芯片系列配置、各模块 log 输出管理、SDK 版本记录等功能   
├── drivers/   # 提供 MCU 外设驱动（Clock、Flash、I2C、USB、GPIO、UART 等）
├── stack/     # 存放协议栈相关的头文件，源文件被编译到库文件里面，对于用户是不可见的  
├── tlkalg     # 提供加密、音频相关的算法
├── tlkapi/    # 提供通用的函数，如 flash 数据保存和 FIFO 相关操作的接口  
├── tlkapp/    # 封装上层应用接口（音频任务调度、Profile 状态处理、Key 和 LED 配置等）
├── tlklib/    # 存放 SDK 运行所必须的库文件和部分开源代码源码  
├── tlkmw/     # Middle Ware 中间层，集成管理各模块功能，提供简洁接口（BT 连接/查询/回连等）
├── tlksys/    # 存放 SDK 系统功能函数（系统任务、Timer、PM、双核通信、外设管理、HAL 层等）   
└── vendor/    # 用于存放不同应用工程相关的上层代码
```

### 导入 SDK 到 IDE

（1）打开 Telink IoT Studio，点击 **File** -> **Import**。

![在文件菜单中选择导入](pic/System/QuickStart/project_import_1.png){height=360px}

（2）在弹出的对话框中展开 **General** 目录，选择 **Existing Projects into Workspace**，点击 **Next**。

![选择 Existing Projects into Workspace](pic/System/QuickStart/project_import_2.png){height=360px}

（3）在 **Select root directory** 配置项右侧，点击 **Browse...** 按钮，选择已解压好的 **tl_bluetooth_audio_sdk** -> **Open**。

（4）在 **Projects** 部分勾选与目标工程对应的芯片系列 SDK，然后点击页面右下角的 **Finish** 按钮完成导入。

![导入 SDK](pic/System/QuickStart/project_import_3.png){height=300px}

以导入 TL751x 芯片系列 SDK 为例，导入后的目录结构如下图所示。

![SDK 目录结构](pic/System/QuickStart/project_import_4.png){height=300px}

### 配置

在编译工程前，根据目标工程及实际需求对功能模块、协议栈及 OTA 等进行配置。

每个工程的编译配置定义在相应的 `app_config.h` 文件中，例如 btble_headset 工程的编译配置定义在 `\tlk_bluetooth_sdk\vendor\btble_headset\app_config.h`。

!!! note "注意"

    - 请确保对应工程 `app_config.h` 文件中定义的编译配置与目标芯片系列的硬件外设配置一致。
    - SDK 的默认配置定义在 `\tlk_bluetooth_sdk\vendor\common\default_config.h` 文件中。

#### 功能模块配置

如果需要使用 key 和 LED 等功能，需要在 `app_config.h` 文件中使能相关配置：

```C
#define TLK_DEV_KEY_ENABLE       1
#define TLK_DEV_LED_ENABLE       (1 && !TLK_CFG_SUSPEND_ENABLE)
#define TLK_USB_UDB_ENABLE       (1 && !TLK_CFG_SUSPEND_ENABLE)
#define TLK_CFG_USB_ENABLE       (1 && !TLK_CFG_SUSPEND_ENABLE)
```

#### 协议栈配置

在使用 BT 或 BLE 协议栈时，需要在 `app_config.h` 文件中启用相关配置：

- 在使用 BT 协议栈时，必须启用 `TLK_STK_BT_ENABLE` 配置：

    ```C
    #define TLK_STK_BT_ENABLE         1
    ```

- 在使用 BLE 协议栈时，必须启用 `TLK_STK_BLE_ENABLE` 配置：

    ```C
    #define TLK_STK_BLE_ENABLE  1
    ```

其余协议功能配置可根据实际需求进行启用或禁用。

```C
#define TLKBTP_CFG_RFC_ENABLE     (1 && TLK_STK_BT_ENABLE)
#define TLKBTP_CFG_SPP_ENABLE	  (0 && TLKBTP_CFG_RFC_ENABLE)
#define TLKBTP_CFG_HFP_ENABLE     (1 && TLKBTP_CFG_RFC_ENABLE)
#define TLKBTP_CFG_HFPHF_ENABLE   (1 && TLKBTP_CFG_HFP_ENABLE)
#define TLKBTP_CFG_A2DP_ENABLE    (1 && TLK_STK_BT_ENABLE)
#define TLKBTP_CFG_A2DPSNK_ENABLE (1 && TLKBTP_CFG_A2DP_ENABLE)
```

#### OTA 功能配置

对于需要使用 OTA 功能的工程，需要在目标工程的 `app_config.h` 文件中启用 OTA 功能配置：

```C
#define TLK_MW_USER_CTRL_ENABLE       1
#define TLK_MW_OTA_ENABLE             (1 && TLK_MW_USER_CTRL_ENABLE)
```



## 编译与烧录

本章将引导您完成从选择工程、代码编译，到最终烧录并运行的完整开发闭环。

### 选择第一个工程

tl_bluetooth_audio_sdk 支持多个芯片系列，每个芯片系列包含多个工程，分别面向不同的功能与应用场景。同一工程在不同芯片系列上的功能保持一致。

各芯片系列支持的音频应用如下：

| 芯片系列        | 支持的音频应用                                       |说明|
| -------------- | --------------------------------------------------- | --------------------------------------------------- |
|TLSR952x |btble_headset、btble_audio_source、le_example、bluetooth_controller|支持 LE Audio 等音频应用|
|TL751x |btble_headset、btble_audio_source、a2dp_to_bis、bttpsll_headset、bttpsll_tws、le_example、recoarding_card|支持 BT/BLE 双模、TWS、低延时等音频应用|
|TL721x |tpsll_dongle、le_example、recoarding_card|支持泰凌专有同步链路层（TPSLL）等音频应用|
|TL322x|le_example|支持 LE Audio 等音频应用|

TL751x 芯片系列的 btble_headset 工程包含了 BT 和 BLE 的业务逻辑，是一个完整的 Headset 应用，可完整验证 BT 和 BLE 功能，以及 LED、按键等基本外设功能。

推荐选择 TL751x 系列的 btble_headset 作为您的第一个工程。

### 编译

#### 编译 TL751x 系列 btble_headset 工程 <a id="编译-tl751x-系列-btble_headset-工程"></a>

编译 TL751x 系列 btble_headset 工程，需要先编译 controller 工程，再编译目标工程。

具体的编译步骤如下：

（1）打开 Telink IoT Studio 后，打开已导入的 **TL751X_tlk_bluetooth_sdk (in TL751X)**。

（2）点击 **vendor** -> **controller** -> **controller_config.h**，设置 `CONTROLLER_MODE` 为工程 `BTBLE_HEADSET`。

![Controller 配置](pic/System/QuickStart/controller_setting.png)

（3）点击 **1** 的下拉框选择 **controller** 工程，然后点击编译按钮（小锤子）进行编译，等待编译完成。

![Controller 工程编译](pic/System/QuickStart/controller_build.png)

（4）点击 **1** 的下拉框选择目标工程 **btble_headset**，然后点击编译按钮（小锤子）进行编译。

![工程编译](pic/System/QuickStart/project_build_1.png){height=450px}

**编译输出：**

- 文件名：btble_headset&n22_controller_120.bin
- 路径：`tl_bluetooth_audio_sdk\telink_b91m_bluetooth_sdk\tlk_bluetooth_sdk\build\TL751X\btble_headset\`

!!! note "提示"

    - 完成上述步骤后，可参考[检查编译是否成功](#检查编译是否成功)章节进行验证。
    - 若您开发的工程是 TL751x 系列的 **bttpsll_tws**，在编译前需先配置 controller 的库文件。详见[附录 2：bttpsll_tws 工程库文件配置](#附录bttpsll_tws-工程库文件配置)。
    - 若您开发的工程是 TL751x 系列的其他工程，编译步骤与[编译 TL751x 系列 btble_headset 工程](#编译-tl751x-系列-btble_headset-工程)相同，只需将 **btble_headset** 替换为您的工程名即可。
    - 若您开发的是其他芯片系列的工程，请参考下方 [补充说明](#补充说明) 章节。

#### 补充说明 <a id="补充说明"></a>

根据芯片系列不同，编译方式分为双核编译和单核编译两种。

**双核编译（适用于 TL751x 系列）：**

- 编译流程：先编译 controller 工程，再编译目标工程。
- 自动合并：编译完成后，系统会自动执行固件合并脚本，将双核固件合并为一个 bin 文件，简化烧录流程。
- 操作步骤：参考[编译 TL751x 系列 btble_headset 工程](#编译-tl751x-系列-btble_headset-工程)。

**单核编译（适用于 TLSR952x、TL721x、TL322x 系列）：**

- 编译流程：只需编译目标工程，无需额外操作。
- 操作步骤：参考[编译 TL751x 系列 btble_headset 工程](#编译-tl751x-系列-btble_headset-工程)的**步骤（1）与（4）**。
- 输出文件名：`工程名.bin`
- 路径：`tl_bluetooth_audio_sdk\telink_b91m_bluetooth_sdk\tlk_bluetooth_sdk\build\芯片系列\工程名\`

### 检查编译是否成功 <a id="检查编译是否成功"></a>

检查是否编译成功的方法：

观察 Telink IoT Studio 控制台（**Console**）的输出，若无报错信息并显示 **Build Finished. 0 errors, 0 warnings.**，则表示编译成功。

![编译成功](pic/System/QuickStart/build_ok.png)

### 烧录

#### 连接硬件 <a id="连接硬件"></a>

在使用 BDT 工具前，请按照以下逻辑连接 PC、烧录器、开发板与 USB 转串口模块：

- PC <-> 烧录器：使用 USB 线连接。若烧录器上的绿色指示灯常亮，表明烧录器已被 PC 端成功识别。
- 烧录器 <-> 开发板：使用杜邦线连接：
    - 电源线：VCC <-> VIN；GND <-> GND
    - 数据线（单线 SWM 总线）：将烧录器的 SWM 引脚连接至开发板的 SWS 引脚。
- PC <-> USB 转串口模块：将 USB 转串口模块插入 PC 端的 USB 接口。
- 开发板 <-> USB 转串口模块：使用杜邦线连接：
    - USB 转串口模块的 TX <-> 开发板的 RX（PB7）
    - USB 转串口模块的 RX <-> 开发板的 TX（PC6）
    - USB 转串口模块的 GND <-> 开发板的 GND（任意一个 GND 引脚即可）
<!--
![硬件连接](pic/System/QuickStart/debug.png){height=450px}-->
<img src="../pic/System/QuickStart/debug.png" class="image" style="width:80%">

!!! note "注意"

    - 查看 TL751x 系列开发板 TX/RX 引脚定义的步骤：打开 Telink IoT Studio，在 Project Explorer 栏中，点击 **TL751X_tlk_bluetooth_sdk (in TL751X)** -> **vendor** -> **common** -> **boards**，打开当前使用开发板的配置文件，搜索 `#define TLKDEV_SERIAL1_TX_PIN` 与 `#define TLKDEV_SERIAL1_RX_PIN`。
    - 查看其他芯片系列开发板 TX/RX 引脚定义的步骤同上。

#### 安装烧录工具 <a id="安装烧录工具"></a>

BDT 用于将编译生成的固件烧录至目标芯片或开发板。

请根据您的操作系统下载对应的安装包：

- Windows: [BDT.zip](https://doc.telink-semi.cn/tools/bdt/Windows/BDT.zip)
- Linux: [BDT_Linux.zip](https://doc.telink-semi.cn/tools/bdt/Linux/BDT_Linux.zip)

**安装步骤（以 Windows 环境为例）：**
   
（1）将下载的工具包解压到自定义的路径。

（2）进入解压后的文件夹，双击运行 **Telink BDT.exe** 即可启动该工具。

![BDT Interface](pic/pic_common/BDT_Interface.png "BDT Interface"){height=300px}

#### 烧录固件

（1）参考[安装烧录工具](#安装烧录工具)章节，打开 BDT 工具，推荐选择 **Programmer V1.0 - V3.0**。

![选择烧录器版本](pic/pic_common/SelectBurningEVKVersion.png){height=350px}

（2）选择 **TL751X** 系列，如图所示：

![选择芯片系列](pic/System/QuickStart/bdt_1.png){height=400px}

（3）点击 **Device** -> **Connect**，选择设备进行连接，如果没有识别到设备，点击 **Refresh**，然后再次点击 **Connect**。

![BDT设备连接](pic/System/QuickStart/bdt_2.png)

（4）点击 **File** -> **Open**，选择编译产生的 bin 文件，直接打开。

![打开文件](pic/System/QuickStart/bdt_3.png)

（5）点击 **Download**，进行下载（如果开启了 flash 保护功能，烧录前请先点击 **Unlock** 解锁）。

![烧录](pic/System/QuickStart/bdt_unclock.png){height=350px}

显示 **Download OK!** 的 log 信息，则表示下载成功。

（6）下载完成之后，点击 **Reset**，重启设备。

!!! note "注意"

    - 当出现下载失败时，可以尝试在下载前依次点击 **Activate** 和 **Unlock** 按钮，然后再进行下载。

![烧录失败](pic/System/QuickStart/bdt_fail.png) 

### 运行与验证

烧录完成后，开发板上的 LED 指示灯闪烁，表示程序正常运行。此时 UART 端会有 log 输出，具体查看方法，请参考 [Log 打印](#log-打印)章节。

![运行与验证](pic/System/QuickStart/code_run.png) 



## 调试及 Log 打印

### 死机调试

死机情况下，可以通过 BDT 查看 PC 指针，然后结合 list 文件定位出现问题的地方。

在通过 PC 值定位问题时，请确保对应的 bin 文件与 list 文件位于同一目录下。

PC 值可如下图所示进行查看。

![查看 PC 值](pic/System/QuickStart/bdt_5.png)

### Log 打印 <a id="log-打印"></a>

SDK 支持基于 UART 调试的日志输出。

通过 UART 输出日志时，您需要确保目标工程 `app_config.h` 文件中的 `TLK_CFG_DBG_ENABLE` 和 `TLK_CFG_UART_TOOL_ENABLE`宏定义都已使能。

#### 上位机查看 Log（仅 Windows 环境支持） <a id="上位机查看-log仅-windows-环境支持"></a>

**上位机软件安装：**

TelinkBluetoothTool 是一款用于蓝牙设备的上位机软件，提供日志查看等功能。该软件已集成在 SDK 压缩包中，无需单独下载。

**系统要求：**

- 本软件仅适用于 Windows 操作系统。

**安装步骤：**

（1）参考[获取 SDK](#获取-sdk) 章节，下载并解压 SDK。

（2）进入 SDK 目录下的 `tool` 文件夹，解压 **.zip** 压缩包至自定义路径。

（3）进入解压后的目录，双击 **TelinkBluetoothTool.exe** 启动工具。

![TelinkBluetoothTool](pic/pic_common/telinkbluetoothtool.png)

**log 查看步骤：**

（1）打开 TelinkBluetoothTool 工具，设置波特率为 1,500,000 bps。

（2）点击顶部菜单栏的 **View** -> **Toggle Developer Tools** -> **Refresh**，在 **Port** 的下拉框中选择串口，即可在控制台中查看 log 日志。

![Toggle Developer Tools](pic/System/uartTool/uart_dev_tool.png){height=350px}

!!! note "注意"

    - 若控制台无日志输出，请尝试按下开发板上的复位按钮。按钮位置请参考[连接硬件](#连接硬件)章节的硬件连接示意图。 

日志等级分成四类，分别为：

- 详细（Verbose）：显示程序与开发板交互日志
- 信息（Info）：显示开发板输出的 log 日志
- 警告（Warnings）：显示程序遇到的警告
- 错误（Errors）：显示程序遇到的错误
    
您可以选择自己需要查看的日志等级，并通过过滤功能搜索需要的 log 信息。

![选择日志等级](pic/System/uartTool/uart_log_level.png)



## 附录 1：常见问题 

以下列出了 SDK 快速入门过程中可能遇到的常见问题及解决方法。

Q：为什么有些 log 打印不出来？

A：可能是某一时间输出的 log 太多，导致 UART 缓冲区溢出，从而造成部分 log 丢失。建议在调试阶段适当增大 UART 缓冲区大小。可通过修改 `tlkdb_cfg.h` 文件中的 `TLK_DEBUG_LOG_MEM_POOL_SIZE` 宏定义来实现。



## 附录 2：bttpsll_tws 工程库文件配置 <a id="附录bttpsll_tws-工程库文件配置"></a>

编译 TL751x 芯片系列的 bttpsll_tws 工程前，需先将 controller 的库文件配置为`lib751x_stack_controller_bttpsll_tws.a`。

具体步骤如下：

（1）在 Telink IoT Studio 中点击 **1** 的下拉框选择 **controller** 工程；

![点击Project](pic/System/QuickStart/controller_1.png){height=300px}

（2）点击 **Project**，在下拉框中选择 **Properties**；

![点击 Properties ](pic/System/QuickStart/lib4.png){height=300px}

（3）在弹出的 **Properties** 窗口中，点击左侧栏中的 **C/C++ Build** -> **Settings** -> **Tool Settings** -> **GNU RISC-V Cross C Linker** -> **Libraries**；

![点击 Libraries ](pic/System/QuickStart/lib3.png)

（4）选中 **751x_stack_controller_three_to_one** 库文件，点击页面右侧删除按钮将其移除。

![删除库文件](pic/System/QuickStart/lib2.png)

（5）点击右侧添加按钮，输入 `lib751x_stack_controller_bttpsll_tws.a`，点击 **OK** 按钮，添加此库文件。

（6）点击右下角 **Apply** -> **Apply and Close**，应用更改。

![添加库文件](pic/System/QuickStart/lib_add.png)


