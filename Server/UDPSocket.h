//
// Created by Marvin on 03/03/2022.
//

#ifndef SERVER_UDPSOCKET_H
#define SERVER_UDPSOCKET_H

# include <stdio.h>
#  include <WinSock2.h>
#  include <Ws2tcpip.h>
#  pragma comment(lib, "Ws2_32.lib")
class WSASubsystem{
public:
    WSADATA data;
    WSASubsystem(){

        int wsaStart = WSAStartup( MAKEWORD( 2, 2 ), &(this->data) );

        if(wsaStart != NO_ERROR){
            printf("WSAStartup failed with error %d\n", wsaStart);
            exit(EXIT_FAILURE);
        }
    }

    ~WSASubsystem(){WSACleanup();}
};
class UDPSocket {


private:
    sockaddr_in localAddr;
    SOCKET sock;
    static WSASubsystem WSASess;

public:
    UDPSocket() {

        sock = INVALID_SOCKET;
        sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if (sock == INVALID_SOCKET) {
            printf("Error at socket(): %d\n", WSAGetLastError());
            exit(EXIT_FAILURE);
        }
    }

    ~UDPSocket(){
        closesocket(sock);
    }

    sockaddr_in bindTo(u_short port) {
        localAddr.sin_family = AF_INET;
        localAddr.sin_addr.s_addr = INADDR_ANY;
        localAddr.sin_port = htons(port);
        if (bind(sock, (struct sockaddr *) &localAddr, sizeof(localAddr)) == SOCKET_ERROR) {
            printf("Bind failed with error code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        return localAddr;
    }

    sockaddr_in readFrom(char recvBuffer[],int buffSize){

        sockaddr_in from;
        int addrSize = sizeof(from);

        memset(recvBuffer,'\0', buffSize);
        int n = recvfrom(sock, recvBuffer, buffSize, 0, (sockaddr *)(&from),
                         &addrSize);
        if (n < 0)
        {
            printf("Read Socket failed with error code : %d" , WSAGetLastError());
            exit(1);
        }

        return from;
    }

    int sendTo(char sendBuff[],int buffSize,sockaddr_in dest,int flags=0){
        int destSize= sizeof(dest);
        return sendto(sock, sendBuff, buffSize, flags, reinterpret_cast<const sockaddr *>(&dest), destSize);
    }

    int sendTo(char sendBuff[],int buffSize,char* destIP,u_short destPort,int flags=0){

        sockaddr_in dest = {};
        dest.sin_family=AF_INET;
        inet_pton(AF_INET,destIP,&dest.sin_addr.s_addr);
        dest.sin_port = htons(destPort);

        int destSize= sizeof(dest);
        return sendto(sock, sendBuff, buffSize, flags, reinterpret_cast<const sockaddr *>(&dest), destSize);
    }
};



#endif //SERVER_UDPSOCKET_H
