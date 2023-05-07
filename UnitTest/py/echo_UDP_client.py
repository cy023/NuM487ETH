import socket
import signal
import sys
import select

# HOST = '127.0.0.1'
HOST = '192.168.0.23'
PORT = 7

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.connect((HOST, PORT))
print('Connect to echo server', str((HOST, PORT)))


def signal_handler(sig, frame):
    print('KeyboardInterrupt')
    s.close()
    sys.exit(0)


signal.signal(signal.SIGINT, signal_handler)

while True:
    outdata = input('please input message: ')
    print('send:', outdata)
    s.sendall(outdata.encode())

    # 使用 select 實現非阻塞式的 socket 接收
    ready = select.select([s], [], [], 5)  # 等待 5 秒
    if ready[0]:
        indata = s.recv(1024)
        print('recv:', indata.decode())
    else:
        print('No response')
