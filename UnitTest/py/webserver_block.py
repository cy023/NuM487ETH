import socket

# Create a new socket using the given address family, socket type and protocol number.
# AF_INET     means using IPv4.
# SOCK_STREAM means using TCP.
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to address (IP address : port).
HOST = '127.0.0.1'
PORT = 80
serversocket.bind((HOST, PORT))

# Enable a server to accept connections.
serversocket.listen()

print('========== Server ({}:{}) OPEN. =========='.format(HOST, PORT))

# Waiting for connections.
while True:

    # Accept a connection.
    # connectionsocket : a new socket object usable to send and receive data on the connection.
    # addr             : the address bound to the socket on the other end of the connection.
    connectionsocket, addr = serversocket.accept()
    print('===== Client ({}:{}) Connect.    ====='.format(addr[0], addr[1]))

    # Get message from client.
    clientRequest = connectionsocket.recv(1024)
    print(clientRequest)

    # HTTP response message
    Status     = 'HTTP/1.1 200 OK\r\n'
    Header     = 'Connection: close\r\nContent-Type: text/html\r\n'
    Blank_line = '\r\n'
    # Body       = '<html>\r\n<body>\r\n<h1><center>Welcome to MVMC Lab.</center></h1>\r\n</body>\r\n</html>'
    with open('UnitTest/py/index.html', 'r') as f:
        Body = f.read()
    # print(Body)

    # Send Response.
    connectionsocket.sendall(bytes(Status    , 'utf-8'))
    connectionsocket.sendall(bytes(Header    , 'utf-8'))
    connectionsocket.sendall(bytes(Blank_line, 'utf-8'))
    connectionsocket.sendall(bytes(Body      , 'utf-8'))

    # Mark the socket closed.
    connectionsocket.close()
