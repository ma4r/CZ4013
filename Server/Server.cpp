//
// Created by Marvin on 29/03/2022.
//

#include "Server.h"


//Handler, calls different functions based on action requested
//Also performs the appropriate deserialization
//Returns response object with data from deserialized DTO objects
Response Server::processRequest(Request req) {
    switch (req.action){
        case SerEnum::Actions_e::CREATE_ACC:{
            auto temp = fromJson<CreateAccReq>(req.message);
            Response das= createAccount(temp);
            return das;
        }
        case SerEnum::Actions_e::CLOSE_ACC:{
            auto temp = fromJson<DeleteAccountReq>(req.message);
            return deleteAccount(temp);
            break;
        }case SerEnum::Actions_e::DEP_WITH:{
            auto temp = fromJson<DepWithReq>(req.message);
            return depositWithdraw(temp);
            break;
        }case SerEnum::Actions_e::SUBSCRIBE:{
            auto temp = fromJson<SubscribeReq>(req.message);
            return subscribe(temp);
            break;
        }case SerEnum::Actions_e::CHANGE_NAME:{
            auto temp = fromJson<ChangeNameReq>(req.message);
            return changeName(temp);
            break;
        }case SerEnum::Actions_e::TRANSFER:{
            auto temp = fromJson<TransferReq>(req.message);
            return transfer(temp);
            break;
        }
        case SerEnum::Actions_e::SET_UP:{
            return setUp();
        }
        default:
            break;
        }
}
//Create account
Response Server::createAccount(CreateAccReq req){
    Response res;

    struct Account account = {
            req.name,
            req.password,
            req.currency,
            req.amount,
            generator
            };
    //Acc number
    this->users[generator] = account;

    //Set header
    res.code = Codes::SUCCESS;
    res.message = "Account created";

    //Deserialize result DTO
    auto temp = CreateAccountDTO();
    temp.account = generator;
    res.data = toJson<CreateAccountDTO>(temp);

    generator++;

    //Update subscribers
    notify(account,UPDATE::OPEN);

    return res;
}

//Delete account
Response Server::deleteAccount(DeleteAccountReq req) {

    Response res;
    //Check account name,number,password
    checkAccount(req,&res);
    if (res.code == Codes::BASE){

        Account acc = this->users[req.account];

        //Notify subscribers
        notify(acc,UPDATE::CLOSE);

        this->users.erase(req.account);

        //Set headers
        res.message = "Account deleted";
        res.code = Codes::SUCCESS;

        //Deserialize result DTO
        auto temp = DeleteAccountDTO();
        temp.account = req.account;
        res.data = toJson<DeleteAccountDTO>(temp);
    }

    return res;
}

//Deposit or withdraw money from account
Response Server::depositWithdraw(DepWithReq req) {
    Response res;
    //Check account name,number,password
    checkAccount(req,&res);
    if (res.code == Codes::BASE) {
        Account acc = this->users[req.account];

        //Check request currency and source account currency
        if(req.currency != acc.currency) {
            res.code = Codes::REFUSED;
            res.message = "Account has " + SerEnum::toString(acc.currency) + " currency type.\n" +
                          "Request is of " + SerEnum::toString(req.currency) + " currency type.";
        }

        //Check if withdrawal is valid
        else if(req.amount + acc.amount <0){
            res.code = Codes::REFUSED;
            res.message = "Not enough balance, account has "+ f_to_string(acc.amount)+"\n"
                          +"Requested withdrawal "+f_to_string(abs(req.amount));
        }else{
            this->users[req.account].amount+=req.amount;

            //Set headers
            res.code = Codes::SUCCESS;
            res.message = req.amount>0?"Deposit Successful":"Withdrawal Successfull";

            //Notify Subscribers
            notify(acc, req.amount>0?UPDATE::DEPOSIT:UPDATE::WITHDRAW);

            //Deserialize result DTO
            auto temp = DepWithDTO();
            temp.currency = acc.currency;
            temp.newBalance =  this->users[req.account].amount;

            res.data = toJson<DepWithDTO>(temp);


        }
    }
    return res;

}

