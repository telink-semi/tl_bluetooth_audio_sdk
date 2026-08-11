# tl\_bluetooth\_audio\_sdk README

* [中文版](./tl_bluetooth_audio_sdk_README_cn.md)

# SDK Introduction

tl\_bluetooth\_audio\_sdk is a software development platform for Bluetooth audio products. Based on the Telink SoC series, including TL322x, TL751x, TL721x, and TLSR952x, it helps developers rapidly complete product development.

The SDK provides a complete software framework, including BLE, BT, TPSLL (Telink Proprietary Synchronous Link Layer) protocol stacks, hardware drivers, audio framework, system services, and reference projects, supporting the entire development process from prototype design to mass production deployment.

tl\_bluetooth\_audio\_sdk is suitable for applications such as over-ear headsets, TWS earbuds, intercom devices, recording peripherals, smart helmets, Bluetooth audio transmitters, and broadcast audio solutions.

**Core Capabilities**

|Functional Modules                |Core Capabilities                                                               |
|----------------------------|-----------------------------------------------------------------------|
|Protocol Stack                  |BLE protocol stack, BT protocol stack, TPSLL protocol stack, multi-mode coexistence                            |
|Peripheral Drivers                |GPIO, UART, SPI, I2C, USB, Flash, Timer, I2S, etc.                         |
|System Services                |RTOS and bare-metal modes, Hardware Abstraction Layer (HAL), standalone audio scheduling manager, low-power mode, bootloader and wireless OTA upgrade, FatFS file system, UAC/USB Mass Storage mode, DSP and dual-core communication, multi-microphone noise reduction, bone-conduction microphone support|
|Audio Codec Algorithms           |- SBC (Sub-band Coding) <br> - mSBC (Modified Sub-band Coding) <br> - CVSD (Continuous Variable Slope Delta Modulation) <br> - AAC (Advanced Audio Coding) <br> - LC3 (Low Complexity Communication Codec) <br> - LC3p (Low Complexity Communication Codec Plus) <br> - Opus (Opus Interactive Audio Codec)|
|Audio Processing Algorithms             |- EQ (Equalizer) <br> - BBF (Blind Beamforming) <br> - AEC (Acoustic Echo Cancellation) <br> - ANS (Automatic Noise Suppression) <br> - NN_NS (NN-based Noise Suppression) <br> - AGC (Automatic Gain Control) <br> - DRC (Dynamic Range Control) <br> - VAD (Voice Activity Detection) <br> - PLC (Packet Loss Concealment)|
|Tools                    |Logging                                                              |


**Typical Applications**

|Product Categories                |Typical Products and Solutions                                                         |
|-----------------------|-----------------------------------------------------------------------|
|Bluetooth Headsets              |Over-ear Bluetooth headsets, TWS earbuds, Bluetooth/TPSLL dual-mode over-ear and TWS audio-mixing headsets, cycling helmet headsets|
|Bluetooth Speakers              |Classic Bluetooth speakers, LE Audio Bluetooth speakers, Classic Bluetooth audio source to LE Audio broadcast|
|Dongle               |UAC/Line-in audio streaming to Classic Bluetooth or LE Audio headsets|
|Recording Devices                 |AI external sound card|
|LE Audio Solutions        |1\. BIS Broadcast Source: Supports USB audio capture and one-to-many audio broadcasting via BIS; <br> 2\. CIS Unicast Audio Device: Provides point-to-point low-power audio transmission based on CIS.|


**Support Information**

For supported chip models, development boards, development platforms, SDK versions, and download methods, refer to [Release Notes](./doc/tl_bluetooth_audio_sdk_Release_Note.md).

# Related Documents and Resources

|Document                   |Description                           |Link                                   |
|-----------------------|--------------------------------|--------------------------------------|
|Get Started |Development environment setup, SDK acquisition, and quick start guide|[Get Started](https://doc.telink-semi.cn/doc/en/software/res/sdk/bt_audio/tl_bluetooth_audio_sdk_Get_Started_en/)|
|Release Notes|Supported platforms, version information, and detailed change logs       |[Release Notes](./doc/tl_bluetooth_audio_sdk_Release_Note.md)|


|Resource                   |Description                   |Link                                           |
|-----------------------|-----------------------|-----------------------------------------------|
|Telink Official Forum         |Technical discussion and support          |[Forum - Telink](https://forum.telink-semi.cn/)|
|Telink Official Website         |Product and documentation center          |[Telink \| Chips for a Smarter IoT](https://www.telink-semi.com/)|
|GitHub / Gitee         |SDK source code repository            |[GitHub](https://github.com/telink-semi/tl_bluetooth_audio_sdk/) / [Gitee](https://gitee.com/telink-semi/tl_bluetooth_audio_sdk/)|

# License

This project is licensed under the following license:

**Apache License, Version 2\.0**

Licensed under the Apache License, Version 2\.0 (the "License");

You may not use this file except in compliance with the License.

You may obtain a copy of the License at:

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

See the License for the specific language governing permissions and limitations under the License.

