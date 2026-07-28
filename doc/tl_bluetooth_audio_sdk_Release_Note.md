## V6.1.0.0(PR)

## Version

| SDK Version  : tl_bluetooth_audio_sdk V6.1.0.0 | Chip Version | Hardware EVK Version | Platform Version | Toolchain Version |
|------------------------------------------|--------------|----------------------|------------------|-------------------|
| TLSR952X(B92)                            | A3\A4        | C1T266A20_V1.3       | tl_platform_sdk V3.11.0 | TL32 ELF MCULIB V5F GCC12.2 (IDE: TelinkIoTStudio) |
| TL721X                                   | A2\A3        | C1T315A20_V1_2<br>C1TXA104_V1_1 | tl_platform_sdk V3.11.0 | TL32 ELF MCULIB V5F GCC12.2 (IDE: TelinkIoTStudio) |
| TL751X                                   | A1           | C1T368A20_V1_1<br>C1T368A20_V1_2 | tl_platform_sdk V3.11.0 | D25F: TL32 ELF MCULIB V5F GCC12.2 (IDE: TelinkIoTStudio)<br>N22: TL32 ELF MCULIB V5 GCC12.2 (IDE: TelinkIoTStudio) |
| TL322X                                   | A1           | C1T382A20_V1.2       | tl_platform_sdk V3.11.0 | TL32 ELF MCULIB V5F GCC12.2 (IDE: TelinkIoTStudio) |




## BREAKING CHANGES
- N/A

## Features
- Recording card reference design (TL721X platform, TL751X platform)
  - Supports multi-mic BBF, NN noise reduction, and AGC for high-quality recording
  - Supports real-time voice Opus encoding and upload to mobile phone via BLE
  - Supports offline voice data storage via local file system
  - BLE/WiFi shared antenna multiplexing for uploading offline-stored voice data in different scenarios
  - Supports BLE connection maintenance / broadcasting in low-power suspend mode

- BT Interphone reference design (TL751X platform)
  - Supports BT/BLE dual-mode connection, including BT music streaming (A2DP) and phone call (HFP)
  - Supports NN_NS noise suppression, AGC (Automatic Gain Control), and audio mixing algorithms
  - Supports BT dual connection (simultaneous connection to two BT devices)
  - Supports multi-scenario audio management, including:
    - Mobile phone music and phone call
    - Bluetooth headset front/rear seat intercom
    - Mesh audio
    - Sports camera audio recording
    - Car stereo music and navigation
  - Supports BLE data transmission and OTA (Over-The-Air) firmware upgrade
- Flash protection functionality has been activated for all reference designs.

## Performance Improvements
- bttpsll_tws reference design (TL751X platform)  
  - Optimized TPSLL audio latency: ultra-low latency mode at 17ms+, dual-mode online at 24ms+;
  - Improved binaural master-slave switching across various scenarios and fixed stability issues;
  - Enhanced range performance in TPSLL ultra-low latency mode;
  - Improved stability and compatibility with certain devices;

- bttpsll_headset reference design(TL751X platform)        
  - Support TPSLL ultra-low latency mode (17ms+);
  - Optimize TPSLL audio latency performance: dual-mode online 24ms+, BT music mixed with TPSLL music 64ms+, BT call mixed with TPSLL music 44ms+;
  - Optimize range performance in ultra-low latency mode;
  - Improve stability and compatibility with certain devices.
                                                                       
- tpsll_audio_dongle reference design (TL721X platform) 
  - Optimized the Dongle audio path and latency performance in bttpsll_tws/headset project;


- btble_headset  reference design (TLSR952X platform, TL751X platform)   
  - Add CTKD (Cross-Transport Key Derivation) function (TLSR952X platform only)   
                                                                                     
- btble_audio_source reference design(TLSR952X platform, TL751X platform)  
  - Optimize stability and compatibility issues. 
                                                                                   
