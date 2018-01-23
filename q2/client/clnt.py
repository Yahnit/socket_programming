import socket
import sys
import time

no_files = len(sys.argv)-1
s = socket.socket()
host = ""
port = 60001

s.connect((host, port))
s.send(str(no_files))
time.sleep(0.1)
file_names = []
for i in range(no_files):
    file_name = './Data/' + sys.argv[i+1]
    file_names.append(file_name)
    s.send(file_name)
    time.sleep(0.1)

file_sizes = []

for i in range(no_files):
    size = s.recv(1024)
    file_sizes.append(int(size))
    time.sleep(0.1)

print file_sizes

for i in range(no_files):
    print i
    status = s.recv(1024)
    print status
    if status == "Sorry! The file was not found!":
        print status
        continue
    time.sleep(0.2)
    size = 0
    with open(sys.argv[i+1], 'wb') as f:
        while True:
            print('Downloading data...')
            data = s.recv(1024)
            size = size + len(data)
            print size
            time.sleep(0.1)
            if size == file_sizes[i]:
                f.write(data)
                break
            f.write(data)


print("Yeay! File has been downloaded...")
#f.close()
s.close()

print('connection closed')
