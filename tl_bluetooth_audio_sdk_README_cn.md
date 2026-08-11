# tl\_bluetooth\_audio\_sdk README

* [English](./tl_bluetooth_audio_sdk_README_en.md)

# SDK 介绍

tl\_bluetooth\_audio\_sdk 是一个面向蓝牙音频产品的软件开发平台，基于泰凌 TL322x、TL751x、TL721x 和 TLSR952x 等系列 SoC，帮助开发者快速完成产品开发。

SDK 提供完整的软件框架，包括 BLE、BT、TPSLL（Telink Proprietary Synchronous Link Layer, 泰凌专有同步链路层）协议栈、硬件驱动、音频框架、系统服务及示例工程，为产品从原型开发到量产部署的完整开发流程提供支持。

tl\_bluetooth\_audio\_sdk 适用于头戴耳机、TWS 耳机、对讲设备、录音外设、智能头盔、蓝牙音频发射设备以及广播音频应用等场景。

**核心能力**

|功能模块                |核心能力                                                               |
|-----------------------|-----------------------------------------------------------------------|
|协议栈                  |BLE 协议栈、BT 协议栈、TPSLL 协议栈、多模共存                            |
|外设驱动                |GPIO、UART、SPI、I2C、USB、Flash、Timer、I2S 等                         |
|系统服务                |RTOS 和裸机模式、硬件抽象层（HAL）、独立音频调度管理器、低功耗模式、Bootloader 及无线 OTA 升级、FatFS 文件系统、UAC/U 盘模式、DSP 及双核通信、多麦克降噪、骨传导麦克风|
|音频编解码算法           |- SBC（Sub-band Coding）：子带编码 <br> - mSBC（Modified Sub-band Coding）：改进的子带编码 <br> - CVSD（Continuous Variable Slope Delta Modulation）：连续可变斜率增量调制 <br> - AAC（Advanced Audio Coding）：高级音频编码 <br> - LC3（Low Complexity Communication Codec）：低复杂度通信编解码器 <br> - LC3p（Low Complexity Communication Codec Plus）：低复杂度通信编解码器增强版 <br> - Opus（Opus Interactive Audio Codec）：交互式音频编解码器|
|音频性能算法             |- EQ（Equalizer）：均衡器 <br> - BBF（Blind Beamforming）：盲源波束成形 <br> - AEC（Acoustic Echo Cancellation）：回声消除 <br> - ANS（Automatic Noise Suppression）：噪声抑制 <br> - NN_NS（NN-based Noise Suppression）：神经网络降噪 <br> - AGC（Automatic Gain Control）：自动增益控制 <br> - DRC（Dynamic Range Control）：动态范围控制 <br> - VAD（Voice Activity Detection）：语音活动检测 <br> - PLC（Packet Loss Concealment）：丢包补偿|
|工具                     |日志调试                                                              |


**典型应用**

|产品类别                |典型产品与方案                                                         |
|-----------------------|-----------------------------------------------------------------------|
|蓝牙耳机类              |头戴式蓝牙耳机、TWS 真无线耳机、BT/TPSLL 头戴式及 TWS 双模混音在线耳机、骑行头盔耳机|
|蓝牙音箱类              |传统蓝牙音箱、LE Audio 蓝牙音箱、传统蓝牙音源转 LE Audio 广播|
|Dongle 类               |UAC/Line-in 音源转发至传统蓝牙或 LE Audio 耳机|
|录音设备                 |AI 外置录音声卡|
|LE Audio 音频方案        |1. BIS 广播源：支持 USB 音频采集，通过 BIS 实现一对多音频广播； <br> 2. CIS 单播音频设备：基于 CIS 实现点对点低功耗音频传输|


**支持信息**

支持的芯片型号、开发板、开发平台及 SDK 版本和下载方式，请参考 [Release Notes](./doc/tl_bluetooth_audio_sdk_Release_Note.md)。

# 相关文档和资源

|文档                   |说明                            |链接                                   |
|-----------------------|--------------------------------|--------------------------------------|
|快速入门 (Get Started)  |开发环境配置、SDK获取及快速上手方法|[Get Started](https://doc.telink-semi.cn/doc/zh/software/res/sdk/bt_audio/tl_bluetooth_audio_sdk_Get_Started_cn/)|
|发布说明 (Release Notes)|支持平台、版本说明及详细变化       |[Release Notes](./doc/tl_bluetooth_audio_sdk_Release_Note.md)|


|资源                    |说明                   |链接                                           |
|-----------------------|-----------------------|-----------------------------------------------|
|Telink 官方论坛         |技术交流与支持          |[论坛 - Telink](https://forum.telink-semi.cn/)|
|Telink 官方网站         |产品与文档中心          |[Telink \| Chips for a Smarter IoT](https://www.telink-semi.cn/)|
|GitHub / Gitee         |SDK 源码仓库            |[GitHub](https://github.com/telink-semi/tl_bluetooth_audio_sdk/) / [Gitee](https://gitee.com/telink-semi/tl_bluetooth_audio_sdk/)|

# 许可证

本项目采用以下许可证：

**Apache License, Version 2\.0**

Licensed under the Apache License, Version 2\.0 (the "License");

You may not use this file except in compliance with the License.

You may obtain a copy of the License at:

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

See the License for the specific language governing permissions and limitations under the License.

