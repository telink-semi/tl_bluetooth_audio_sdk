# HCI层

## 开发工作

ble_host_hci_send_acl_data 函数优化：
    编写设计文档
    支持多链路共享，即同一时间可以发送多个ACL数据包。
    ACL数据包的拆包机制。
    ACL数据包的响应机制。
    异步发送方式。

HCI event 的分发机制：
    编写设计文档
    具体的分发机制在GAP层实现
    HCI层只是将event分类，上报给GAP层。由GAP层处理。
    event：ACL connect、CIS、BIS、PA等分类。

HCI receive 函数优化(ble_host_hci_rx_packet_parse)
    编写设计文档
    HCI event、ACL data、ISO data的解析。

More：
