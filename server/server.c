#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define BUFFERSIZE 1024

#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;  // sockaddr_in - references elements of the socket address. "in" for internet
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *request_input = "Please enter the file to be downloaded:  ";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)  // creates socket, SOCK_STREAM is for TCP. SOCK_DGRAM for UDP
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //This is to lose the pesky "Address already in use" error message
    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
    //                                               &opt, sizeof(opt))) // SOL_SOCKET is the socket layer itself
    // {
    //     perror("setsockopt");
    //     exit(EXIT_FAILURE);
    // }
    address.sin_family = AF_INET;  // Address family. For IPv6, it's AF_INET6. 29 others exist like AF_UNIX etc.
    address.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP address - listens from all interfaces.
    address.sin_port = htons( PORT );    // Server port to open. Htons converts to Big Endian - Left to Right. RTL is Little Endian

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Port bind is done. You want to wait for incoming connections and handle them in some way.
    // The process is two step: first you listen(), then you accept()
    if (listen(server_fd, 3) < 0) // 3 is the maximum size of queue - connections you haven't accepted
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
while(1)
{
    // returns a brand new socket file descriptor to use for this single accepted connection. Once done, use send and recv
    printf("hello!!  Server is up and running...\n");

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
//    printf("%d\n",new_socket);
    valread = read( new_socket , buffer, 1024);  // read infromation received into the buffer
    printf("Client: %s\n",buffer );
    send(new_socket , request_input , strlen(request_input) , 0 );  // use sendto() and recvfrom() for DGRAM
    printf("Server: Requested the Client to enter the file to be downloaded...\n");
    char file_name[BUFFERSIZE]="";
    
    printf("file_name before is %s\n",file_name);
    read(new_socket,file_name,1024);
    printf("%s\n",file_name);
    printf("Server: The client has requested to download %s file\n",file_name);

    int src_fd, dst_fd, n, err;
    char * dest = malloc(1024);
    dest[0] = 0;
    strcat(dest,"./../client/");
    strcat(dest,file_name);
    src_fd = open(file_name, O_RDONLY);
    if(src_fd == -1)
    {
      printf("Server: Sorry the file could not be found\n");
      continue;
    }
    dst_fd = open(dest, O_CREAT | O_WRONLY);


    while (1) {
        err = read(src_fd, buffer, 4096);
        if (err == -1) {
            printf("Error reading file.\n");
            exit(1);
        }
        n = err;

        if (n == 0) break;

        err = write(dst_fd, buffer, n);
        if (err == -1) {
            printf("Error writing to file.\n");
            exit(1);
        }
    }

    printf("Yeay! The file has been downloaded\n");
}
    return 0;
}
