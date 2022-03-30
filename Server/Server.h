//
// Created by Marvin on 29/03/2022.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H
#define BUFF_SIZE 512
#define MAX_HISTORY 5
#include "UDPSocket.h"
#include "RequestData.h"
#include "Request.h"
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <queue>
#include <utility>
#include <time.h>
#include <sstream>
#include <chrono>

#include "Response.h"
#include "ResponseData.h"

//Account struct
struct Account{
    string name;
    string password;
    SerEnum::Currency_e currency;
    float amount;
    int account;

    //String serializer for when we need to send updates of acc info
    string text(){
        string out = "{";
        out += R"("name":")"+name+"\",";
        out += R"("account":")"+to_string(account)+"\",";
        out+= R"("currency":")"+SerEnum::toString(currency)+"\",";
        out += R"("amount":")"+ f_to_string(amount,2)+"\"}";

        return out;
    }

};

enum Semantic{
    AT_LEAST_ONCE,
    AT_MOST_ONCE
};
//Actions info for when we send account update to subscribers
struct UPDATE{
    enum action{
        OPEN,
        CLOSE,
        SEND,
        RECEIVE,
        DEPOSIT,
        WITHDRAW,
        CHANGE_NAME
    };

    static string toString(action a){
        switch(a){
            case OPEN:return"OPEN";
            case CLOSE:return"CLOSE";
            case SEND:return"SEND";
            case RECEIVE:return"RECEIVE";
            case DEPOSIT:return"DEPOSIT";
            case WITHDRAW:return"WITHDRAW";
            case CHANGE_NAME:return"CHANGE_NAME";

        };
    }

};

//Comparator for subscribers priority queue
struct comparator{
    constexpr bool operator()(
            pair<sockaddr_in,time_t>& a,
            pair<sockaddr_in,time_t>& b) const noexcept;
};
class Server {
public:
    UDPSocket* sock;
    sockaddr_in client;
    Semantic execution;

    //Accounts, keyed by account num
    unordered_map<int,Account> users;

    //History, keyed by session id, each sessionID
    // have MAX_HISTORY previous requests stored, keyed by request Id
    //map is ordered, so we can remove oldest element(smallest request Id)
    unordered_map<int,map<int,string>> history;

    //Generator for generating account number, just increment
    int generator;

    //Generator for generating session id, just increment
    int session;

    //Keep track of subscribers in priority queue, ordered by end_time of their subscription
    //Those that end earliest are at the top of the queue, they are removed
    //if current time is past the end_time
    typedef pair<sockaddr_in,time_t> subEntry;
    priority_queue<subEntry,vector<subEntry>,comparator> subscribers;

    //Hashmap for all subscribers, since priority_queue is non iterable, we keep track of them here
    //To iterate over them when we need to notify account updates
    unordered_map<string,sockaddr_in> subsMap;

    //Server actions
    Response createAccount(CreateAccReq req);
    Response deleteAccount(DeleteAccountReq req);
    Response depositWithdraw(DepWithReq req);
    Response subscribe(SubscribeReq req);
    Response changeName(ChangeNameReq req);
    Response transfer(TransferReq req);
    Response setUp();

    //Update history when request is made, for AT_MOST_ONCE semantics
    void updateHistory(int session,int reqId, string response);

    //Check if request id has been made by the client with session id before
    //If yes, return history, for AT_MOST_ONCE semantics
    boolean checkHistory(int session, int reqId);

    //Check authorization validity for requests
    //Whenever user name, password, and account number are required
    template <typename T>
    void checkAccount(T req, Response* res) {
        auto it = this->users.find(req.account);

        if (it == this->users.end() || it->second.name != req.name) {
            res->message = "User not found";
            res->code = Codes::REFUSED;
        } else {
            if (it->second.password != req.password) {
                res->message = "Incorrect password";
                res->code = Codes::UNAUTHORIZED;
            }
        }
    }

    Response processRequest(Request req);
    //Notify subscribers when there is an account change
    void notify(Account updated,UPDATE::action a);

    Server(UDPSocket* port,Semantic e);
    string handleRequest(string s);
    string listen();

    //Socket send message wrapper, send to client that made the latest request
    int send(sockaddr_in client,string message);
    int send(string message);

    //Lossless send to notify account update, otherwise most of them are dropped by the
    //Simulated packet send loss
    int sendLossless(sockaddr_in client,string message);
    int sendLossless(string message);



};


#endif //SERVER_SERVER_H
