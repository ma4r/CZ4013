//
// Created by Marvin on 03/03/2022.
//

#ifndef SERVER_UDPSOCKET_H
#define SERVER_UDPSOCKET_H

# include <stdio.h>
#  include <WinSock2.h>
#  include <Ws2tcpip.h>
# include <Mstcpip.h>
#include <string>

#  pragma comment(lib, "Ws2_32.lib")
#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR, 12)
//WSA Subsystem init and clean up after close, required for sockets in windows
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

//Wrapper class for udp socket
class UDPSocket {


private:
    sockaddr_in localAddr;
    SOCKET sock;
    static WSASubsystem WSASess;
    int recRate,sendRate;
    int n_rec,n_send;
    int rec = 0;
    int snd=0;

public:


    std::string pprint(){
        char s[INET_ADDRSTRLEN];

        inet_ntop(AF_INET,&(localAddr.sin_addr),s,INET_ADDRSTRLEN);
        int port = htons(localAddr.sin_port);
        return std::string(s)+":"+std::to_string(port);
    }
    UDPSocket(int recRate,int n_rec,int sendRate,int n_send) {
        this->recRate =recRate;
        this->sendRate = sendRate;
        this->n_rec = n_rec;
        this->n_send = n_send;
        sock = INVALID_SOCKET;
        sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if (sock == INVALID_SOCKET) {
            printf("Error at socket(): %d\n", WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        //Error handler if udp packet failed to be sent;
        BOOL bNewBehavior = FALSE;
        DWORD dwBytesReturned = 0;
        WSAIoctl(sock, SIO_UDP_CONNRESET, &bNewBehavior, sizeof bNewBehavior, NULL, 0, &dwBytesReturned, NULL, NULL);

    }

    ~UDPSocket() {
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

    //Read from socket wrapper classs
    sockaddr_in readFrom(char recvBuffer[], int buffSize) {
        sockaddr_in from;
        int addrSize = sizeof(from);

        memset(recvBuffer, '\0', buffSize);
        int n;
        do  {
            n = recvfrom(sock, recvBuffer, buffSize, 0, (sockaddr *) (&from),
                         &addrSize);
            this->rec = (this->rec+1)%this->n_rec;
        }while(this->rec>=this->recRate);//Simulate packet fail to reach server


        if (n < 0) {
            printf("Read Socket failed with error code : %d", WSAGetLastError());
            exit(1);
        }

        return from;
    }

    //Send to dest addres wrapper class
    int sendTo(char sendBuff[], int buffSize, sockaddr_in dest, int flags = 0) {

        int destSize = sizeof(dest);

        this->snd = (this->snd+1)%this->n_send;

        if (this->snd<this->sendRate) {
            return sendto(sock, sendBuff, buffSize, flags,
                          reinterpret_cast<const sockaddr *>(&dest), destSize);//Simulate packet fail to reach client
        }
        else{
            printf("Send Lost\n");
            return 0;
        }
    }

    //Lossless send socket, for notification because otherwise most notifications will be dropped
    int sendToLossless(char sendBuff[], int buffSize, sockaddr_in dest, int flags = 0) {

        int destSize = sizeof(dest);


        return sendto(sock, sendBuff, buffSize, flags,
                  reinterpret_cast<const sockaddr *>(&dest), destSize);


    }

    int sendTo(char sendBuff[],int buffSize,char* destIP,u_short destPort,int flags=0) {
        sockaddr_in dest = {};
        dest.sin_family = AF_INET;
        inet_pton(AF_INET, destIP, &dest.sin_addr.s_addr);
        dest.sin_port = htons(destPort);

        int destSize = sizeof(dest);


        return sendto(sock, sendBuff, buffSize, flags,
                      reinterpret_cast<const sockaddr *>(&dest), destSize);
    }

};



#endif //SERVER_UDPSOCKET_H
