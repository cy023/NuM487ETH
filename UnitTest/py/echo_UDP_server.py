import socket
import signal
import sys
import select

HOST = '127.0.0.1'
PORT = 7

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind((HOST, PORT))
print('===== Listening on', str((HOST, PORT)), '=====')


def signal_handler(sig, frame):
    print('KeyboardInterrupt')
    s.close()
    sys.exit(0)


signal.signal(signal.SIGINT, signal_handler)

while True:
    # 使用 select 實現非阻塞式的 socket 接收
    ready = select.select([s], [], [], 5)  # 等待 5 秒
    if ready[0]:
        data, client_addr = s.recvfrom(1024)
        print('Echo:', data.decode())
        s.sendto(data, client_addr)
    else:
        print('No data received')
