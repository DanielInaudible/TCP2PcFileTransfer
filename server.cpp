// C++ program to show the example of server application in
// socket programming
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <fstream>

#include <sys/stat.h>

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

    char buffer[1024] = { 0 };

    // Get name of file
    recv(clientSocket, buffer, sizeof(buffer), 0);

    cout << " [SYSTEM] Received from initial: " << buffer << std::endl;

    // Create File
    std::ofstream writefile;
    writefile.open(buffer,std::ios::binary | std::ios::app);
    if(!writefile.is_open()){
        std::cout << "[SYSTEM]Problem opening the file ! Aborting..."<<std::endl;
        return 0;
    }

    // Get size of file
    recv(clientSocket, buffer, 128, 0);
    std::cout << "[SYSTEM]Size of the file plaintext : "<<buffer<< std::endl;

    int length = std::atoi(buffer);

    std::cout << "[SYSTEM]Size of file : "<<length<< std::endl;

    for(int i = 0; i < sizeof(buffer); i++){
        buffer[i] = '\0';
    }
    

    // Write everything to the file
    // Receive & Write to file
    // filesize < buffersize
    if(length < sizeof(buffer)){
        recv(clientSocket, buffer, length,0);
        writefile.write(buffer,length);
        //writefile.write(buffer,length);

    // filesize > buffersize
    }else{
        int repetitions = length / sizeof(buffer);
        int lastRep = length % sizeof(buffer);

        std::cout << "[SYSTEM]repetitions: "<<repetitions<<std::endl;
        std::cout << "[SYSTEM]lastRep: "<<lastRep<<std::endl;


        // write 'repetitions' times 'buffer' into 'writefile'
        for(int i = 0;i < repetitions; i++){
            recv(clientSocket, buffer, sizeof(buffer),0);
            writefile.write(buffer,sizeof(buffer));
        }
        // size=1034, buffer=1024, so lastRep will equal 10, so 10 additional
        // bits will be written
        if(lastRep != 0){
            recv(clientSocket, buffer, lastRep,0);
            writefile.write(buffer,lastRep);
        }
    }
    
    // closing
    writefile.close();
    close(serverSocket);

    return 0;
}