//Subscribe to account update notifications
Response Server::subscribe(SubscribeReq req) {
    auto tmp = chrono::system_clock::now().time_since_epoch();
    time_t startT = chrono::duration_cast<chrono::seconds>(tmp).count();
    time_t endT = startT+req.interval;

    this->subscribers.push(make_pair(this->client,endT));

    auto rem = this->client;
    char addr[INET_ADDRSTRLEN];
    inet_ntop(rem.sin_family,&rem.sin_addr,addr,INET_ADDRSTRLEN);

    string k = string(addr)+":"+to_string(rem.sin_port);

    this->subsMap[k] = this->client;

     Response res{
        Codes::SUCCESS,
        "Subscribed successfully"
    };
    auto temp = SubscribeDTO();
    temp.start = startT;
    temp.end = endT;
    res.data = toJson<SubscribeDTO>(temp);

    return res;
}

//Function to notify subscribers
void Server::notify(Account updated, UPDATE::action a) {
    string message = R"({"action":")"+UPDATE::toString(a);
    message += R"(","account":)"+updated.text()+"}";

    while(!this->subscribers.empty() &&
          this->subscribers.top().second<chrono::duration_cast<chrono::seconds>(
                  chrono::system_clock::now().time_since_epoch()
                  ).count()
            ){
        auto rem = this->subscribers.top().first;
        char addr[INET_ADDRSTRLEN];
        inet_ntop(rem.sin_family,&rem.sin_addr,addr,INET_ADDRSTRLEN);

        string k = string(addr)+":"+to_string(rem.sin_port);

        this->subsMap.erase(k);
        this->subscribers.pop();
    }

    for(auto &it:this->subsMap){
        this->sendLossless(it.second,message);

    }



}

//Change name associated with account number
Response Server::changeName(ChangeNameReq req) {
    Response res;
    //Check account name,number,password
    checkAccount(req,&res);
    if (res.code == Codes::BASE) {
        this->users[req.account].name=req.newName;

        //Set Headers
        res.code = Codes::SUCCESS;
        res.message = "Name change success";

        //Deserialize result DTO
        auto temp = ChangeNameDTO();
        temp.oldName = req.name;
        temp.newName = req.newName;

        //Notify subscribers
        notify(this->users[req.account],UPDATE::CHANGE_NAME);

        res.data = toJson<ChangeNameDTO>(temp);
    }
    return res;
}

//Transfer money from one account to another
Response Server::transfer(TransferReq req) {
    Response res;
    //Check account name,number,password
    checkAccount(req,&res);

    if (res.code == Codes::BASE){
        Account acc = this->users[req.account];
        auto it = this->users.find(req.targetAcc);

        //Check if target acc is valid
        if(it == this->users.end()){
            res.code = Codes::REFUSED;
            res.message = "Target account "+ to_string(req.targetAcc)+" does not exist";

        //Check if source account and request currency match
        }else if (acc.currency != req.currency){
            res.code = Codes::REFUSED;
            res.message = "Account has " + SerEnum::toString(acc.currency) + " currency type.\n" +
                          "Request is of " + SerEnum::toString(req.currency) + " currency type.";

        //Check if target account currency match
        }else if(it->second.currency != req.currency){
            res.code = Codes::REFUSED;
            res.message = "Target account has " + SerEnum::toString(it->second.currency) + " currency type.\n" +
                          "Request is of " + SerEnum::toString(req.currency) + " currency type.";

        //Check if source account has enough balance
        }else if(acc.amount-req.amount<0){
            res.code = Codes::REFUSED;
            res.message = "Not enough balance, account has "+ f_to_string(acc.amount)+"\n"
                    +"Requested transfer "+f_to_string(req.amount);

        }else{
            this->users[req.account].amount -= req.amount;
            this->users[req.targetAcc].amount += req.amount;

            //Notify subscribers
            notify(this->users[req.account],UPDATE::SEND);
            notify(this->users[req.targetAcc],UPDATE::RECEIVE);

            //Set headers
            res.code = Codes::SUCCESS;
            res.message = "Transfer Success";

            //Deserialize result DTO
            auto temp = TransferDTO();
            temp.targetAcc = req.targetAcc;
            temp.targetName = this->users[req.targetAcc].name;
            temp.amount = req.amount;
            temp.currency = req.currency;
            temp.balanceLeft = this->users[req.account].amount;

            res.data = toJson<TransferDTO>(temp);
        }
    }

    return res;


}