- btble_headset_example reference design(TLSR952X platform, TL751X platform, TL721X platform, TL322X platform)                                                                                         
  - Optimize stability and compatibility issues.

## Bug Fixes
- bttpsll_tws reference design (TL751X platform)  
  - Resolved occasional stuttering issues during voice assistant or BT call scenarios;


- tpsll_audio_dongle reference design (TL721X platform) 
  - Fixed several known issues and improved overall stability;

- btble_headset  reference design (TLSR952X platform, TL751X platform)   
  - Fixed some known issues and optimized connection and coexistence stability
                                                                                     


## Royalty fee for certain Audio Codec
This SDK may include options for multiple audio codecs, it should be noted that use of certain Codecs may incur Royalty fees. It is the end product manufacturer's responsibility to sign license agreement with the license onwers and pay royalty fees. Telink as an IC provider cannot cover these charges.
* Use of LC3+ codec: If you choose to use LC3+ codec, please contact Fraunhofer/Ericsson (Fraunhofer IIS: lc3-licensing@iis.fraunhofer.de and Ericsson: lc3.licensing@ericsson.com) for proper license agreement and royalty fee information. A flat fee is charged by these License owners for product incorporating LC3+ codec. The royalty fee is open and transparent and charged per device (e.g. Headset, TV ,Box, …).
* Use of LC3 codec: LC3 usage is only free for product qualified as a Bluetooth product by Bluetooth SIG. If your product is not Bluetooth qualified and you choose to use LC3 codec, please contact Fraunhofer/Ericsson (Fraunhofer IIS: lc3-licensing@iis.fraunhofer.de and Ericsson: lc3.licensing@ericsson.com) for proper license agreement and royalty fee information. A flat fee is charged by these License owners for non-Bluetooth product incorporating LC3 codec. The royalty fee is open and transparent and charged per device (e.g. Headset, TV ,Box, …).



## CodeSize

| project               | TLSR952X_Flash_bin_size(KB) | TLSR952X_IRAM_Size(KB) | TLSR952X_DRAM_Size(KB) | TL751X_Flash_bin_size(KB) | TL751X_IRAM_Size(KB) | TL751X_DRAM_Size(KB) | TL721X_Flash_bin_size(KB) | TL721X_IRAM_Size(KB) | TL721X_DRAM_Size(KB) | TL322X_Flash_bin_size(KB) | TL322X_IRAM_Size(KB) | TL322X_DRAM_Size(KB) |
| --------------------- | ----------------------------- | ------------------------ | ------------------------ | ---------------------------- | ----------------------- | ----------------------- | ---------------------------- | ----------------------- | ----------------------- | ---------------------------- | ----------------------- | ----------------------- |
| btble_headset         | 961                           | 209                      | 188                      | 648                          | 162                     | 162                     | 275                          | 255                     | 51                      | \                            | \                       | \                       |
| btble_audio_source    | 937                           | 219                      | 215                      | 590                          | 226                     | 185                     | 298                          | 274                     | 67                      | \                            | \                       | \                       |
| btble_a2dp_to_bis     | \                             | \                        | \                        | 447                          | 95                      | 152                     | 318                          | 286                     | 70                      | \                            | \                       | \                       |
| bttpsll_headset       | \                             | \                        | \                        | 581                          | 188                     | 232                     | 223                          | 209                     | 48                      | \                            | \                       | \                       |
| bttpsll_tws           | \                             | \                        | \                        | 599                          | 205                     | 280                     | 240                          | 229                     | 51                      | \                            | \                       | \                       |
| tpsll_dongle          | \                             | \                        | \                        | \                            | \                       | \                       | \                            | \                       | \                       | 283                          | 122                     | 235                     |
| bluetooth_controller   | 536                           | 239                      | 85                       | \                            | \                       | \                       | \                            | \                       | \                       | \                            | \                       | \                       |
| bt_interphone          | \                             | \                        | \                        | 749                          | 129                     | 229                     | 289                          | 269                     | 53                      | \                            | \                       | \                       |
| recording_card         | \                             | \                        | \                        | 531                          | 238                     | 258                     | 229                          | 174                     | 67                      | 774                          | 237                     | 241                     |

