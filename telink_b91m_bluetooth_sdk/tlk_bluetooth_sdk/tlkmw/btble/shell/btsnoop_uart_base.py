import os
import time
import socket
from datetime import datetime, timezone
from construct import Struct, Const, Int32ub, Int64ub, Int8ub, Array

# =============================
# BTSnoop 格式定义
# =============================

BtSnoopHeader = Struct(
    "identify" / Const(b"btsnoop\0"),
    "version" / Const(0x01, Int32ub),
    "datalink" / Const(1002, Int32ub),  # HCI H4
)

BtSnoopPacketRecordH4 = Struct(
    "origin_len" / Int32ub,
    "include_len" / Int32ub,
    "packet_flag" / Int32ub,
    "cumulative_drops" / Int32ub,
    "timestamp_microseconds" / Int64ub,
    "h4_type" / Int8ub,
    "data" / Array(lambda ctx: ctx.origin_len - 1, Int8ub),
)

# =============================
# HCI 类型定义
# =============================

HCI_COMMAND_PACKET = 0x01
HCI_ACL_DATA_PACKET = 0x02
HCI_SCO_DATA_PACKET = 0x03
HCI_EVENT_PACKET = 0x04
HCI_ISO_DATA_PACKET = 0x05

# 对应 Ellisys 注入格式的 PacketType
InjectedHciPacketType_Command = 0x01
InjectedHciPacketType_AclFromHost = 0x02
InjectedHciPacketType_AclFromController = 0x82
InjectedHciPacketType_ScoFromHost = 0x03
InjectedHciPacketType_ScoFromController = 0x83
InjectedHciPacketType_Event = 0x84
InjectedHciPacketType_IsoFromHost = 0x05
InjectedHciPacketType_IsoFromonController = 0x85

# 对应 Ellisys 注入调试信息等级
MESSAGE_LEVEL_INF = 0x01
MESSAGE_LEVEL_WAR = 0x02
MESSAGE_LEVEL_ERR = 0x03
# =============================
# BtSnoop 封装类
# =============================

