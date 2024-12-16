// C++ program to show the example of server application in
// socket programming
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

int main()
{
    // creating socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying the address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // binding socket.
    bind(serverSocket, (struct sockaddr*)&serverAddress,sizeof(serverAddress));

    // listening to the assigned socket
    listen(serverSocket, 5);

    // accepting connection request
    int clientSocket = accept(serverSocket, nullptr, nullptr);

    // recieving data
    char buffer[1024] = { 0 };
    recv(clientSocket, buffer, sizeof(buffer), 0);

    cout << "Encrypted message from client: " << buffer << endl;

    // Encryption / Decryption can be done with same function using modulo
    for(int i = 0; i < strlen(buffer); i++){
        buffer[i] = buffer[i] - 13;
        if(buffer[i] < 33 && buffer[i] != ' '){
            buffer[i] = 126 - (31 - buffer[i]);
        } 
    }

    cout << "Decrypted Message:"  << buffer << endl;
    // closing the socket.
    close(serverSocket);

    return 0;
}