## 版本

| SDK Version: tl_bluetooth_audio_sdk V6.1.0.0 | Chip Version | Hardware EVK Version | Platform Version | Toolchain Version |
|------------------------------------------|--------------|----------------------|------------------|-------------------|
| TLSR952X(B92)                            | A3\A4        | C1T266A20_V1.3       | tl_platform_sdk V3.11.0 | TL32 ELF MCULIB V5F GCC12.2 (IDE: TelinkIoTStudio) |
| TL721X                                   | A2\A3        | C1T315A20_V1_2<br>C1TXA104_V1_1 | tl_platform_sdk V3.11.0 | TL32 ELF MCULIB V5F GCC12.2 (IDE: TelinkIoTStudio) |
| TL751X                                   | A1           | C1T368A20_V1_1<br>C1T368A20_V1_2 | tl_platform_sdk V3.11.0 | D25F: TL32 ELF MCULIB V5F GCC12.2 (IDE: TelinkIoTStudio)<br>N22: TL32 ELF MCULIB V5 GCC12.2 (IDE: TelinkIoTStudio) |
| TL322X                                   | A1           | C1T382A20_V1.2       | tl_platform_sdk V3.11.0 | TL32 ELF MCULIB V5F GCC12.2 (IDE: TelinkIoTStudio) |




## BREAKING CHANGES
- N/A


## Features
- Recording card参考设计 (TL721X platform, TL751X platform)
  - 支持多麦BBF、NN降噪、AGC高质量录音
  - 支持通过BLE将实时语音opus编码后上传手机
  - 支持本地文件系统离线保存语音数据
  - 支持BLE/WIFI共享天线分场景复用上传离线保存的语音数据
  - 支持suspend低功耗下BLE保连接/广播功能

- BT_Interphone参考设计（TL751X 平台）
  - 支持 BT/BLE 双模连接，包括 BT 音乐（A2DP）及通话（HFP）
  - 支持 NN_NS 降噪、AGC（自动增益控制）、混音算法
  - 支持 BT 双连接（同时连接两台 BT 设备）
  - 支持多场景音频管控，包括：
    - 手机音乐通话
    - 蓝牙耳机前后座对讲
    - Mesh 音频
    - 运动相机音频录制
    - 车机音乐及导航
  - 支持通过 BLE 传输数据及 OTA 固件升级

- 所有参考设计flash保护功能已生效

## Performance Improvements
- Bttpsll_tws参考设计（TL751X platform）
  - 优化TPSLL音频延时表现，超低延时模式17ms+，双模在线模式24ms+
  - 优化各场景下双耳主从切换，修复稳定性问题
  - 优化超低延时模式下距离表现
  - 优化部分稳定性以及兼容性问题

- Bttpsll_headset参考设计（TL751X platform）
  - 支持TPSLL超低延时模式（17ms+）;
  - 优化TPSLL音频延时表现，双模在线模式24ms+，BT音乐与TPSLL音乐混音模式64ms+，BT电话与TPSLL音乐混音模式44ms+
  - 优化超低延时模式下距离表现；
  - 优化部分稳定性以及兼容性问题；

- tpsll_audio_dongle 参考设计 (TL721X platform)
  - 优化Dongle Audio Path 以及 bttpsll_tws/headset模式下延时表现


- btble_headset 参考设计 (TLSR952X platform, TL751X platform)  
  - 新增CTKD(跨传输密钥派生)功能(仅TLSR952X platform)   
 
- btble_audio_source 参考设计(TLSR952X platform, TL751X platform)
  - 优化部分稳定性以及兼容性问题                                                                 

