#include "networking.h"

int main(int argc, char **argv) {

    int server_socket;
    char buffer[BUFFER_SIZE];
    char port[BUFFER_SIZE];
    printf("Enter port# between 49152-65535: ");
    fgets(port, sizeof(port), stdin);
    *strchr(port, '\n') = 0;
    if (argc == 2)
        server_socket = client_setup( argv[1], port);
    else
        server_socket = client_setup( TEST_IP, port );

    while (1) {
        printf("enter data: ");
        fgets(buffer, sizeof(buffer), stdin);
        *strchr(buffer, '\n') = 0;
        write(server_socket, buffer, sizeof(buffer));
        read(server_socket, buffer, sizeof(buffer));
        printf("received: [%s]\n", buffer);
    }
}