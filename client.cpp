// C++ program to illustrate the client application in the
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

int main(int argc,char* argv[]){
    
    // creating socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);

    // IP of the other PC
    // serverAddress.sin_addr.s_addr = inet_addr("172.0.0.1"); // loopback for local testing, later change to 192.168.1.32
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    
    // sending connection request
    connect(clientSocket, (struct sockaddr*)&serverAddress,sizeof(serverAddress));
    
    // Open file for Reading
    std::ifstream readfile;
    readfile.open(argv[1],std::ios::binary);    // File is the first Argument
    if(!readfile.is_open()){
        std::cout << "[SYSTEM]Problem opening the file ! Aborting..."<<std::endl;
        return 0;
    }

    // Send name of the file
    std::cout << "[SYSTEM]Sending name of the file : "<<argv[1]<< std::endl;

    // 1 SEND
    send(clientSocket, argv[1], strlen(argv[1]),0);

    // Send size of the file
    readfile.seekg (0, readfile.end);
    int length = readfile.tellg();
    readfile.seekg (0, readfile.beg);
    std::string dlzka = std::to_string(length);
    std::cout << "[SYSTEM]Size of file : "<<dlzka.c_str()<< std::endl;

    
    // Size of the file will max be 128 bytes, but our program will send 128 bytes and if the size of smaller, it will fill the remaining
    // bytes with /0
    char* a = new char(128-dlzka.length());
    for(int i = 0; i < 128-dlzka.length();i++){
        a[i] = '\0';
    }

    send(clientSocket, dlzka.c_str(), dlzka.length(),0);
    send(clientSocket, a, 128-dlzka.length(),0);

    // Send the content of file
    /* known size approach */
    char buffer[1024] = { 0 };

    // filesize < buffersize
    if(length < sizeof(buffer)){
        readfile.read(buffer,length);
        send(clientSocket, buffer, length,0);
        //writefile.write(buffer,length);

    // filesize > buffersize
    }else{
        int repetitions = length / sizeof(buffer);
        int lastRep = length % sizeof(buffer);

        std::cout << "[SYSTEM]repetitions: "<<repetitions<<std::endl;
        std::cout << "[SYSTEM]lastRep: "<<lastRep<<std::endl;


        // write 'repetitions' times 'buffer' into 'writefile'
        for(int i = 0;i < repetitions; i++){
            readfile.read(buffer,sizeof(buffer));
            send(clientSocket, buffer, sizeof(buffer),0);
        }
        // size=1034, buffer=1024, so lastRep will equal 10, so 10 additional
        // bits will be written
        if(lastRep != 0){
            readfile.read(buffer,lastRep);
            send(clientSocket, buffer, lastRep,0);
        }
    }

    // close
    readfile.close();
    close(clientSocket);

    return 0;
}