- bluetooth_controller 参考设计 (TLSR952X Platform)  
  - 优化部分稳定性以及兼容性问题                                                                         


- le_example 参考设计 (TLSR952X platform, TL751X platform, TL721X platform, TL322X platform)
  - 优化部分稳定性以及兼容性问题


## Bug Fixes

- Bttpsll_tws参考设计（TL751X platform）
  - 修复在语音助手或BT通话场景概率卡顿问题


- tpsll_audio_dongle 参考设计 (TL721X platform)
  - 修复了部分已知问题，优化稳定性


- btble_headset 参考设计 (TLSR952X platform, TL751X platform)  
  - 修复了部分已知问题，优化连接和共存稳定性                                                                            



## Royalty fee for certain Audio Codec
此SDK可能包括多种音频编解码器的选项，需要注意的是，使用某些编解码器可能会产生版权费用。最终产品制造商有责任与许可所有者签订许可协议并支付版权费用。作为IC提供商的Telink无法承担这些费用。
* 使用LC3+编解码器：如果您选择使用LC3+编解码器，请联系Fraunhofer/Ericsson（Fraunhofer IIS：lc3-licensing@iis.fraunhofer.de 和 Ericsson：lc3.licensing@ericsson.com）以获得适当的许可协议和版权费用信息。这些许可所有者对采用LC3+编解码器的产品收取固定费用。版权费用是公开透明的，按设备收费（例如耳机、电视、盒子等）。
* 使用LC3编解码器：只有被蓝牙SIG认证为蓝牙产品的产品才可以免费使用LC3编解码器。如果您的产品未经蓝牙认证且选择使用LC3编解码器，请联系Fraunhofer/Ericsson（Fraunhofer IIS：lc3-licensing@iis.fraunhofer.de 和 Ericsson：lc3.licensing@ericsson.com）以获得适当的许可协议和版权费用信息。这些许可所有者对非蓝牙产品采用LC3编解码器收取固定费用。版权费用是公开透明的，按设备收费（例如耳机、电视、盒子等）。




## CodeSize










## V6.0.0.0(PR)

## Version

| SDK Version  : tl_bluetooth_audio_sdk V6.0.0.0 | Chip Version | Hardware EVK Version | Platform Version | Toolchain Version |
|------------------------------------------|--------------|----------------------|------------------|-------------------|
| TLSR952X(B92)                            | A3\A4        | C1T266A20_V1.3       | tl_platform_sdk V3.9.0 | TL32 ELF MCULIB V5F GCC12.2 (IDE: TelinkIoTStudio) |
| TL721X                                   | A2\A3        | C1T315A20_V1_2<br>C1TXA104_V1_1 | tl_platform_sdk V3.9.0 | TL32 ELF MCULIB V5F GCC12.2 (IDE: TelinkIoTStudio) |
| TL751X                                   | A1           | C1T368A20_V1_1<br>C1T368A20_V1_2 | tl_platform_sdk V3.9.0 | D25F: TL32 ELF MCULIB V5F GCC12.2 (IDE: TelinkIoTStudio)<br>N22: TL32 ELF MCULIB V5 GCC12.2 (IDE: TelinkIoTStudio) |
| TL322X                                   | A1           | C1T382A20_V1.2       | tl_platform_sdk V3.9.0 | TL32 ELF MCULIB V5F GCC12.2 (IDE: TelinkIoTStudio) |




## BREAKING CHANGES
- N/A


