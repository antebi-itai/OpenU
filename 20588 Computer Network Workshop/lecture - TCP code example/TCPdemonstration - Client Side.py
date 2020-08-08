import socket

HOST = '192.168.166.130'
PORT = 8080

s = socket.socket(socket.AF_INET,\
	socket.SOCK_STREAM)

s.connect((HOST, PORT))

s.sendall("hello")
s.recv(1024)
s.close()

