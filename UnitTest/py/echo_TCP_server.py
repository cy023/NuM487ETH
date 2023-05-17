import signal
import socket
import sys

# HOST = '127.0.0.1'
HOST = '192.168.0.220'
PORT = 7

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
print('===== Listening on', str((HOST, PORT)), '=====')

# 將 socket 設定為 non-blocking 模式
s.setblocking(False)

# 記錄已連線的 socket 和其對應的位址
clients = {}

def signal_handler(sig, frame):
    print('KeyboardInterrupt')
    s.close()
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

while True:
    try:
        conn, addr = s.accept()
        print('Connected by', str(addr))
        
        # 將新連線的 socket 設定為 non-blocking 模式
        conn.setblocking(False)
        clients[conn] = addr

    except BlockingIOError:
        pass

    for client in list(clients):
        try:
            data = client.recv(1024)
            if data:
                print('Echo:', data.decode())
                client.send(data)
            else:
                print('Closed', str(clients[client]), 'connection.')
                client.close()
                del clients[client]

        except BlockingIOError:
            pass
