import socket

HOST = "192.168.166.128"
PORT = 8080

s = socket.socket(socket.AF_INET, \
	socket.SOCK_STREAM)

s.bind((HOST, PORT))
s.listen(1)
conn, addr = s.accept()

conn.recv(1024)
conn.sendall("world!")
conn.close()

