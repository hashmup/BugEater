import socket
s = socket.socket()
host = '192.168.179.5'
port = 12345
s.connect((host, port))
print(s.recv(1024))
s.send('test')
s.close()
