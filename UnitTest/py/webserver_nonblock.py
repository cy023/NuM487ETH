import socket
import time

last_time = ""

# Create a new socket using the given address family, socket type and protocol number.
# AF_INET     means using IPv4.
# SOCK_STREAM means using TCP.
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Client list.
clients = []

# Bind the socket to address (IP address : port).
HOST = '127.0.0.1'
PORT = 80
serversocket.bind((HOST, PORT))

# Set blocking or non-blocking mode of the socket.
serversocket.setblocking(False)

# Enable a server to accept connections.
serversocket.listen()

print('========== Server ({}:{}) OPEN. =========='.format(HOST, PORT))

# Waiting for connections.
while True:
    try:
        # Accept a connection.
        # conn : a new socket object usable to send and receive data on the connection.
        # addr : the address bound to the socket on the other end of the connection.
        connectionsocket, addr = serversocket.accept()
        print('===== Client ({}:{}) Connect.    ====='.format(addr[0], addr[1]))

        # Set blocking or non-blocking mode of the socket.
        connectionsocket.setblocking(False)

        # Append client in the list.
        clients.append(connectionsocket)

    except:
        pass

    # Check every client request.
    for client in clients:
        try:
            # Get request from client.
            clientRequest = client.recv(1024)
            raddr = client.getpeername() # remote address (IP address : port).
            print('===== From ({}:{}) =====\n'.format(raddr[0], raddr[1]), clientRequest)

            # HTTP response message
            Status     = 'HTTP/1.1 200 OK\r\n'
            Header     = 'Connection: close\r\nContent-Type: text/html\r\n'
            Blank_line = '\r\n'
            # Body       = '<html>\r\n<body>\r\n<h1><center><valign>Welcome to MVMC Lab.</valign></center></h1>\r\n</body>\r\n</html>'
            with open('UnitTest/py/index.html', 'r') as f:
                Body = f.read()
            # print(Body)

            # Send Response.
            client.sendall(bytes(Status    , 'utf-8'))
            client.sendall(bytes(Header    , 'utf-8'))
            client.sendall(bytes(Blank_line, 'utf-8'))
            client.sendall(bytes(Body      , 'utf-8'))

            client.close()
            clients.remove(client)

        except:
            pass

    if last_time != time.ctime():
        print(time.ctime())
        last_time = time.ctime()