## Features
- **btpsll tws reference design (TL751X platform)**
  - Supports binaural teaming in both wired and wireless modes, including pairing, connection, and reconnection functions for TPSLL and BT links.
  - Supports BT music playback (SBC codec) and call mode (CVSD and MSBC codecs supported) in both single-ear and binaural modes.
  - Supports TPSLL music/call mode in both single-ear and binaural modes (Downlink: 48kHz sampling rate, 24-bit depth; Uplink: 16kHz sampling rate, 24-bit depth, LC3 Plus codec).
  - Supports enabling NN (Neural Network) noise reduction algorithm in BT call uplink and TPSLL call uplink modes.
  - Supports downlink mixing playback of BT music/phone calls and TPSLL music/calls in both single-ear and binaural modes, with mixing performed at 48kHz sampling rate and 24-bit depth.
  - Supports mixing playback and standalone playback of prompt tones (ADPCM format).
  - Supports seamless master-slave switching between the two earpieces in all scenarios.
  - Supports TPSLL ultra-low latency mode (18ms+) and regular low latency mode (28ms+).

- **btpsll headset reference design(TL751X platform)**
  - Supports BT and TPSLL link pairing and re-connection
  - Supports BT music playback (SBC decoding) and BT phone calls (CVSD and MSBC codecs)
  - Supports TPSLL Dongle music and call modes (48 kHz 24-bit downlink, 16 kHz 24-bit uplink, LC3 Plus encoding)
  - Supports TPSLL low-latency music mode (~29.4 ms)
  - Supports NN-based noise suppression for BT and TPSLL uplink voice during calls
  - Supports 48 kHz 24-bit mixing of BT music/call and TPSLL music

- **tpsll audio dongle reference design (TL721X platform)**
  - Supports (UAC) TPSLL Music, Call, and Mono Recording modes
  - Supports LC3 Plus codec format, with 48kHz sampling rate and 24-bit depth for the downlink, and 16kHz sampling rate and 24-bit depth for the uplink
  - Supports switching between compatibility as a btpsll headset dongle and a btpsll TWS dongle

- **btble_headset reference design (TLSR952X platform, TL751X platform)**
  - Supported BT and BLE link pairing and re-connection
  - Supported BT music playback (SBC decoding) and BT phone calls (CVSD and MSBC codecs)
  - Supported LE Audio music playback(48kHz) and phone call(32kHz), using LC3 codec
  - Supported BLE HID Keyboard mode
  - Supported LE Audio music/call mode, coexisting with one BT connection at the same time
  - Supported LE Audio music/call mode, and coexists with one BT music channel at the same time
  - Supported BT Multipoint function



- **btble_audio_source reference design (TLSR952X platform, TL751X platform)**
  - Supports forwarding UAC to BT/LE headphones as a dongle.
  - Bluetooth Classic Features
    - Supports pairing and automatic reconnection with Bluetooth audio TWS
    - Supports Bluetooth A2DP music, HFP voice call
    - Supports SBC encoding and MSBC/CVSD encoding/decoding
  - Bluetooth LE Audio Features
    - Supports pairing and connection with LE Audio TWS
    - Supports LC3 encoding/decoding
    - Supports LE Audio music/voice call
  - Bluetooth Classic and LE Coexistence
    - Supports Bluetooth Classic connection + LE Audio music/voice call
    - Supports LE connection + Bluetooth Classic music/ voice call

- **btble_a2dp_to_bis reference design (TL751X platform)**
  - Support BT pairing and reconnection
  - Support BT music playing (SBC decoder)
  - Support A2DP to BIS mode (BT music playing locally by A2DP profile, Auracast transmitter)
  - Support BIS sync mode (Filtering device and syncing automatically)
  - Support BIS sink mode (Auracast Receiver)
  - Support mode switching among A2DP to BIS/BIS sync/BIS sink
  - Support enable/disable BIS

- **bluetooth_controller reference design (TLSR952X Platform)**
  - Support BT/BLE scanning and pairing
  - Support BT/BLE music play/back scenarios
  - Support BT/BLE calling scenarios
  - Support coexistence of BT and BLE
  - Support standard HCI UART communication

