import socket
from pathlib import Path
import os
import time

port = 60001
s = socket.socket()
host = ""

s.bind((host, port))
s.listen(5)

while True:
    print("Hello!! Server is up and running...")
    conn, addr = s.accept()
    no_files = conn.recv(1024)
    time.sleep(0.1)
    print no_files
    no_files = int(no_files)
    print no_files
    file_names = []
    file_sizes = []
    for i in range(no_files):
        file_name = conn.recv(1024)
        time.sleep(0.1)
    #    file_name = './Data/' + file_name
        file_names.append(file_name)
        size = os.path.getsize(file_name)
        file_sizes.append(size)

    print file_names
    print file_sizes

    for i in range(no_files):
        conn.send(str(file_sizes[i]))
        print file_sizes[i]
        time.sleep(0.1)

    for i in range(no_files):
        filename = file_names[i]
        print filename
        my_file = Path(filename)
        if not my_file.is_file():
            conn.send("Sorry! The file was not found!")
            print("Sorry! The file was not found!")
            continue

        conn.send(" ")
        time.sleep(0.2)
        print 'Got connection from', addr
        f = open(filename,'rb')
        l = f.read(1024)
        while (l):
           conn.send(l)
           print('Sent ',repr(l))
           l = f.read(1024)
        f.close()
    #
    # print('Done sending')
    conn.close()
