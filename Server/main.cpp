#include <iostream>


#include "main.h"
#include "Server.h"
#include <string>
using namespace std;

int main(int argc,char *argv[]){

    int recRate = 4;//Receive recRate packets
    int n_rec =5; // for every n_rec packets-> Simulate packet loss, fail to reach server
    int sendRate = 4;//Send sendRate packets
    int n_send = 5;// for every n_send packets-> Simulate packet loss, fail to send back to client
    int port = 8888;//Port number
    Semantic e = AT_MOST_ONCE;//Execution semantics
    if (argc<7){
        cout<<"Please enter all 6 arguments"<<endl;
        cout<<"server.exe {port_number} {sucess_rec_rate} {total_rec} {success_send_rate} {total_send} {AT_LEAST_ONCE|AT_MOST_ONCE}"<<endl;
        return 0;
    }
    //Initialize Server info
    for(int i=0;i<argc;i++){
        if(i==1){
            port = stoi(argv[i]);
        }else if(i==2){
            recRate = stoi(argv[i]);
        }else if(i==3){
            n_rec = stoi(argv[i]);
        }else if(i==4){
            sendRate = stoi(argv[i]);
        }else if(i==5){
            n_send = stoi(argv[i]);
        }else if(i==6){
            if(string(argv[i])=="AT_LEAST_ONCE"){
                e = Semantic::AT_LEAST_ONCE;
            }else{
                e = Semantic::AT_MOST_ONCE;
            }
        }
    }

    UDPSocket socket = UDPSocket(recRate,n_rec,sendRate,n_send);
    socket.bindTo(port);

    Server server = Server(&socket,e);
//
    while(1){

        cout<<"Listening on "<<socket.pprint();
        string request = server.listen();
        string response = server.handleRequest(request);
        server.send(response);

    }

};