- **le_example reference design(TLSR952X platform, TL751X platform, TL721X platform, TL322X platform)**
  - le_audio reference design
    - unicast_client reference design
      - Supports connection to a group of TWS/Headset
      - Supports call mode, music mode, MIC single uplink mode
      - Supports 8kHz/16kHz/24kHz/32kHz/48kHz sample rates
      - Supports switching between UAC and Codec input modes
    - unicast_server reference design
      - Supports TWS mode or Headset mode
      - Supports 8kHz/16kHz/24kHz/32kHz/48kHz sample rates
      - Supports call control profile and media control profile
    - broadcast_source reference design
      - Supports broadcast stereo audio
      - Supports switching between UAC and Codec input modes
      - Supports configuration of broadcast name, broadcast ID, encryption enable/disable, encryption key, etc
    - broadcast_sink reference design
      - Supports automatic synchronization with Telink's Broadcast Source Demo and playback
      - Supports synchronization with standard Broadcast Source through Broadcast Assistant device
      - Supports 8kHz/16kHz/24kHz/32kHz/48kHz sample rates
      - Supports mono/stereo audio synchronization
    - le_audio_device reference design
      - Supports complete Unicast Server functionality
      - Supports complete Broadcast Sink functionality
      - Supports switching between Unicast Server and Broadcast Sink functionality on mobile phones



## Royalty fee for certain Audio Codec
This SDK may include options for multiple audio codecs, it should be noted that use of certain Codecs may incur Royalty fees. It is the end product manufacturer's responsibility to sign license agreement with the license onwers and pay royalty fees. Telink as an IC provider cannot cover these charges.
* Use of LC3+ codec: If you choose to use LC3+ codec, please contact Fraunhofer/Ericsson (Fraunhofer IIS: lc3-licensing@iis.fraunhofer.de and Ericsson: lc3.licensing@ericsson.com) for proper license agreement and royalty fee information. A flat fee is charged by these License owners for product incorporating LC3+ codec. The royalty fee is open and transparent and charged per device (e.g. Headset, TV ,Box, …).
* Use of LC3 codec: LC3 usage is only free for product qualified as a Bluetooth product by Bluetooth SIG. If your product is not Bluetooth qualified and you choose to use LC3 codec, please contact Fraunhofer/Ericsson (Fraunhofer IIS: lc3-licensing@iis.fraunhofer.de and Ericsson: lc3.licensing@ericsson.com) for proper license agreement and royalty fee information. A flat fee is charged by these License owners for non-Bluetooth product incorporating LC3 codec. The royalty fee is open and transparent and charged per device (e.g. Headset, TV ,Box, …).



## CodeSize
| Project               | TLSR952X（single-core） | TLSR952X（single-core） | TLSR952X（single-core） | TL751X（D25F-core） | TL751X（D25F-core） | TL751X（D25F-core） | TL751X（N22-core） | TL751X（N22-core） | TL751X（N22-core） | TL721X（single-core） | TL721X（single-core） | TL721X（single-core） | TL322X（single-core） | TL322X（single-core） | TL322X（single-core） |
|-----------------------|-------------------------|-------------------------|-------------------------|---------------------|---------------------|---------------------|---------------------|---------------------|---------------------|-----------------------|-----------------------|-----------------------|-----------------------|-----------------------|-----------------------|
|                       | Flash bin size(KB)      | IRAM Size(KB)           | DRAM Size(KB)           | Flash bin size(KB)  | IRAM Size(KB)       | DRAM Size(KB)       | Flash bin size(KB)  | IRAM Size(KB)       | DRAM Size(KB)       | Flash bin size(KB)    | IRAM Size(KB)         | DRAM Size(KB)         | Flash bin size(KB)    | IRAM Size(KB)         | DRAM Size(KB)         |
| btble_headset         | 964                     | 221                     | 187                     | 642                 | 174                 | 100                 | 293                 | 262                 | 66                  | -                     | -                     | -                     | -                     | -                     | -                     |
| btble_audio_source    | 1002                    | 218                     | 209                     | 663                 | 90                  | 183                 | 294                 | 261                 | 69                  | -                     | -                     | -                     | -                     | -                     | -                     |
| btble_a2dp_to_bis     | -                       | -                       | -                       | 525                 | 89                  | 154                 | 333                 | 301                 | 74                  | -                     | -                     | -                     | -                     | -                     | -                     |
| bttpsll_headset       | -                       | -                       | -                       | 627                 | 206                 | 300                 | 211                 | 202                 | 46                  | -                     | -                     | -                     | -                     | -                     | -                     |
| bttpsll_tws           | -                       | -                       | -                       | 600                 | 205.7               | 280.5               | 240                 | 230.3               | 51.9                | -                     | -                     | -                     | -                     | -                     | -                     |
| tpsll_dongle          | -                       | -                       | -                       | -                   | -                   | -                   | -                   | -                   | -                   | 283                   | 122.3                 | 235                   | -                     | -                     | -                     |
| le_example            | -                       | -                       | -                       | -                   | -                   | -                   | -                   | -                   | -                   | -                     | -                     | -                     | -                     | -                     | -                     |
| bluetooth_controller  | 513                     | 236                     | 80                      | -                   | -                   | -                   | -                   | -                   | -                   | -                     | -                     | -                     | -                     | -                     | -                     |




