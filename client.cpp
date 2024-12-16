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
    /*
    // creating socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);

    // IP of the other PC
    serverAddress.sin_addr.s_addr = inet_addr("192.168.1.32");

    // sending connection request
    connect(clientSocket, (struct sockaddr*)&serverAddress,sizeof(serverAddress));
    */
    // 1.READ FILE
    // 2.SEND FILE CONTENTS
    // 3.CLOSE
    
    // 1
    std::ifstream readfile;
    readfile.open(argv[1],std::ios::binary);    // File is the first Argument
    if(!readfile.is_open()){
        std::cout << "Problem opening the file ! Aborting..."<<std::endl;
        return 0;
    }

    struct stat buf;
    std::ofstream writefile;

    // file does not exist
    if(stat(argv[2],&buf) != -1){
        writefile.open(argv[2],std::ios::binary | std::ios::app);
        if(!writefile.is_open()){
            std::cout << "Problem opening the file ! Aborting..."<<std::endl;
            return 0;
        }
    // file does exist
    }else{
        writefile.open(argv[2],std::ios::binary | std::ios::trunc);
        if(!writefile.is_open()){
            std::cout << "Problem opening the file ! Aborting..."<<std::endl;
            return 0;
        }
    }

    // remove file

    // Get the length of the file
    readfile.seekg (0, readfile.end);
    int length = readfile.tellg();
    readfile.seekg (0, readfile.beg);
    std::cout << "Number to excract : "<<length<< std::endl;


    /* known size approach */
    char buffer[1024] = { 0 };

    // filesize < buffersize
    if(length < sizeof(buffer)){
        readfile.read(buffer,length);
        writefile.write(buffer,length);

    // filesize > buffersize
    }else{
        int repetitions = sizeof(buffer) / length;
        int lastRep = sizeof(buffer) % length;

        std::cout << "repetitions: "<<repetitions<<std::endl;
        std::cout << "lastRep: "<<lastRep<<std::endl;


        // write 'repetitions' times 'buffer' into 'writefile'
        for(int i = 0;i < repetitions; i++){
            readfile.read(buffer,sizeof(buffer));
            writefile.write(buffer,sizeof(buffer));
        }
        // size=1034, buffer=1024, so lastRep will equal 10, so 10 additional
        // bits will be written
        if(lastRep != 0){
            readfile.read(buffer,lastRep);
            writefile.write(buffer,lastRep);
        }
    }

    /* unknown size approach

    char buffer[1024] = { 0 };
    while(readfile.read(buffer, sizeof(buffer))){
        writefile.write(buffer,readfile.gcount());//outfile
        //std::cout.write(buffer, readfile.gcount());//console
    }
    
    // Handle any remaining bytes (last partial chunk)
    if (readfile.gcount() > 0) {
        writefile.write(buffer,readfile.gcount());//outfile
        //std::cout.write(buffer, readfile.gcount());//console
    }
    */

    // 5
    readfile.close();
    writefile.close();

    /*
    //sending data
    std::string myString;
    std::getline(std::cin, myString);

    for(int i = 0; i < myString.length(); i++){
        myString[i] = myString[i] + 13;
        if(myString[i] > 126){
            myString[i] = 33 + (126 - myString[i]);
        } 
    }
    std::cout << "Encrypted message being sent: " << myString << "." << std::endl;

    // bottom - 33
    // top - 126

    const char* message = myString.c_str();

    send(clientSocket, message, strlen(message),0);

    // sending data
    //const char* message = "Hello, server!";
    //send(clientSocket, message, strlen(message), 0);

    // std::ifstream file("source.docx", std::ios::binary);

    // closing socket
    close(clientSocket);
    */

    return 0;
}