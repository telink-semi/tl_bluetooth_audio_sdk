import time
import serial
import serial.tools.list_ports
from btsnoop_uart_base import BtSnoop

# CRC16-CCITT 标准实现
def crc16_ccitt(data: bytes, poly=0x1021, init=0xFFFF) -> int:
    crc = init
    for byte in data:
        crc ^= byte << 8
        for _ in range(8):
            crc = ((crc << 1) ^ poly) if (crc & 0x8000) else (crc << 1)
            crc &= 0xFFFF
    return crc

# 封包解析函数
# 字段	长度	说明
# 包头	2 字节	固定 0xAA55（低位在前，即 0x55 0xAA）
# 类型	1 字节	自定义类型标识
# 数据长度	2 字节	小端格式
# 数据区	N 字节	数据内容
# CRC16	2 字节	CRC16-CCITT (0x1021)，从 type 开始计算


BTSNOOP_TX_DATA = 0x01 # HCI TX packet
BTSNOOP_RX_DATA = 0x02 # HCI RX packet

BTSNOOP_MSG_LOG_BASE = 0x10
BTSNOOP_MSG_LOG = 0x11
BTSNOOP_MSG_WAR = 0x12
BTSNOOP_MSG_ERR = 0x13

BTSNOOP_REST = 0x80
def parse_and_log_packet(packet: bytes, btsnoop: BtSnoop):
    if len(packet) < 7:
        print("⚠️ Packet too short")
        return

    if packet[0] != 0x55 or packet[1] != 0xAA:
        print("⚠️ Invalid header")
        return

    pkt_type = packet[2]
    data_len = packet[3] | (packet[4] << 8)
    if len(packet) < 7 + data_len:
        print("⚠️ Incomplete packet")
        return

    data = packet[5:5 + data_len]
    recv_crc = packet[5 + data_len] | (packet[6 + data_len] << 8)
    calc_crc = crc16_ccitt(packet[2:5 + data_len])


    if calc_crc != recv_crc:
        print(f"❌ CRC mismatch: recv {recv_crc:04X}, calc {calc_crc:04X}")
        return

    # ✅ 特殊清空命令 80 00 00
    print(f"pkg_type={pkt_type:02X}, data_len={data_len}, data={data.hex()}, recv_crc={recv_crc:04X}, calc_crc={calc_crc:04X}")
    if pkt_type == BTSNOOP_REST and data_len == 0:
        print("🔄 Reset command received. Clearing BTSnoop log...")
        btsnoop.reset()  # 清空并重建文件头
        return

    if data_len < 1:
        print("⚠️ No HCI type in data")
        return
    hci_type = data[0]
    hci_payload = data[1:]

    if pkt_type == BTSNOOP_TX_DATA:
        btsnoop.log_packet(hci_type, 1, hci_payload, int(time.time() * 1_000_000))
        print(f"➡️ TX packet: HCI type={hci_type:02X}, len={len(hci_payload)}")
    elif pkt_type == BTSNOOP_RX_DATA:
        btsnoop.log_packet(hci_type, 0, hci_payload, int(time.time() * 1_000_000))
        print(f"⬅️ RX packet: HCI type={hci_type:02X}, len={len(hci_payload)}")
    elif pkt_type & BTSNOOP_MSG_LOG_BASE:
        btsnoop.log_msg_udp(pkt_type - BTSNOOP_MSG_LOG_BASE , data, int(time.time() * 1_000_000))
        print(f"⬅️ LOG packet: message type={pkt_type:02X}, len={len(data)}")
    else:
        print("⚠️ Unknown packet type")



# 串口读取与重组完整包
def read_from_serial(port="/dev/ttyUSB0", baudrate=115200):
    ser = serial.Serial(port, baudrate, timeout=0.1)
    buffer = bytearray()
    # btsnoop = BtSnoop("file", "serial_log", "output")
    btsnoop = BtSnoop("udp", "localhost", 24352)

    try:
        print("📡 Listening on", port)
        while True:
            data = ser.read(128)
            if data:
                buffer += data

                # 检查是否包含完整包
                while len(buffer) >= 7:
                    if buffer[0] != 0x55 or buffer[1] != 0xAA:
                        buffer.pop(0)
                        continue

                    if len(buffer) < 5:
                        break

                    data_len = buffer[3] | (buffer[4] << 8)
                    full_len = 7 + data_len
                    if len(buffer) < full_len:
                        break  # 不完整

                    pkt = buffer[:full_len]
                    buffer = buffer[full_len:]

                    parse_and_log_packet(pkt, btsnoop)
    except KeyboardInterrupt:
        print("\n🛑 Stopped by user")
    finally:
        ser.close()

def list_serial_ports():
    all_ports = list(serial.tools.list_ports.comports())
    # 过滤掉 description 为 'n/a' 的端口（如 /dev/ttyS0）
    ports = [p for p in all_ports if p.description.lower() != 'n/a']

    if not ports:
        print("未检测到任何有效串口设备。")
        return None

    print("可用串口列表：")
    for i, port in enumerate(ports):
        print(f"  [{i}] {port.device} ({port.description})")

    while True:
        try:
            index = int(input(f"请输入要打开的串口编号 (0 - {len(ports) - 1}): "))
            if 0 <= index < len(ports):
                return ports[index].device
            else:
                print("输入编号超出范围，请重试。")
        except ValueError:
            print("输入无效，请输入数字编号。")

# def read_from_serial(port_name, baudrate):
#     try:
#         with serial.Serial(port_name, baudrate, timeout=1) as ser:
#             print(f"已连接到 {port_name}，开始接收数据...")
#             while True:
#                 line = ser.readline()
#                 if line:
#                     print(line.decode(errors='ignore').strip())
# except Exception as e:
#     print(f"串口打开失败: {e}")


# 运行主函数
if __name__ == "__main__":
    selected_port = list_serial_ports()
    if selected_port:
        read_from_serial(selected_port, 3000000)