class BtSnoop:
    # mode: "udp" or "file"
    # args: (ip, port) or (name, path)
    def __init__(self, mode: str, *args, **kwargs):
        if mode != "udp" and mode != "file":
            raise ValueError("mode should be udp or file")

        if len(args) != 2:
            return

        self.mode = mode

        if self.mode == "udp":
            self.udp_target = (args[0], args[1])
            self.udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.file_path = None
        elif self.mode == "file":
            self.name = args[0]
            self.path = args[1]
            self.file_path = os.path.join(self.path, self.name + ".cfa")
            if not os.path.exists(self.path):
                os.makedirs(self.path)
            self._create_file()

        self.base_time = 62168256000000000  # 1970.01.01


    def _create_file(self):
        if self.file_path == None:
            return
        if os.path.exists(self.file_path):
            os.remove(self.file_path)
        with open(self.file_path, 'wb') as f:
            f.write(BtSnoopHeader.build({}))

    def reset(self):
        self._create_file()

    def _get_timestamp(self):
        # 每包假设间隔 10ms 递增
        self.base_time += 10_000
        return self.base_time

    def log_packet(self, packet_type: int, direction: int, packet_data: bytes, timestamp=None):
        if self.mode == "udp":
            self.log_packet_udp(packet_type,direction,packet_data,timestamp)
        else:
            self.log_packet_file(packet_type,direction,packet_data,timestamp)

    def log_packet_file(self, packet_type: int, direction: int, packet_data: bytes, timestamp=None):
        if timestamp is None:
            timestamp = self._get_timestamp()
        elif isinstance(timestamp, float):
            timestamp = int(timestamp * 1_000_000)

        packet_flag = 0
        if direction == 0:  # RX
            packet_flag |= 0x00
        else:              # TX
            packet_flag |= 0x01

        if packet_type in (HCI_COMMAND_PACKET, HCI_EVENT_PACKET):
            packet_flag |= 0x02

        full_packet = dict(
            origin_len=len(packet_data) + 1,
            include_len=len(packet_data) + 1,
            packet_flag=packet_flag,
            cumulative_drops=0,
            timestamp_microseconds=timestamp,
            h4_type=packet_type,
            data=list(packet_data),
        )

        with open(self.file_path, 'ab') as f:
            f.write(BtSnoopPacketRecordH4.build(full_packet))

    def log_packet_udp(self, packet_type: int, direction: int, packet_data: bytes, timestamp=None):
        if timestamp is None:
            timestamp = self._get_timestamp()
        elif isinstance(timestamp, float):
            timestamp = int(timestamp * 1_000_000)

        # ==== Ellisys Packet Format ==== #
        send_data = b''

        # Service ID & Version
        send_data += (0x0002).to_bytes(2, 'little')
        send_data += (0x01).to_bytes(1, 'little')

        # Timestamp (day-based)
        local_dt = datetime.now()
        dt = datetime.now(timezone.utc)
        # dt_day = datetime(dt.year, dt.month, dt.day)
        timestamp_ns = (dt.hour * 3600 + dt.minute * 60 + dt.second ) * 1000000000 + dt.microsecond * 1000
        # timestamp_ns = int((dt.timestamp() - dt_day.timestamp()) * 1_000_000_000)
        send_data += (0x02).to_bytes(1, 'little')
        send_data += local_dt.year.to_bytes(2, 'little')
        send_data += local_dt.month.to_bytes(1, 'little')
        send_data += local_dt.day.to_bytes(1, 'little')
        send_data += timestamp_ns.to_bytes(6, 'little')

        # Bitrate
        send_data += (0x80).to_bytes(1, 'little')
        send_data += (12000000).to_bytes(4, 'little')

        # HCI Type
        send_data += (0x81).to_bytes(1, 'little')
        if packet_type == HCI_COMMAND_PACKET:
            send_data += (InjectedHciPacketType_Command).to_bytes(1, 'little')
        elif packet_type == HCI_EVENT_PACKET:
            send_data += (InjectedHciPacketType_Event).to_bytes(1, 'little')
        elif packet_type == HCI_ACL_DATA_PACKET:
            send_data += (InjectedHciPacketType_AclFromHost if direction else InjectedHciPacketType_AclFromController).to_bytes(1, 'little')
        elif packet_type == HCI_SCO_DATA_PACKET:
            send_data += (InjectedHciPacketType_ScoFromHost if direction else InjectedHciPacketType_ScoFromController).to_bytes(1, 'little')
        elif packet_type == HCI_ISO_DATA_PACKET:
            send_data += (InjectedHciPacketType_IsoFromHost if direction else InjectedHciPacketType_IsoFromonController).to_bytes(1, 'little')
        else:
            send_data += (0x00).to_bytes(1, 'little')

        # HCI Data
        send_data += (0x82).to_bytes(1, 'little')
        send_data += packet_data

        self.udp_socket.sendto(send_data, self.udp_target)
        # print("send value is ", send_data);

    def log_msg_udp(self, level:int, packet_data: bytes, timestamp=None):
        if timestamp is None:
            timestamp = self._get_timestamp()
        elif isinstance(timestamp, float):
            timestamp = int(timestamp * 1_000_000)

        # ==== Ellisys Packet Format ==== #
        send_data = b''

        # Service ID & Version
        send_data += (0x0001).to_bytes(2, 'little')
        send_data += (0x01).to_bytes(1, 'little')

        # Timestamp (day-based)
        dt = datetime.now(timezone.utc)
        # print(dt.timestamp(), datetime.now(timezone.utc).timestamp())
        # dt_day = datetime(dt.year, dt.month, dt.day)
        timestamp_ns = (dt.hour * 3600 + dt.minute *60+dt.second ) * 1000000000 + dt.microsecond * 1000000
        # timestamp_ns = int((dt.timestamp() - dt_day.timestamp()) * 1_000_000_000)
        send_data += (0x02).to_bytes(1, 'little')
        send_data += dt.year.to_bytes(2, 'little')
        send_data += dt.month.to_bytes(1, 'little')
        send_data += dt.day.to_bytes(1, 'little')
        send_data += timestamp_ns.to_bytes(6, 'little')
        # obj level
        send_data += (0x80).to_bytes(1, 'little')
        send_data += (level).to_bytes(1, 'little')
        # obj text
        send_data += (0x06).to_bytes(1, 'little')
        send_data += packet_data
        # end obj text
        send_data += (0x00).to_bytes(1, 'little')

        self.udp_socket.sendto(send_data, self.udp_target)

def replay_btsnoop_file(input_file: str, output_btsnoop: BtSnoop):
    if not os.path.exists(input_file):
        print(f"❌ File not found: {input_file}")
        return

    with open(input_file, 'rb') as f:
        header = f.read(16)
        if header[:8] != b"btsnoop\x00":
            print("❌ Invalid BTSnoop header")
            return

        while True:
            try:
                record = BtSnoopPacketRecordH4.parse_stream(f)
                packet_type = record.h4_type
                direction = 0 if (record.packet_flag & 0x01) == 0 else 1
                timestamp = record.timestamp_microseconds
                data = bytes(record.data)
                output_btsnoop.log_msg_udp(MESSAGE_LEVEL_INF,"abc".encode(),timestamp)
                output_btsnoop.log_msg_udp(MESSAGE_LEVEL_WAR,"123".encode(),timestamp)
                output_btsnoop.log_msg_udp(MESSAGE_LEVEL_ERR,"456".encode(),timestamp)
                output_btsnoop.log_packet(packet_type, direction, data, timestamp)
                time.sleep(0.001)
            except EOFError:
                break
            except Exception as e:
                print(f"⚠️ Error parsing packet: {e}")
                break

    print("✅ Replay completed.")


# =============================
# 模块测试
# =============================

if __name__ == '__main__':
    new_btsnoop = BtSnoop("udp","localhost" ,24352)
    # btsnoop = BtSnoop("test", "output")
    new_btsnoop.log_packet(HCI_COMMAND_PACKET, 1, b"\x03\x0C\x00")  # 简单 HCI Reset 命令
    new_btsnoop.reset()  # 重置日志文件

    #test
    # input_path = "output/example.btsnoop"
    # output_path = "output/replayed"
    # # new_btsnoop = BtSnoop("file", "name_save", output_path)
    # new_btsnoop = BtSnoop("udp","localhost" ,24352)
    # replay_btsnoop_file(input_path, new_btsnoop)