## 版本
同上。



## BREAKING CHANGES
- N/A


## Features

- **btpsll_tws参考设计 (TL751X platform)**
  - 支持双耳有线方式和无线方式组队、TPSLL和BT链路的配对、连接、回连功能
  - 支持单耳/双耳模式下，BT音乐播放（SBC编解码格式）通话模式（支持CVSD和MSBC编解码格式）
  - 支持单/双耳模式下，TPSLL音乐/通话模式下（下行48KHz采样率 24bit位深，上行16KHz采样率 24bit位深 LC3 Plus编解码格式）
  - 支持BT通话上行和TPSLL通话上行模式下启用NN降噪算法
  - 在单耳或双耳模式下，支持BT音乐/电话与TPSLL音乐/通话场景下进行混音播放，并且在48kHz采样率 24bit位深参数下进行混音播放
  - 支持提示音（ADPCM格式）混音播放和单独播放功能
  - 支持各个场景下，双耳进行主从耳无缝切换功能
  - 支持TPSLL超低延时模式（18ms+）以及常规低延时模式(28ms+)

- **btpsll_headset参考设计 (TL751X platform)**
  - 支持BT和TPSLL链路配对、回连
  - 支持BT音乐播放（SBC解码）和BT电话（CVSD和MSBC编解码）模式
  - 支持TPSLL Dongle音乐和通话模式（下行48K 24bit，上行16K 24bit LC3 Plus编码格式）
  - 支持TPSLL音乐低延时模式（29.4ms左右）
  - 支持BT和TPSLL通话上行模式NN降噪算法
  - 支持BT音乐/电话和TPSLL音乐48k 24bit混音模式

- **tpsll_audio_dongle参考设计 (TL721X platform)**
  - 支持（UAC）TPSLL音乐、通话、单录音模式
  - 支持下行48kHz采样率 24bit位深，上行16K采样率 24bit位深 LC3 Plus编解码格式
  - 支持与兼容作为btpsll headset dongle和btpsll tws dongle之间切换

- **btble_headset参考设计 (TLSR952X platform, TL751X platform)**
  - 支持BT和BLE链路配对、回连
  - 支持BT音乐播放（SBC解码）和BT电话（CVSD和MSBC编解码）模式
  - 支持LE Audio音乐和通话模式(音乐48kHz，通话32kHz LC3编码格式)
  - 支持BLE HID Keyboard模式
  - 支持LE Audio耳机音乐/通话模式，同时与一路BT连接共存
  - 支持LE Audio耳机音乐/通话模式，同时与一路BT音乐共存
  - 支持BT Multipoint功能


