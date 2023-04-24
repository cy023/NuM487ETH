from scapy.all import *

frame = Ether(dst='66:66:66:66:66:66', src='66:66:66:66:66:66', type=0x1337) / Raw("Hello Ethernet! I'am x86_64 PC.")
sendp(frame, iface="Realtek PCIe GbE Family Controller")