//Generate session id for client
Response Server::setUp() {

    Response res{
        Codes::SUCCESS,
        "Session acknowledged"
    };

    auto temp= SetUpDTO();
    temp.session = this->session;

    res.data = toJson<SetUpDTO>(temp);
    this->session ++;

    return res;

}

Server::Server(UDPSocket* socket, Semantic e) {
    this->sock = socket;
    this->users = {};
    this->generator = 0;
    this->session = 0;
    this->subscribers={};
    this->execution=e;

}

//Check if request was made previously, only on AT_MOST_ONCE semantic
boolean Server::checkHistory(int sess, int reqId) {
    auto it =this->history.find(sess);
    if( it == this->history.end()){
        return false;
    }

    else{
        auto sessHist = it->second;
        if( sessHist.count(reqId)==0){
            return false;
        }else{
            return true;
        }
    }
}

//Update history with request result, keep only MAX_HISTORY last requests
//only on AT_MOST_ONCE semantic
void Server::updateHistory(int session, int reqId, string response) {

    if(this->history.count(session) == 0){
        auto temp = map<int,string>();
        this->history[session] = temp;
    }

    auto sessMap = &this->history[session];
    if(sessMap->size() == MAX_HISTORY){
        cout<<"Max History Length "<<endl;
        cout<<"Removing "<<sessMap->begin()->first<<" from "<<session<<endl;
        sessMap->erase(sessMap->begin());
    }
    (*sessMap)[reqId] = response;

}


//General request handler, deserialize header, pass to processRequest,
//Check for past history if AT_MOST_ONCE semantic, serialize response header
// Update history if AT_MOST_ONCE semantic
string Server::handleRequest(string s) {
    Request req = fromJson<Request>(s);
    if(this->execution == Semantic::AT_MOST_ONCE && req.action != SerEnum::Actions_e::SET_UP){
        if(this->checkHistory(req.session,req.id)){
            return this->history[req.session][req.id];
        }
    }

    Response res = processRequest(req);
    string out = toJson<Response>(res);
    string data = "";

    if (!res.data.empty()){
        data += res.data;
    }
    out.pop_back();
    out+=",\"data\":"+data;
    if(data.empty()){
        out+= "null";
    }
    out += "}";

    if(this->execution == Semantic::AT_MOST_ONCE){
        this->updateHistory(req.session,req.id,out);
    }

    return out;



}

string Server::listen() {

    char recBuff[BUFF_SIZE];
    this->client = this->sock->readFrom(recBuff,BUFF_SIZE);
    cout<<"Receive:"<<string(recBuff)<<endl;

    return string(recBuff);
}

int Server::send(sockaddr_in client, string message) {
    cout<<"Sending:"<<message<<endl;
    char sendBuff[BUFF_SIZE];
    strcpy_s(sendBuff,BUFF_SIZE,message.c_str());

    return this->sock->sendTo(sendBuff,BUFF_SIZE,client,0);
}

int Server::send(string message) {
    return this->send(this->client,message);
}

int Server::sendLossless(sockaddr_in client, string message) {
    cout<<"Sending:"<<message<<endl;
    char sendBuff[BUFF_SIZE];
    strcpy_s(sendBuff,BUFF_SIZE,message.c_str());

    return this->sock->sendToLossless(sendBuff,BUFF_SIZE,client,0);
}

int Server::sendLossless(string message) {
    return this->sendLossless(this->client,message);
}

constexpr bool comparator::operator()(pair<sockaddr_in, time_t> &a, pair<sockaddr_in, time_t> &b) const noexcept {
    return a.second>b.second;
}
