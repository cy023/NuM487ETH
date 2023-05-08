import socket
import sys
import select
import signal

HOST = '127.0.0.1'
PORT = 7

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setblocking(False)
try:
    s.connect((HOST, PORT))
except socket.error:
    pass

print('Connect to echo server', str((HOST, PORT)))

def signal_handler(sig, frame):
    print('KeyboardInterrupt')
    s.close()
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

while True:
    outdata = input('please input message: ')
    if outdata:
        print('send:', outdata)
        s.sendall(outdata.encode())

    ready = select.select([s], [], [])
    if ready[0]:
        indata = s.recv(1024)
        if len(indata) == 0: # connection closed
            s.close()
            print('server closed connection.')
            break
        print('recv:', indata.decode())
