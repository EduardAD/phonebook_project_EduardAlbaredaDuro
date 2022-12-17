#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdbool.h>  

const char socket_name[]="./communication_channel";
char msg[100];

int i = 0;
struct entry{
    char name[100];
    char surname[100];
    char number[30];
};



void add_number(int temp, int data_socket, struct entry phonebook[100]){
        bool found = false;
        for(int k = 0; k <=2; k++) {
            temp = read(data_socket, msg, 100);
            if (temp == -1) {
                perror("read");
                exit(-1);
            }
            if (temp == 0) {
                break;
            }

            msg[temp] = 0;

            switch (k) {
                case 0:
                    strcpy(phonebook[i].name, msg);
                    break;
                case 1:
                    strcpy(phonebook[i].surname, msg);
                    break;
                case 2:
                    strcpy(phonebook[i].number, msg);
                    for (int v = 0; v <= i; v++) {
                        fprintf(stderr,"%s \n\n", &phonebook[v].number[0]);
                        if (strcmp(phonebook[v].number, msg) == 0) {
                            temp = write(data_socket, "Number already exists! \n\n", 26);
                            found = true;
                            if (temp == -1) {
                                perror("write");
                                exit(-1);
                            }
                            strcpy(phonebook[i].name, "0");
                            strcpy(phonebook[i].surname, "0");
                            strcpy(phonebook[i].number, "0");
                            break;
                        }
                    }
                    break;
            }
            // Response with *
            if (found) {
                temp = write(data_socket, "New number added succesfully. \n\n", 33);
                if (temp == -1) {
                    perror("write");
                    exit(-1);
                }
            }
        }
        printf("%s \n\n", &phonebook[i].name[0]);
        printf("%s \n\n", &phonebook[i].surname[0]);
        printf("%s \n\n", &phonebook[i].number[0]);
        i++;
}

void delete_number(int temp, int data_socket, struct entry phonebook[100]){

    bool found = false;

    temp = read(data_socket, msg, 100);
    if(temp == -1){
        perror("read");
        exit(-1);
    }
    msg[temp] = 0;
    
    for(int p = 0; p <= i; p++){
        if(strcmp(phonebook[p].number,msg)==0){
            found = true;
            strcpy(phonebook[p].name,"0");
            strcpy(phonebook[p].surname,"0");
            strcpy(phonebook[p].number,"0");
            printf("%s \n\n", &phonebook[p].name[0]);
            printf("%s \n\n", &phonebook[p].surname[0]);
            printf("%s \n\n", &phonebook[p].number[0]);
        }
    }

    temp = write(data_socket, "Number deleted succesfully \n\n", 30);
    if (temp == -1) {
                perror("write");
                exit(-1);
    } 

    

}

void close_phonebook(int temp, int data_socket, int connection_socket){
    printf(" Connection closed by client \n\n");
    temp = write(data_socket, "Closed connection \n\n", 21);
    if (temp == -1) {
        perror("write");
        exit(-1);
    }
    close(connection_socket);
    unlink(socket_name);

    exit(-1);
}

int main(int argc, char *argv[]){

    int temp;
    int stop_server = 0;
    int data_socket;
    int connection_socket;
    struct sockaddr_un addr;
    struct entry phonebook[100];

    char instruction[25];

    //Socket creation
    connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if(connection_socket == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    //Adress structure initialized to 0
    memset(&addr, 0, sizeof(struct sockaddr_un));

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_name, sizeof(addr.sun_path) - 1);
    


    //Bind the name and type, and create a nwe file

    temp = bind(connection_socket, (const struct sockaddr *) &addr, sizeof(struct sockaddr_un));
    if (temp == -1){
        perror("binding");
        exit(-1);
    }

    // Configuration for waiting of the clients - up to 2 in the queue
    temp = listen(connection_socket, 0);
    if (temp == -1) {
        perror("listen");
        exit(-1);
    }

    // Loop for waiting on the clients to connect

    while(!stop_server){
        // We wait for a client, we obtain a new socket for a communication with it

        printf("Waiting for the client ...\n");
        data_socket = accept(connection_socket, NULL, NULL);
            if (data_socket == -1) {
                perror("accept");
                exit(-1);
            }
        // Loop for a communication with a single client
        while(1){
            // Wait for message
            printf("Reading the message ...\n\n");
            temp = read(data_socket, msg, 100);
                if (temp == -1) {
                    perror("read");
                    exit(-1);
                }
                if (temp == 0){
                    break;
                }
            
            if (strcmp(msg, "./phonebook_add") == 0){
                add_number(temp, data_socket, phonebook);
            }else if (strcmp(msg, "./phonebook_delete") == 0){
                delete_number(temp, data_socket, phonebook);
            }else if (strcmp(msg, "./phonebook_close") == 0){
                close_phonebook(temp, data_socket, connection_socket);
            }else if (strcmp(msg, "./phonebook_printout") == 0){
                add_number(temp, data_socket, phonebook);
            }else if (strcmp(msg, "./phonebook_search") == 0){
                add_number(temp, data_socket, phonebook);
            }
            break;
        }    
    }

    // Close and erase the socket for establishing the connection
    close(connection_socket);
    unlink(socket_name);

    exit(-1);
        
    

}

