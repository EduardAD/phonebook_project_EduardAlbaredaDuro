#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

const char socket_name[]="./communication_channel";

int main(int argc, char *argv[]){
    
    int temp;
    int data_socket;
    struct sockaddr_un addr;

    //Socket creation
    data_socket = socket(AF_UNIX,SOCK_SEQPACKET, 0);
    if(data_socket == -1){
        perror("Error creating socket \n");
        exit(-1);
    }

    //Adress structure initialized to 0
    memset(&addr, 0, sizeof(struct sockaddr_un));

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_name, sizeof(addr.sun_path) - 1);
    
    //Bind the name and type, and create a nwe file

    temp = connect(data_socket, (const struct sockaddr *) &addr, sizeof(struct sockaddr_un));
    if (temp == -1){
        perror("binding");
        exit(-1);
    }


    char msg[11];
    char answer[11];
    while(1){
        printf("Message? ");
        scanf("%10s", msg); // We read up to 10 characters

        // if we put in QUIT, the client is finished
        if(strcmp(msg, "QUIT")==0){
            break;
        }

        // Send a message to the server
        temp = write(data_socket, msg, strlen(msg));
        if (temp == -1) {
            perror("write");
            exit(-1);
        }

        // We recieve a message with max. 10 characters
        temp = read(data_socket, answer, 10);
        if (temp == -1) {
            perror("read");
            exit(-1);
        }

        // Function read works on binary data, not strings!
        // In order to do that, we must conclude it with 0
        answer[temp] = 0;
        printf("odgovor: %s\n", answer);
    }
    // Close the connection
    close(data_socket);
    return 0;

}
