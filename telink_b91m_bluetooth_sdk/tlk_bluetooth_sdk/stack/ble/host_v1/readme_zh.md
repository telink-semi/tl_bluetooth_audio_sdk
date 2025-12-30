# Telink BLE Host V1

## 概述

暂无

## 目录结构

暂无

## 适配说明

### HCI layer

### L2CAP layer

## 开发规则

1. 遵循Telink代码规范。

2. 函数命名规则：
   - host_v1所有public函数，先以ble_host开头，后续有更好的命名，在全局替换。static函数，尽量遵循该规则。
   - 函数名全部小写，单词之间用下划线连接。
   - 私有函数和变量，必须要用static定义。
   - 全局变量，不允许extern后跨文件使用
   - 模块解耦。

3. Host层include，不能包含大的目录，只包含必要的头文件。

4. 头文件：
   #pragma once 替代 #ifndef #define #endif
   头文件中，只允许包含必要的头文件，不允许包含大目录。

## 开发思路

1. 错误码定义。

   参考与借鉴：https://mynewt.apache.org/latest/network/ble_hs/ble_hs_return_codes.html