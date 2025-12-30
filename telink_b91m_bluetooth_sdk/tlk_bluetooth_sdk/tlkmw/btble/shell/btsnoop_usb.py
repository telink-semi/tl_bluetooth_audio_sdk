'''
    Packet format: https://fte.com/webhelpii/hsu/Content/Technical_Information/BT_Snoop_File_Format.htm
    Packet format: https://github.com/boundary/wireshark/blob/master/wiretap/btsnoop.c
    Packet format: https://github.com/bluekitchen/btstack/blob/master/platform/windows/hci_dump_windows_fs.c
'''

import os
import time
import re

try:
    from construct import *
except ImportError:
    os.system("pip install construct")
    # print("Please install construct module first.")

from construct import *

BtSnoopHeader = Struct(
    "identify" / Const(b"btsnoop\0"),
    "version" / Const(0x01, Int32ub),
    "datalink" / Const(1002, Int32ub),
)

BtSnoopPacketRecord = Struct(
    "origin_len" / Int32ub,
    "include_len" / Int32ub,
    "packet_flag" / Int32ub,
    "cumulative_drops" / Int32ub,
    "timestamp_microseconds" / Int64ub,
    "data" / Array(this.origin_len, Byte),
)

BtSnoopPacketRecordH4 = Struct(
    "origin_len" / Int32ub,
    "include_len" / Int32ub,
    "packet_flag" / Int32ub,
    "cumulative_drops" / Int32ub,
    "timestamp_microseconds" / Int64ub,
    "h4_type" / Int8ub,
    "data" / Array(this.origin_len - 1, Byte),
)

HCI_COMMAND_PACKET = 0x01
HCI_ACL_DATA_PACKET = 0x02
HCI_SCO_DATA_PACKET = 0x03
HCI_EVENT_PACKET = 0x04
HCI_ISO_DATA_PACKET = 0x05

class BtSnoop(object):
    def create_file(self):
        if len(self.path) == 0:
            return
        if os.path.exists(self.file_path):
            os.remove(self.file_path)
        if os.path.exists("output\\test.bes"):
            os.remove("output\\test.bes")
        if os.path.exists("output\\test.frm"):
            os.remove("output\\test.frm")
        if os.path.exists("output\\test.fsc"):
            os.remove("output\\test.fsc")
        if not os.path.exists(self.path):
            os.mkdir(self.path)
        with open(self.file_path, 'wb') as f:
            f.write(BtSnoopHeader.build(None))

    def __init__(self, name, path):
        self.name = name
        self.path = path
        self.add_time = 62168256000000000 # 1970-01-01 00:00:00 base time.
        self.file_path = os.path.join(self.path, self.name + '.cfa')
        self.create_file()

    def get_timestamp(self, timestamp):
        self.add_time += 10 * 1000 # 10ms
        return self.add_time

    def log_packet(self, packet_type, in_value, packet, timestamp=time.time()):
        if len(self.path) == 0:
            return
        length = len(packet) + 1
        packet_flag = 0
        if in_value:
            packet_flag |= 0x01
        if packet_type == HCI_COMMAND_PACKET \
                or packet_type == HCI_EVENT_PACKET:
            packet_flag |= 0x02

        with open(self.file_path, 'ab') as f:
            f.write(BtSnoopPacketRecordH4.build(
                dict(
                    origin_len=length, 
                    include_len=length,
                    packet_flag=packet_flag,
                    cumulative_drops=0,
                    timestamp_microseconds=self.get_timestamp(timestamp),
                    h4_type=packet_type,
                    data=packet)
                )
            )

is_tx = 0
match_data = ""

def get_btsnoop_data(file_path):
    if not os.path.exists(file_path):
        return None

    def found_data(text):
        global is_tx
        global match_data
        text = text.strip()

        if "[BT_SNOOP] HCI TX Packet:" in text:
            is_tx = 1
            match = re.search(r'\[BT_SNOOP\] HCI TX Packet: ?([\da-fA-F\s]+)', text)
            if match:
                match_data += match.group(1).strip()
            return True

        if "[BT_SNOOP] HCI RX Packet:" in text:
            is_tx = 2
            match = re.search(r'\[BT_SNOOP\] HCI RX Packet: ?([\da-fA-F\s]+)', text)
            if match:
                match_data += match.group(1).strip()
            return True

        match = re.search(r'\[BT_SNOOP\] end of packet', text)
        if match and is_tx!= 0 or ":" in text:
            # print("end of packet is ", text)
            # print("is_tx is ", is_tx)
            # print("match_data is ", match_data)
            try:
                cmd_str = bytes.fromhex(match_data)
            except ValueError:
                print("error: invalid hex data")
                return True

            # print("cmd is ", cmd_str[0], type(cmd_str[0]), "\n\n")

            if is_tx == 1:
                if cmd_str[0] == HCI_COMMAND_PACKET:
                    btsnoop.log_packet(HCI_COMMAND_PACKET, 1, cmd_str[1:], time.time())
                elif cmd_str[0] == HCI_ACL_DATA_PACKET:
                    btsnoop.log_packet(HCI_ACL_DATA_PACKET, 1, cmd_str[1:], time.time())
                elif cmd_str[0] == HCI_ISO_DATA_PACKET:
                    btsnoop.log_packet(HCI_ISO_DATA_PACKET, 1, cmd_str[1:], time.time())
            elif is_tx == 2:
                if cmd_str[0] == HCI_EVENT_PACKET:
                    btsnoop.log_packet(HCI_EVENT_PACKET, 0, cmd_str[1:], time.time())
                if cmd_str[0] == HCI_ACL_DATA_PACKET:
                    btsnoop.log_packet(HCI_ACL_DATA_PACKET, 0, cmd_str[1:], time.time())
                elif cmd_str[0] == HCI_ISO_DATA_PACKET:
                    btsnoop.log_packet(HCI_ISO_DATA_PACKET, 0, cmd_str[1:], time.time())

            match_data = ""
            is_tx = 0
            # while(1):
            #     pass
            return True

        if is_tx != 0:
            match_data += " " + text.strip()

    with open(file_path, 'r') as File:
        for text in File.readlines():
            found_data(text)

# for test
if __name__ == '__main__':
    btsnoop = BtSnoop('test', 'output')
    btsnoop.create_file()
    # btsnoop.log_packet(HCI_ACL_DATA_PACKET, 0, acl_packet, current_time)
    # btsnoop.log_packet(HCI_ACL_DATA_PACKET, 1, acl_packet, current_time)
    get_btsnoop_data("btsnoop_test_log.txt")
