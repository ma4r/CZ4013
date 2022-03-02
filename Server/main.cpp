#include <iostream>


#include "main.h"
#include "UDPSocket.h"
using namespace std;

int main(){


    cout<<"SOCKET DONE";

    UDPSocket server = UDPSocket();
    server.bindTo(8888);

    char recBuff[512];
    for(int i=0;i<5;i++) {
        sockaddr_in client = server.readFrom(recBuff, 512);

        char sendBuff[] = "Hi from C++";
        int buffSie = strlen(sendBuff);
        server.sendTo(sendBuff,buffSie,client,0);
        cout << recBuff << endl;
    }

    




}