- **btble_audio_source 参考设计 (TLSR952X platform, TL751X platform)**
  - 支持作为Dongle将UAC转发到BT/LE耳机
  - BT Classic主要功能
    - 支持对BT Audio耳机进行配对、回连
    - 支持SBC编码、MSBC/CVSD编解码
    - 支持BT A2DP音乐播放、BT HFP通话
  - LE Audio主要功能
    - 支持对LE Audio耳机进行配对、连接
    - 支持LC3编解码
    - 支持LE Audio音乐/通话
  - 支持BT+LE共存
    - 支持BT连接 + LE Audio音乐/通话
    - 支持LE连接 + BT Audio音乐/通话

- **btble_a2dp_to_bis 参考设计 (TL751X platform)**
  - 支持BT音乐播放（SBC解码）
  - 支持A2DP to BIS模式（A2DP本地播放，Auracast发送）
  - 支持BIS sync模式（过滤设备，自动同步）
  - 支持BIS sink模式（Auracast接收）
  - 支持A2DP to BIS/BIS sync/BIS sink三模式动态切换

- **bluetooth_controller 参考设计 (TLSR952X Platform)**
  - 支持BT/BLE扫描、配对
  - 支持BT/BLE音乐播放场景
  - 支持BT/BLE通话场景
  - 支持BT/BLE共存
  - 支持标准HCI uart通信


- **le_example参考设计 (TLSR952X platform, TL751X platform, TL721X platform, TL322X platform)**
  - le_audio参考设计
    - unicast_client参考设计
      - 支持连接一组TWS/Headset
      - 支持电话模式，音乐模式，MIC单上行模式
      - 支持8kHz/16kHz/24kHz/32kHz/48kHz采样率
      - 支持UAC和Codec两种输入模式切换
    - unicast_server参考设计
      - 支持TWS模式或者Headset模式
      - 支持8kHz/16kHz/24kHz/32kHz/48kHz采样率
      - 支持通话控制配置文件，媒体控制配置文件
    - broadcast_source参考设计
      - 支持广播立体声音频
      - 支持UAC和Codec两种输入模式切换
      - 支持设置广播名称、广播ID、加密与否、加密密钥等配置
    - broadcast_sink参考设计
      - 支持自动同步Telink的Broadcast Source Demo并播放
      - 支持通过Broadcast Assistant设备同步标准Broadcast Source
      - 支持8kHz/16kHz/24kHz/32kHz/48kHz采样率
      - 支持单声道/立体声音频同步
    - le_audio_device参考设计
      - 支持完整的Unicast Server功能
      - 支持完整的Broadcast Sink功能
      - 支持手机实现Unicast Server和Broadcast Sink功能的切换


## Royalty fee for certain Audio Codec
此SDK可能包括多种音频编解码器的选项，需要注意的是，使用某些编解码器可能会产生版权费用。最终产品制造商有责任与许可所有者签订许可协议并支付版权费用。作为IC提供商的Telink无法承担这些费用。
* 使用LC3+编解码器：如果您选择使用LC3+编解码器，请联系Fraunhofer/Ericsson（Fraunhofer IIS：lc3-licensing@iis.fraunhofer.de 和 Ericsson：lc3.licensing@ericsson.com）以获得适当的许可协议和版权费用信息。这些许可所有者对采用LC3+编解码器的产品收取固定费用。版权费用是公开透明的，按设备收费（例如耳机、电视、盒子等）。
* 使用LC3编解码器：只有被蓝牙SIG认证为蓝牙产品的产品才可以免费使用LC3编解码器。如果您的产品未经蓝牙认证且选择使用LC3编解码器，请联系Fraunhofer/Ericsson（Fraunhofer IIS：lc3-licensing@iis.fraunhofer.de 和 Ericsson：lc3.licensing@ericsson.com）以获得适当的许可协议和版权费用信息。这些许可所有者对非蓝牙产品采用LC3编解码器收取固定费用。版权费用是公开透明的，按设备收费（例如耳机、电视、盒子等）。




## CodeSize
同上。
